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
require dasllama/dasllama_convert
require dasllama/dasllama_gemma4a   // gemma4a_attn_past: the Conformer cell's window
require dasllama/dasllama_qwen3a   // nolint:STYLE030,LINT019 — QWEN3A_TOK_PER_CHUNK: the front movers' qwen3a fixture, inside the vulkan arm alone
require dasllama/dasllama_audio   // nolint:STYLE030,LINT019 — build_dft_twiddles: the spectrum cell's twiddle table, inside the vulkan arm alone
require dasllama/dasllama_whisper   // nolint:STYLE030,LINT019 — wdec_kscale, the decoder's key pre-scale the K/V store cell feeds
require dasllama/dasllama_kqformat   // kq_elems: the Q8_0 block width that sizes the requant feed's scale plane
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require _attn_oracle   // attn_row_scored: the softmax row the attention oracles share
require _vk_kq_fixtures   // nolint:STYLE030,LINT019 — f16r, inside the vulkan arms alone
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 - the oracles' device-math twins, used only inside the vulkan static_if arms (a build without the module drops them)
require daslib/jobque_boost
require daslib/linq   // nolint:STYLE030,LINT019 — count: the sentinel words a dispatch left, the pads' zero tails, inside the vulkan arms alone

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

def private fa_bidir_arm(t : T?; khr : bool; hs : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "{khr ? "fa_khr" : "fa_cm2"} h{hs} bidir"
        let nheads = TOWER_NHEADS
        let rows = TOWER_ROWS
        let qd = nheads * hs
        let kpad = rows + TOWER_SLACK_ROWS
        var qh, kh, vh : array<float>
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
        let sh_bytes = int64(kpad * qd) * 2l
        with_cell_planes() $(var cp : CellPlanes) {
            let qp = cp |> in_(qh)
            let kp = cp |> in_(kh)
            let vp = cp |> in_(vh)
            let khp = cp |> out_words(sh_bytes, F16_SENTINEL_BITS)
            let vhp = cp |> out_words(sh_bytes, F16_SENTINEL_BITS)
            let yp = cp |> out_(rows * qd)
            let ycp = cp |> out_(rows * qd)
            var sck = cp |> cls_set(fixed_array(kp, khp), @@set_tower_clamp_cvt_cls)
            var scv = cp |> cls_set(fixed_array(vp, vhp), @@set_tower_clamp_cvt_cls)
            let fa = fixed_array(qp, khp, vhp, yp)
            let fc = fixed_array(qp, khp, vhp, ycp)
            var sfa = fa_stamp_set(khr, gather_planes(cp.dev, fa), gather_planes(cp.bytes, fa), gather_planes(cp.hazard_bit, fa))
            var sfc = fa_stamp_set(khr, gather_planes(cp.dev, fc), gather_planes(cp.bytes, fc), gather_planes(cp.hazard_bit, fc))
            var pcv = gk_f16_rows_args(uint(kpad * qd))
            var pcf = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(qd), kvd = uint(qd), kv_mul = 1u, kvbase = 0u,
                scale = TOWER_SCALE, window = 0u, sinkoff = 0u)
            let cvtwgs = int64((kpad * qd / 4 + 255) / 256)
            let wgs = fa_stamp_wgs(khr, int64(nheads), int64(rows))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tower_clamp_cvt_cls(raw, h, sck, pcv, cvtwgs)
                enc_tower_clamp_cvt_cls(raw, h, scv, pcv, cvtwgs)
                fa_stamp_enc(khr, int64(hs), false, false, false, false, raw, h, sfa, pcf, wgs, true)
                fa_stamp_enc(khr, int64(hs), false, false, false, false, raw, h, sfc, pcf, wgs)
            }
            var y <- cp |> plane_run(yp)
            var yc <- cp |> plane_run(ycp)
            var y_ref : array<float>
            y_ref |> resize(rows * qd)
            for (r in range(rows)) {
                attn_heads_oracle(qh, r * qd, kh, 0, vh, 0, qd, nheads, hs, 1, rows, TOWER_SCALE, y_ref, r * qd)
            }
            // `approx` covers the tile's f16 Q and P staging and the coopmat's reassociated sums; K and V are f16 on both sides
            check_rows(t, "{tag} every query row attending every key vs the CPU oracle", y, y_ref, Bar(rel = APPROX_REL, poison = 1.5), qd, hs)
            t |> success(logits_maxabs(y) < TOWER_SENTINEL * 0.01, "{tag}: the slack rows past kvlen stayed masked (max |o| {logits_maxabs(y)})")
            // the causal twin on the same planes: row 0 sees one key there and every key here, so its output moves
            let moved = logits_maxdiff(copy_span(y, 0, qd), copy_span(yc, 0, qd))
            t |> success(moved > 0.1, "{tag} control: the causal twin's first row differs (maxdiff {moved})")
            // ... while the last row sees every key on both stamps: its head rows agree to the bar
            t |> success(mismatch_approx(copy_span(y, (rows - 1) * qd, qd), copy_span(yc, (rows - 1) * qd, qd)) == 0, "{tag} control: the last query row agrees with the causal twin (every key on both)")
            delete y
            delete yc
            delete y_ref
        }
        delete qh
        delete kh
        delete vh
    }
}

[test]
def test_vkt_fa_bidir(t0 : T?) {
    t0 |> run("bidirectional flash attention (h64 and the padded h128, cm2 and KHR arms) == the CPU oracle over every key") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_clamp_cvt_cls()) {
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

//! the chunked attention's combine on the CPU: a row's head slice over `nch` chunk partials (max, exp-sum) and their unnormalized values
def private wdec_comb_ref(pms, py : array<float>; rows, heads, hs, nch : int) : array<float> {
    var y : array<float>
    y |> resize(rows * heads * hs)
    for (r in range(rows)) {
        for (h in range(heads)) {
            var mg = -3.0e38
            for (c in range(nch)) {
                mg = max(mg, pms[((c * heads + h) * rows + r) * 2])
            }
            var tot = 0.0
            for (c in range(nch)) {
                let slot = (c * heads + h) * rows + r
                tot += pms[slot * 2 + 1] * exp(pms[slot * 2] - mg)
            }
            for (j in range(hs)) {
                var acc = 0.0
                for (c in range(nch)) {
                    let slot = (c * heads + h) * rows + r
                    acc += py[slot * hs + j] * exp(pms[slot * 2] - mg)
                }
                y[r * heads * hs + h * hs + j] = acc / max(tot, 1.0e-30)
            }
        }
    }
    return <- y
}

//! the four fused row passes at one shape: `d` the row width (a 64-multiple), `rows` the batch, `nch` the combine's chunks - each
//! byte for byte and scale for scale against the pass then `TowerClampRq`, the bias arm and the combine against their CPU forms
def private wdec_fused_rq_shape(t : T?; d, rows, nch : int) {   // nolint:STYLE038 — one flat gate over the four fused classes: the fixture, one command buffer, the pairs, the oracles and their poisons
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_X = 1u
        let B_XB = 2u
        let B_Y = 4u
        let B_H = 8u
        let B_P = 16u
        let B_YC = 32u
        let B_Q = 64u
        let q8e = int(kq_elems(KqFmt.q8))
        let ff = 4 * d
        let heads = d / int(WDEC_HS)
        let nd = rows * d
        let nff = rows * ff
        let shape = "d {d}, {rows} rows, {nch} chunks"
        var x <- row_fill(nd, 29, 3, 0.03, 1.4)
        var y <- row_fill(nd, 17, 11, 0.05, 2.1)
        var hrows <- row_fill(nff, 13, 5, 0.06, 2.8)
        var wn <- row_fill(5 * d + ff, 7, 2, 0.02, 0.9)   // the layernorm's w at 0 and b at d, the post-add bias at 2d, the next norm's w at 3d and b at 4d, the activation's bias row at 5d
        var pms <- row_fill(nch * heads * rows * 2, 19, 1, 0.01, 0.3)
        var py <- row_fill(nch * heads * rows * int(WDEC_HS), 23, 9, 0.04, 1.9)
        with_cell_planes() $(var cp : CellPlanes) {   // the planes name their bits: the separate and fused passes share them, so the barriers between the passes stand
            let xp = cp |> in_(x, 0, 0, B_X)
            let x2p = cp |> io_(x, 0, 0, B_X)
            let x3p = cp |> io_(x, 0, 0, B_X)
            let xbp = cp |> out_(nd, 0, 0, B_XB)
            let xb2p = cp |> out_(nd, 0, 0, B_XB)
            let yp = cp |> in_(y, 0, 0, B_Y)
            let ycp = cp |> out_(nd, 0, 0, B_YC)
            let hp = cp |> io_(hrows, 0, 0, B_H)
            let h2p = cp |> in_(hrows, 0, 0, B_H)
            let wnp = cp |> in_(wn)
            let pmsp = cp |> in_(pms, 0, 0, B_P)
            let pyp = cp |> in_(py, 0, 0, B_P)
            let BIAS_PAIR = 2   // the pairs' Q8_0 planes: the separate pass at an even index, the fused stamp at the odd one after it; the bias arm's pair is ff-wide
            var qp, sp : int[8]
            for (i in range(8)) {
                let wide = i / 2 == BIAS_PAIR
                qp[i] = cp |> out_words(int64(wide ? nff : nd), I8_SENTINEL_BITS, B_Q)
                sp[i] = cp |> out_((wide ? nff : nd) / q8e, 0, 0, B_Q)
            }
            var s_ln = cp |> cls_set(fixed_array(xp, xbp, wnp), @@set_tower_ln_cls)
            var s_rq1 = cp |> cls_set(fixed_array(xbp, qp[0], sp[0]), @@set_tower_clamp_rq_cls)
            var s_lnrq = cp |> cls_set(fixed_array(xp, wnp, qp[1], sp[1]), @@set_tower_ln_rq_cls)
            var s_post = cp |> cls_set(fixed_array(x2p, yp, xb2p, wnp), @@set_tower_post_add_ln_cls)
            var s_rq2 = cp |> cls_set(fixed_array(xb2p, qp[2], sp[2]), @@set_tower_clamp_rq_cls)
            var s_postrq = cp |> cls_set(fixed_array(x3p, yp, wnp, qp[3], sp[3]), @@set_tower_post_add_ln_rq_cls)
            var s_bias = cp |> cls_set(fixed_array(hp, wnp), @@set_tower_bias_act_cls)
            var s_rq3 = cp |> cls_set(fixed_array(hp, qp[4], sp[4]), @@set_tower_clamp_rq_cls)
            var s_biasrq = cp |> cls_set(fixed_array(h2p, wnp, qp[5], sp[5]), @@set_tower_bias_act_rq_cls)
            var s_comb = cp |> cls_set(fixed_array(pmsp, pyp, ycp), @@set_tower_wdec_attn_comb_cls)
            var s_rq4 = cp |> cls_set(fixed_array(ycp, qp[6], sp[6]), @@set_tower_clamp_rq_cls)
            var s_combrq = cp |> cls_set(fixed_array(pmsp, pyp, qp[7], sp[7]), @@set_tower_wdec_attn_comb_rq_cls)
            var pa_ln = GkPostAddLnArgs(d = uint(d), rows = uint(rows), eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = uint(d), xboff = 0u)   // nolint:LINT003 - the encoders take the push block by value, and the lambda below captures it
            var pa_rq = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = -3.0e38, hi = 3.0e38)   // nolint:LINT003 - the same
            var pa_rqh = RqArgs(inbase = 0u, nblk = uint(nff / q8e), lo = -3.0e38, hi = 3.0e38)   // nolint:LINT003 - the same
            var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(4 * d), ascale = 1.0, xboff = 0u)   // nolint:LINT003 - the same
            var pa_bias = GkBiasActArgs(d = uint(ff), nelem = uint(nff), boff = uint(5 * d), act = BIAS_ACT_GELU_TANH)   // nolint:LINT003 - the same
            var pa_biasrq = TowerBiasRqArgs(d = uint(ff), nblk = uint(nff / q8e), boff = uint(5 * d))   // nolint:LINT003 - the same
            var pa_c = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(heads), nrows = uint(rows), hs = WDEC_HS)   // nolint:LINT003 - the same
            let wg4 = int64((nd / 4 + 255) / 256)
            let wg4h = int64((nff / 4 + 255) / 256)
            let wg_comb = int64(heads * rows)
            let encode <- @(raw : VkCommandBuffer; var h : VkHaz&) {
                enc_tower_ln_cls(raw, h, s_ln, pa_ln, int64(rows))
                enc_tower_clamp_rq_cls(raw, h, s_rq1, pa_rq, wg4)
                enc_tower_ln_rq_cls(raw, h, s_lnrq, pa_ln, int64(rows))
                enc_tower_post_add_ln_cls(raw, h, s_post, pa_post, int64(rows))
                enc_tower_clamp_rq_cls(raw, h, s_rq2, pa_rq, wg4)
                enc_tower_post_add_ln_rq_cls(raw, h, s_postrq, pa_post, int64(rows))
                enc_tower_bias_act_cls(raw, h, s_bias, pa_bias, wg4h)
                enc_tower_clamp_rq_cls(raw, h, s_rq3, pa_rqh, wg4h)
                enc_tower_bias_act_rq_cls(raw, h, s_biasrq, pa_biasrq, wg4h)
                enc_tower_wdec_attn_comb_cls(raw, h, s_comb, pa_c, wg_comb)
                enc_tower_clamp_rq_cls(raw, h, s_rq4, pa_rq, wg4)
                enc_tower_wdec_attn_comb_rq_cls(raw, h, s_combrq, pa_c, wg_comb)
            }
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                invoke(encode, raw, h)
            }
            var qs : array<array<int8>>
            var ss : array<array<float>>
            qs |> reserve(8)
            ss |> reserve(8)
            for (i in range(8)) {
                qs |> emplace(cp |> plane_as(qp[i], type<int8>, int(cp.bytes[qp[i]])))
                ss |> emplace(cp |> plane_run(sp[i]))
            }
            var x2 <- cp |> plane_run(x2p)
            var x3 <- cp |> plane_run(x3p)
            var h_dev <- cp |> plane_run(hp)
            var yc_dev <- cp |> plane_run(ycp)
            let names <- ["layernorm", "post-add + next norm", "bias + tanh GELU", "attention combine"]
            for (i in range(4)) {
                let off = mismatch_q8(qs[2 * i + 1], ss[2 * i + 1], qs[2 * i], ss[2 * i], 0.0, 0)
                t |> equal(off, 0, "{names[i]} ({shape}) fused with the requant lands the separate pass's Q8_0 bytes and scales ({off} differ)")
                t |> equal(count(qs[2 * i + 1], $(v) => int(v) == -128), 0, "{names[i]} ({shape}): every block written (no sentinel bytes left)")
            }
            t |> equal(mismatch_exact(x3, x2), 0, "the fused post-add ({shape}) updates the residual rows in place as the separate pass does")
            t |> success(mismatch_exact(x2, x) > 0, "the post-add ({shape}) moved the residual rows off their input")
            var h_ref := hrows
            add_bias_rows(h_ref, wn, int64(5 * d), int64(ff), int64(rows))
            gelu_tanh_lut_batch(h_ref, int64(ff), int64(rows))
            check_rows(t, "the bias + tanh GELU ({shape}) vs add_bias_rows + gelu_tanh_lut_batch", h_dev, h_ref, BAR_APPROX, ff)
            var yc_ref <- wdec_comb_ref(pms, py, rows, heads, int(WDEC_HS), nch)
            check_rows(t, "the attention combine ({shape}) vs the CPU combine over the chunk partials", yc_dev, yc_ref, BAR_APPROX, d, int(WDEC_HS))
            x[7]++
            y[7 + d]++
            hrows[3]++
            py[5]++
            cp |> plane_upload(xp, x)
            cp |> plane_upload(x3p, x)
            cp |> plane_upload(yp, y)
            cp |> plane_upload(h2p, hrows)
            cp |> plane_upload(pyp, py)
            for (i in range(8)) {
                if (i % 2 == 1) {
                    cp |> plane_refill(qp[i])
                    cp |> plane_refill(sp[i])
                }
            }
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                invoke(encode, raw, h)
            }
            for (i in range(8)) {
                if (i % 2 == 1) {
                    var inscope qr <- cp |> plane_as(qp[i], type<int8>, int(cp.bytes[qp[i]]))
                    var inscope sr <- cp |> plane_run(sp[i])
                    let moved = mismatch_q8(qr, sr, qs[i], ss[i], 0.0, 0)
                    t |> success(moved > 0, "{names[i / 2]} ({shape}): the poisoned input moves the fused bytes ({moved} differ)")
                    t |> equal(count(qr, $(v) => int(v) == -128), 0, "{names[i / 2]} ({shape}): the poisoned run rewrote every block over the sentinel")
                    delete qr
                    delete sr
                }
            }
            delete qs
            delete ss
            delete x2
            delete x3
            delete h_dev
            delete yc_dev
            delete h_ref
            delete yc_ref
        }
        delete x
        delete y
        delete hrows
        delete wn
        delete pms
        delete py
    }
}

[test]
def test_vkt_wdec_fused_rq(t0 : T?) {
    t0 |> run("the layernorm, the post-add, the bias-activation and the attention combine fused with the Q8_0 requant == the pass then TowerClampRq, at the other cells' width, whisper tiny's and turbo's") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_ln_rq_cls() || !ensure_tower_post_add_ln_rq_cls() || !ensure_tower_bias_act_rq_cls() || !ensure_tower_wdec_attn_comb_rq_cls()
                || !ensure_tower_ln_cls() || !ensure_tower_post_add_ln_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_wdec_attn_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            wdec_fused_rq_shape(t, ROWS_D, ROWS_N, 2)   // the row classes' width; two chunks
            wdec_fused_rq_shape(t, 384, ROWS_N, 1)      // whisper tiny: the strided loops' partial tail; one chunk
            wdec_fused_rq_shape(t, 1280, ROWS_N, 1)     // large-v3-turbo: the block pass's second trip (d > 1024)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkt_tile_rows(t0 : T?) {
    t0 |> run("a GEMM record's rows round up to the l column; the s and m columns and the batch tile keep the raw count") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> equal(vt_tile_rows(VtTile(v = 0, tc = 256l), 1500l), 1536l, "whisper's 1500-row chunk on the l column")
            t |> equal(vt_tile_rows(VtTile(v = 0, tc = 256l), 1536l), 1536l, "a whole column count stays")
            t |> equal(vt_tile_rows(VtTile(v = 0, tc = 256l), 1152l), 1280l, "four and a half l columns round to five")
            t |> equal(vt_tile_rows(VtTile(v = 0, tc = 128l), 1500l), 1500l, "the m column keeps the raw count (its partial column takes the fast path)")
            t |> equal(vt_tile_rows(VtTile(v = 0, tc = 32l), 13l), 13l, "the s column keeps the raw count (gemma4a's rel record over its 13 rows)")
            t |> equal(vt_tile_rows(VtTile(v = 3, tc = 0l), 1500l), 1500l, "the q8 batch tile carries the raw count")
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


[test]
def test_vkt_tower_rows(t0 : T?) {   // nolint:STYLE038 — one flat gate over the eight row classes: the fixture, one command buffer, eight oracles and their controls
    t0 |> run("the tower row classes (rms, clamp, clamp copy, clamp+cvt, clamp+rq, rope2d, post-add, post-add plain, geglu) == the CPU tower helpers") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_rms_cls() || !ensure_tower_clamp_cls() || !ensure_tower_clamp_cvt_cls() || !ensure_tower_clamp_copy_cls()
                    || !ensure_tower_clamp_rq_cls() || !ensure_tower_rope2d_cls() || !ensure_tower_post_add_cls()
                    || !ensure_tower_post_add_plain_cls() || !ensure_tower_geglu_cls()) {
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
            let q8e = int(kq_elems(KqFmt.q8))
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var q <- row_fill(nd, 29, 11, 0.06, 2.8)
            let g <- row_fill(nff, 13, 5, 0.09, 4.0)
            let u <- row_fill(nff, 23, 9, 0.07, 3.3)
            var wn <- row_fill(2 * d, 7, 1, 0.01, -0.6)   // two weight rows: the pre-norm's at 0, the post-norm's at d - both near 1
            let tabs <- row_fill(rows * 4 * pairs, 31, 2, 0.02, 0.97)   // per row [cx | sx | cy | sy]: any values serve the oracle, which applies the same tables
            let lo = -1.25
            let hi = 1.5
            let lo2 = -0.75
            let hi2 = 0.9
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x)
                let yp = cp |> io_(y)
                let cc_src_p = cp |> io_(y)    //! the clamp copy's source, which it must leave as it found it
                let cc_dst_p = cp |> out_(nd)   // ... and its destination
                let xbp = cp |> out_(nd)
                let xb2p = cp |> out_(nd)
                let qp = cp |> io_(q)
                let gp = cp |> io_(g)
                let up = cp |> in_(u)
                let wp = cp |> in_(wn)
                let tp = cp |> in_(tabs)
                let xhp = cp |> out_words(int64(nd) * 2l, F16_SENTINEL_BITS)
                let xqp = cp |> out_words(int64(nd), I8_SENTINEL_BITS)
                let xsp = cp |> out_(nd / q8e)
                let x0p = cp |> io_(x)    // the ln_on = 0 arm's x: the same seam with no next pre-norm
                let xb0p = cp |> out_(nd)   // ... and its pre-norm output, which must stay untouched
                let x_plain_p = cp |> io_(x)    // the plain stamp's x: the branch rows added at half weight with no post-norm
                let xb_plain_p = cp |> out_(nd)   // ... and its next pre-norm
                let x_plain0_p = cp |> io_(x)    // the plain stamp at ln_on = 0: the seam alone
                let xb_plain0_p = cp |> out_(nd)   // ... and its pre-norm output, which must stay untouched
                let x_half_p = cp |> io_(x)      // the post-norm stamp at ascale = 0.5, ln_on = 0
                let xb_half_p = cp |> out_(nd)
                var s_rms = cp |> cls_set(fixed_array(xp, xbp, wp), @@set_tower_rms_cls)
                var s_clamp = cp |> cls_set(fixed_array(yp), @@set_tower_clamp_cls)
                var s_ccopy = cp |> cls_set(fixed_array(cc_src_p, cc_dst_p), @@set_tower_clamp_copy_cls)
                var s_cvt = cp |> cls_set(fixed_array(xp, xhp), @@set_tower_clamp_cvt_cls)
                var s_rq = cp |> cls_set(fixed_array(xp, xqp, xsp), @@set_tower_clamp_rq_cls)
                var s_rope = cp |> cls_set(fixed_array(qp, tp), @@set_tower_rope2d_cls)
                var s_post = cp |> cls_set(fixed_array(xp, yp, xb2p, wp), @@set_tower_post_add_cls)
                var s_post0 = cp |> cls_set(fixed_array(x0p, yp, xb0p, wp), @@set_tower_post_add_cls)
                var s_post_plain = cp |> cls_set(fixed_array(x_plain_p, yp, xb_plain_p, wp), @@set_tower_post_add_plain_cls)
                var s_post_plain0 = cp |> cls_set(fixed_array(x_plain0_p, yp, xb_plain0_p, wp), @@set_tower_post_add_plain_cls)
                var s_post_half = cp |> cls_set(fixed_array(x_half_p, yp, xb_half_p, wp), @@set_tower_post_add_cls)
                var s_geglu = cp |> cls_set(fixed_array(gp, up), @@set_tower_geglu_cls)
                var pa_rms = gk_rms_args(uint(d), uint(rows), ROWS_EPS, 0u)
                var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = uint(d), boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
                var pa_post0 = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = uint(d), boff = 0u, eps = ROWS_EPS, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 1.0, xboff = 0u)
                var pa_post_plain = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = 0u, ascale = 0.5, xboff = 0u)
                var pa_post_plain0 = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = 0u, boff = 0u, eps = ROWS_EPS, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 0.5, xboff = 0u)
                var pa_post_half = GkPostAddLnArgs(d = uint(d), rows = uint(rows), woff = uint(d), boff = 0u, eps = ROWS_EPS, ln_on = 0u, lnoff = 0u, lnboff = 0u, ascale = 0.5, xboff = 0u)
                var pa_clamp = GkClampArgs(nelem = uint(nd), lo = lo, hi = hi, scale = 1.0)
                var pa_cvt = GkClampArgs(nelem = uint(nd), lo = lo2, hi = hi2, scale = 1.0)
                var pa_rq = RqArgs(inbase = 0u, nblk = uint(nd / q8e), lo = lo2, hi = hi2)
                var pa_rope = GkRope2dArgs(nheads = uint(ROWS_HEADS), hs = uint(hs), rows = uint(rows))
                var pa_geglu = GkGegluArgs(nelem = uint(nff), glo = -2.0, ghi = 2.5, ulo = -1.5, uhi = 1.75, dlo = -1.0, dhi = 1.2)
                let wg4 = int64((nd / 4 + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_rms_cls(raw, h, s_rms, pa_rms, int64(rows))
                    enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, wg4)
                    enc_tower_clamp_rq_cls(raw, h, s_rq, pa_rq, int64((nd / 4 + 255) / 256))
                    enc_tower_rope2d_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * pairs + 255) / 256))
                    enc_tower_geglu_cls(raw, h, s_geglu, pa_geglu, int64((nff / 4 + 255) / 256))
                    enc_tower_clamp_cls(raw, h, s_clamp, pa_clamp, wg4)
                    enc_tower_clamp_copy_cls(raw, h, s_ccopy, pa_clamp, wg4)
                    enc_tower_post_add_cls(raw, h, s_post0, pa_post0, int64(rows))
                    enc_tower_post_add_plain_cls(raw, h, s_post_plain, pa_post_plain, int64(rows))
                    enc_tower_post_add_plain_cls(raw, h, s_post_plain0, pa_post_plain0, int64(rows))
                    enc_tower_post_add_cls(raw, h, s_post_half, pa_post_half, int64(rows))
                    enc_tower_post_add_cls(raw, h, s_post, pa_post, int64(rows))   // last: it rewrites x, which the rms and the feeds read whole
                }
                let xb_dev <- cp |> plane_run(xbp)
                let y_dev <- cp |> plane_run(yp)
                let ys_dev <- cp |> plane_run(cc_src_p)
                let yc_dev <- cp |> plane_run(cc_dst_p)
                let x_dev <- cp |> plane_run(xp)
                let xb2_dev <- cp |> plane_run(xb2p)
                let g_dev <- cp |> plane_run(gp)
                let xh_dev <- cp |> plane_as(xhp, type<uint16>, nd)
                let xq_dev <- cp |> plane_as(xqp, type<int8>, nd)
                let xs_dev <- cp |> plane_run(xsp)
                let x0_dev <- cp |> plane_run(x0p)
                let xb0_dev <- cp |> plane_as(xb0p, type<uint>, nd)
                let xp_dev <- cp |> plane_run(x_plain_p)
                let xbp_dev <- cp |> plane_run(xb_plain_p)
                let xp0_dev <- cp |> plane_run(x_plain0_p)
                let xbp0_dev <- cp |> plane_as(xb_plain0_p, type<uint>, nd)
                let xhalf_dev <- cp |> plane_run(x_half_p)
                let q_dev <- cp |> plane_run(qp)
                let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(2 * d))
                var xb_ref : array<float>
                xb_ref |> resize(nd)
                rms_rows(xb_ref, x, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_rms weighted rms rows vs rms_rows", xb_dev, xb_ref, BAR_APPROX, d)
                var y_ref : array<float>
                y_ref |> resize(nd)
                clamp_rows(y_ref, y, int64(nd), lo, hi)
                check_rows(t, "tower_clamp rows clamped in place vs clamp_rows", y_dev, y_ref, BAR_EXACT, d)
                check_rows(t, "tower_clamp_copy rows clamped into the copy vs clamp_rows", yc_dev, y_ref, BAR_EXACT, d)
                check_rows(t, "tower_clamp_copy leaves its source as it found it", ys_dev, y, BAR_EXACT, d)
                var clamped_x : array<float>
                clamped_x |> resize(nd)
                clamp_rows(clamped_x, x, int64(nd), lo2, hi2)
                check_halves(t, "tower_clamp_cvt: the f16 feed is the clamped rows rounded to f16", xh_dev, clamped_x)
                var xq_ref : array<int8>
                var xs_ref : array<float>
                requant_rows_q8_sized(clamped_x, int64(d), int64(rows), xq_ref, xs_ref)
                let bad_rq = mismatch_q8(xq_dev, xs_dev, xq_ref, xs_ref, APPROX_REL, 1)   // the device rounds a half-step value apart: one quant step, the scales at the approx bar
                t |> success(bad_rq == 0, "tower_clamp_rq: the Q8_0 feed is the clamped rows requantized as the CPU does ({bad_rq} off)")
                var xs_poison := xs_ref
                xs_poison[0] += 0.5
                t |> success(mismatch_q8(xq_dev, xs_dev, xq_ref, xs_poison, APPROX_REL, 1) > 0, "tower_clamp_rq: the bar reds a poisoned scale")
                var q_ref := q
                var cos_x, sin_x, cos_y, sin_y : array<float>
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
                check_rows(t, "tower_rope2d 2D neox rope vs rope_neox_2d_rows", q_dev, q_ref, BAR_APPROX, d, hs)
                var x_ref := x
                var ypost : array<float>
                ypost |> resize(nd)
                rms_rows(ypost, y_ref, wplane, int64(d), int64(d), int64(rows), ROWS_EPS)   // the post-add reads y after the clamp the same command applied to it
                add_inplace_rows(x_ref, ypost, int64(nd))
                check_rows(t, "tower_post_add x += rms(y) . wpost", x_dev, x_ref, BAR_APPROX, d)
                var xb2_ref : array<float>
                xb2_ref |> resize(nd)
                rms_rows(xb2_ref, x_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_post_add the next pre-norm over the updated rows", xb2_dev, xb2_ref, BAR_APPROX, d)
                check_rows(t, "tower_post_add ln_on = 0 the seam alone lands the same x", x0_dev, x_ref, BAR_APPROX, d)
                t |> equal(count(xb0_dev, $(w) => w == NAN_SENTINEL_BITS), nd, "tower_post_add ln_on = 0: the pre-norm output stays untouched (every word the sentinel)")
                var xhalf_ref := x
                for (i in range(nd)) {
                    xhalf_ref[i] += 0.5 * ypost[i]
                }
                check_rows(t, "tower_post_add ascale = 0.5 x += 0.5 . rms(y) . wpost", xhalf_dev, xhalf_ref, BAR_APPROX, d)
                var xp_ref := x
                for (i in range(nd)) {
                    xp_ref[i] += 0.5 * y_ref[i]
                }
                check_rows(t, "tower_post_add_plain x += 0.5 . y with no post-norm", xp_dev, xp_ref, BAR_APPROX, d)
                t |> success(mismatch_approx(xp_dev, x_ref) > nd / 4, "tower_post_add_plain: the plain stamp is not the post-norm stamp ({mismatch_approx(xp_dev, x_ref)} of {nd} apart)")
                check_rows(t, "tower_post_add_plain ln_on = 0 the seam alone lands the same x", xp0_dev, xp_ref, BAR_APPROX, d)
                t |> equal(count(xbp0_dev, $(w) => w == NAN_SENTINEL_BITS), nd, "tower_post_add_plain ln_on = 0: the pre-norm output stays untouched (every word the sentinel)")
                var xbp_ref : array<float>
                xbp_ref |> resize(nd)
                rms_rows(xbp_ref, xp_ref, wplane, 0l, int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_post_add_plain the next pre-norm over the half-weight rows", xbp_dev, xbp_ref, BAR_APPROX, d)
                var g_ref : array<float>
                g_ref |> resize(nff)
                for (i in range(nff)) {
                    let gv = clamp(g[i], -2.0, 2.5)
                    let uv = clamp(u[i], -1.5, 1.75)
                    g_ref[i] = clamp(gelu_quick_lut_f32(gv) * uv, -1.0, 1.2)
                }
                check_rows(t, "tower_geglu clamped GEGLU-quick vs the CPU table form", g_dev, g_ref, BAR_APPROX, ff)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private VR_W = 96              //! the vision rows' canvas: 6 x 4 patches of 16 - an even grid for the merge-adjacent walk, 24 rows a 4-multiple
let private VR_H = 64
let private VR_P = 16
let private VR_PW = VR_W / VR_P
let private VR_PH = VR_H / VR_P
let private VR_NPOS = VR_PW * VR_PH
let private VR_PD = 3 * VR_P * VR_P   //! the im2col row: 768, the 64 lattice
let private VR_POS_ENTRIES = 8       //! the 2-axis table's rows an axis: past the grid's 6 columns and 4 rows
let private VR_POOL_GW = 6           //! the grid pool: a 6 x 3 row grid pooled 3 x 3 into 2 x 1
let private VR_POOL_GH = 3
let private VR_POOL_K = 3

//! the vision chains' stem and tail stamps against the CPU forms: the patch im2col under the [0, 1] -> [-1, 1] map (the pad
//! rows zero), the gemma4v 2-axis position add, the qwen3v merge-walk stem assemble, the grid mean pool and the standardize
//! rows on both arms; every compare with its poisoned-expectation control
[test]
def test_vkt_tower_vision_rows(t0 : T?) {   // nolint:STYLE038 — one flat gate over the five vision row classes: the fixture, one command buffer, the oracles
    t0 |> run("the vision stem and tail rows (patch im2col, pos add 2d, stem assemble, grid pool, affine rows) == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_patch_im2col_cls() || !ensure_tower_pos_add_2d_cls() || !ensure_tower_stem_asm_cls()
                    || !ensure_tower_pool_2d_cls() || !ensure_tower_affine_rows_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let npos = VR_NPOS
            let nd = npos * d
            let npos_pad = npos + 2   //! two pad rows past the grid, which the im2col writes zero
            let pool_rows = VR_POOL_GW * VR_POOL_GH
            let nout = (VR_POOL_GW / VR_POOL_K) * (VR_POOL_GH / VR_POOL_K)
            let planes <- row_fill(VR_W * VR_H * 3, 29, 11, 0.004, 0.37, 251)   //! pixels off any patch symmetry: every patch its own
            let x <- row_fill(nd, 41, 7, 0.05, 2.1)
            let tbl <- row_fill(2 * VR_POS_ENTRIES * d, 13, 5, 0.03, -0.9)
            let xn <- row_fill(nd, 17, 3, 0.04, 1.9)
            let pos <- row_fill(nd, 23, 9, 0.02, 0.5)
            let bias <- row_fill(2 * d, 7, 1, 0.01, -0.6)   //! the stem bias at 0, the standardize bias at d
            let std_scale <- row_fill(d, 31, 2, 0.02, 0.97)      // the standardize scale
            let gx <- row_fill(pool_rows * d, 37, 5, 0.043, 2.1)
            let ax <- row_fill(nout * d, 19, 7, 0.06, 2.8)
            let scale = sqrt(float(d))
            with_cell_planes() $(var cp : CellPlanes) {
                let plp = cp |> in_(planes)
                let colp = cp |> out_(npos_pad * VR_PD)
                let xp = cp |> io_(x)
                let tp = cp |> in_(tbl)
                let xnp = cp |> in_(xn)
                let bp = cp |> in_(bias)
                let pp = cp |> in_(pos)
                let asmp = cp |> out_(nd)
                let gxp = cp |> in_(gx)
                let poolp = cp |> out_(nout * d)
                let mp = cp |> in_(std_scale)
                let ax_std_p = cp |> io_(ax)      // the standardize arm
                let ax_scale_p = cp |> io_(ax)     // the scale-only arm
                var s_im = cp |> cls_set(fixed_array(plp, colp), @@set_tower_patch_im2col_cls)
                var s_pos = cp |> cls_set(fixed_array(xp, tp), @@set_tower_pos_add_2d_cls)
                var s_asm = cp |> cls_set(fixed_array(xnp, bp, pp, asmp), @@set_tower_stem_asm_cls)
                var s_pool = cp |> cls_set(fixed_array(gxp, poolp), @@set_tower_pool_2d_cls)
                var s_aff = cp |> cls_set(fixed_array(ax_std_p, bp, mp), @@set_tower_affine_rows_cls)
                var s_aff0 = cp |> cls_set(fixed_array(ax_scale_p, bp, mp), @@set_tower_affine_rows_cls)
                var pa_im = GkPatchIm2colArgs(w = uint(VR_W), h = uint(VR_H), p = uint(VR_P), row_dim = uint(VR_PD), npos = uint(npos),
                    total = uint(npos_pad * VR_PD), scale = 2.0, shift = -1.0)
                var pa_pos = GkPosAdd2DArgs(dim = uint(d), ncols = uint(VR_PW), pos_entries = uint(VR_POS_ENTRIES), total = uint(nd), tboff = 0u)
                var pa_asm = GkStemAsmArgs(dim = uint(d), pw = uint(VR_PW), total = uint(nd), boff = 0u, poff = 0u)
                var pa_pool = GkPool2dArgs(width = uint(d), gw = uint(VR_POOL_GW), k = uint(VR_POOL_K), total = uint(nout * d))
                var pa_aff = GkAffineRowsArgs(width = uint(d), total = uint(nout * d), scale = scale, boff = uint(d), moff = 0u, std = 1u)
                var pa_aff0 = GkAffineRowsArgs(width = uint(d), total = uint(nout * d), scale = scale, boff = 0u, moff = 0u, std = 0u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_patch_im2col_cls(raw, h, s_im, pa_im, int64((npos_pad * VR_PD + 255) / 256))
                    enc_tower_pos_add_2d_cls(raw, h, s_pos, pa_pos, int64((nd + 255) / 256))
                    enc_tower_stem_asm_cls(raw, h, s_asm, pa_asm, int64((nd + 255) / 256))
                    enc_tower_pool_2d_cls(raw, h, s_pool, pa_pool, int64((nout * d + 255) / 256))
                    enc_tower_affine_rows_cls(raw, h, s_aff, pa_aff, int64((nout * d + 255) / 256))
                    enc_tower_affine_rows_cls(raw, h, s_aff0, pa_aff0, int64((nout * d + 255) / 256))
                }
                let col_dev <- cp |> plane_run(colp)
                let x_dev <- cp |> plane_run(xp)
                let asm_dev <- cp |> plane_run(asmp)
                let pool_dev <- cp |> plane_run(poolp)
                let ax_dev <- cp |> plane_run(ax_std_p)
                let ax0_dev <- cp |> plane_run(ax_scale_p)
                var col_cpu : array<float>
                let n_cpu = im2col_rgb_patches(planes, int64(VR_W), int64(VR_H), int64(VR_P), col_cpu)
                t |> equal(n_cpu, int64(npos), "the CPU im2col's patch count is the grid")
                var col_want : array<float>
                col_want |> resize(npos_pad * VR_PD)
                for (i in range(npos * VR_PD)) {
                    col_want[i] = col_cpu[i] * 2.0 - 1.0
                }
                check_rows(t, "tower_patch_im2col the patch columns under the [0, 1] -> [-1, 1] map vs im2col_rgb_patches, the pad rows zero", col_dev, col_want, BAR_EXACT, VR_PD)
                var x_want : array<float>
                x_want |> resize(nd)
                for (pi in range(npos)) {
                    for (i in range(d)) {
                        let ex = tbl[(pi % VR_PW) * d + i]
                        let ey = tbl[(VR_POS_ENTRIES + pi / VR_PW) * d + i]
                        x_want[pi * d + i] = (x[pi * d + i] + ex) + ey
                    }
                }
                check_rows(t, "tower_pos_add_2d (row + x-table row) + y-table row vs the CPU form", x_dev, x_want, BAR_EXACT, d)
                t |> success(x_want[0] != x[0], "tower_pos_add_2d: the rows moved off x")
                var asm_want : array<float>
                asm_want |> resize(nd)
                for (r in range(npos)) {
                    let g = r / 4   //! the merge-adjacent walk the CPU stem reads (`q3v_reorder_src`): 2 x 2 partners adjacent, groups row-major
                    let sub = r % 4
                    let src = ((g / (VR_PW / 2)) * 2 + sub / 2) * VR_PW + (g % (VR_PW / 2)) * 2 + sub % 2
                    for (i in range(d)) {
                        asm_want[r * d + i] = (xn[src * d + i] + bias[i]) + pos[src * d + i]
                    }
                }
                check_rows(t, "tower_stem_asm the merge-walk gather (conv row + bias) + position row vs the CPU walk", asm_dev, asm_want, BAR_EXACT, d)
                var pool_want : array<float>
                avg_pool2d_rows(pool_want, gx, int64(VR_POOL_GW), int64(VR_POOL_GH), int64(d), int64(VR_POOL_K))
                check_rows(t, "tower_pool_2d the k x k mean over the row grid vs avg_pool2d_rows", pool_dev, pool_want, BAR_APPROX, d)
                var aff_want : array<float>
                aff_want |> resize(nout * d)
                var aff0_want : array<float>
                aff0_want |> resize(nout * d)
                for (i in range(nout * d)) {
                    let c = i % d
                    aff0_want[i] = ax[i] * scale
                    aff_want[i] = (ax[i] * scale - bias[d + c]) * std_scale[c]
                }
                check_rows(t, "tower_affine_rows (x . scale - b) . m vs the CPU form", ax_dev, aff_want, BAR_APPROX, d)
                t |> success(aff_want[0] != ax[0] && aff0_want[0] != ax[0], "tower_affine_rows: both arms moved off x")
                check_rows(t, "tower_affine_rows x . scale on the scale-only arm", ax0_dev, aff0_want, BAR_APPROX, d)
                delete col_cpu
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the chat projector tails' row classes on the templates both homes stamp: the pair pool against the mean of each row pair, the
//! swapped gate against the CPU `swiglu_swapped_rows` - the fixture's halves differ, so the gate with its halves swapped misses on
//! most elements - at a width off every workgroup multiple
[test]
def test_vkt_tower_tail_rows(t0 : T?) {
    t0 |> run("the pair pool and the swapped SwiGLU rows == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_pool_rows_cls() || !ensure_tower_swiglu_swapped_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let width = 70
            let rows = 5
            let total = rows * width
            let x <- row_fill(2 * total, 37, 5, 0.043, 2.1)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(x)
                let pp = cp |> out_(total)
                let gp = cp |> out_(total)
                var s_pool = cp |> cls_set(fixed_array(xp, pp), @@set_tower_pool_rows_cls)
                var s_gate = cp |> cls_set(fixed_array(xp, gp), @@set_tower_swiglu_swapped_cls)
                var pa_pool = GkPoolRowsArgs(width = uint(width), total = uint(total))
                var pa_gate = GkGatedRowsArgs(d = uint(width), nelem = uint(total))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_pool_rows_cls(raw, h, s_pool, pa_pool, int64((total + 255) / 256))
                    enc_tower_swiglu_swapped_cls(raw, h, s_gate, pa_gate, int64((total + 255) / 256))
                }
                let p_dev <- cp |> plane_run(pp)
                let g_dev <- cp |> plane_run(gp)
                var p_ref : array<float>
                p_ref |> resize(total)
                for (i in range(total)) {
                    let at = (i / width) * 2 * width + i % width
                    p_ref[i] = 0.5 * (x[at] + x[at + width])
                }
                check_rows(t, "tower_pool_rows the mean of each row pair", p_dev, p_ref, BAR_APPROX, width)
                var g_ref : array<float>
                g_ref |> resize(total)
                swiglu_swapped_rows(g_ref, x, int64(width), int64(rows))
                check_rows(t, "tower_swiglu_swapped silu(second half) . first half vs swiglu_swapped_rows", g_dev, g_ref, BAR_APPROX, width)
                var swapped : array<float>
                swapped |> resize(total)
                for (i in range(total)) {
                    let at = (i / width) * 2 * width + i % width
                    swapped[i] = silu_f32(x[at]) * x[at + width]
                }
                let moved = mismatch_approx(g_dev, swapped)
                t |> success(moved > total / 2, "tower_swiglu_swapped control: the gate with its halves swapped lands elsewhere ({moved} of {total} moved)")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private G4A_PAST = int(gemma4a_attn_past())   //! the RPE table carries past + 1 rows
let private G4A_CAP = gemma4a_softcap()

[test]
def test_vkt_tower_conformer(t0 : T?) {   // nolint:STYLE038 — one flat gate over the four classes: the fixture, one command buffer, the oracles and their controls
    t0 |> run("the Conformer row classes (silu, GLU, depthwise conv, windowed RPE attention) == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_act_cls() || !ensure_tower_glu_sig_cls() || !ensure_tower_dw_conv5_cls()
                    || !ensure_tower_g4a_attn64_cls() || !ensure_tower_g4a_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let rows = ROWS_N
            let hs = d / ROWS_HEADS
            let nd = rows * d
            var s <- row_fill(nd, 23, 5, 0.09, 6.0)   // silu's rows: both tails
            let glu_pw_rows <- row_fill(rows * 2 * d, 31, 7, 0.05, 2.5)
            let q <- row_fill(nd, 41, 3, 0.03, 1.1)
            let k <- row_fill(nd, 17, 11, 0.04, 1.3)
            let v <- row_fill(nd, 29, 13, 0.05, 1.7)
            let rpe <- row_fill((G4A_PAST + 1) * d, 13, 2, 0.02, 0.9)
            let wn <- row_fill(hs + d * 5, 7, 1, 0.01, 0.4)
            with_cell_planes() $(var cp : CellPlanes) {
                let sp = cp |> io_(s)
                let hp = cp |> in_(glu_pw_rows)
                let gp = cp |> out_(nd)
                let cpl = cp |> out_(nd)
                let qp = cp |> in_(q)
                let kp = cp |> in_(k)
                let vp = cp |> in_(v)
                let aop = cp |> out_(nd)
                let rp = cp |> in_(rpe)
                let wp = cp |> in_(wn)
                var s_silu = cp |> cls_set(fixed_array(sp), @@set_tower_act_cls)
                var s_glu = cp |> cls_set(fixed_array(hp, gp), @@set_tower_glu_sig_cls)
                var s_dw = cp |> cls_set(fixed_array(gp, cpl, wp), @@set_tower_dw_conv5_cls)
                var s_attn = cp |> cls_set(fixed_array(qp, kp, vp, aop, rp, wp), @@set_tower_g4a_attn64_cls)
                var pa_silu = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
                var pa_convmod = GkGatedRowsArgs(d = uint(d), nelem = uint(nd))
                var pa_dw = GkDwConvRowsArgs(c = uint(d), t_in = uint(rows), t_out = uint(rows), woff = uint(hs))
                let sc = gemma4a_attn_scales(int64(hs))
                let qscale = sc.qscale
                let kscale = sc.kscale
                var pa_attn = GkG4aAttnArgs(d = uint(d), npos = uint(rows), hs = uint(hs), pdsoff = 0u, cap = GK_G4A_ATTN_CAP, qscale = qscale, kscale = kscale)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_act_cls(raw, h, s_silu, pa_silu, int64((nd / 4 + 255) / 256))
                    enc_tower_glu_sig_cls(raw, h, s_glu, pa_convmod, int64((nd + 255) / 256))
                    enc_tower_dw_conv5_cls(raw, h, s_dw, pa_dw, int64((nd + 255) / 256))
                    enc_tower_g4a_attn64_cls(raw, h, s_attn, pa_attn, int64((rows * ROWS_HEADS + 63) / 64))
                }
                let s_dev <- cp |> plane_run(sp)
                let g_dev <- cp |> plane_run(gp)
                let c_dev <- cp |> plane_run(cpl)
                let ao_dev <- cp |> plane_run(aop)
                var s_ref := s
                silu(s_ref, int64(nd))
                check_rows(t, "tower_act (the unbiased silu stamp) vs silu", s_dev, s_ref, BAR_APPROX, d)
                var g_ref : array<float>
                g_ref |> resize(nd)
                for (r in range(rows)) {
                    for (c in range(d)) {
                        let gate = glu_pw_rows[r * 2 * d + d + c]
                        g_ref[r * d + c] = glu_pw_rows[r * 2 * d + c] * sigmoid_f32(gate)
                    }
                }
                check_rows(t, "tower_glu_sig value . sigmoid(gate) off the pointwise rows", g_dev, g_ref, BAR_APPROX, d)
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
                check_rows(t, "tower_dw_conv5 causal depthwise taps vs the CPU loop", c_dev, c_ref, BAR_APPROX, d)
                let ao_ref <- g4a_attn_oracle(q, k, v, rpe, wn, rows, d, ROWS_HEADS, hs, G4A_PAST, qscale, kscale, G4A_CAP)
                check_rows(t, "tower_g4a_attn (64-wide stamp) windowed RPE attention vs the CPU loop", ao_dev, ao_ref, BAR_APPROX, d, hs)
                let k_scaled <- scaled_copy(k, 1.5)
                let ao_ctl <- g4a_attn_oracle(q, k_scaled, v, rpe, wn, rows, d, ROWS_HEADS, hs, G4A_PAST, qscale, kscale, G4A_CAP)
                let moved = mismatch_approx(ao_dev, ao_ctl)
                t |> success(moved > nd / 4, "tower_g4a_attn control: scaled keys land the oracle elsewhere ({moved} of {nd} moved)")
                // the 128-wide stamp (the served carriers' head) over the same rows read as two heads, the queries scaled up so
                // the scores reach the softcap's knee: the oracle at the cap, and at four times the cap as the control
                let heads2 = ROWS_HEADS / 2
                let hs2 = d / heads2
                var q_at_cap_knee <- scaled_copy(q, 30.0)
                cp |> plane_upload(qp, q_at_cap_knee)
                cp |> plane_refill(aop)
                var s_attn128 = cp |> cls_set(fixed_array(qp, kp, vp, aop, rp, wp), @@set_tower_g4a_attn_cls)
                let qscale2 = gemma4a_attn_scales(int64(hs2)).qscale
                var pa_attn128 = GkG4aAttnArgs(d = uint(d), npos = uint(rows), hs = uint(hs2), pdsoff = 0u, cap = GK_G4A_ATTN_CAP, qscale = qscale2, kscale = kscale)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_g4a_attn_cls(raw, h, s_attn128, pa_attn128, int64((rows * heads2 + 63) / 64))
                }
                let ao_knee_dev <- cp |> plane_run(aop)
                let ao_capped_ref <- g4a_attn_oracle(q_at_cap_knee, k, v, rpe, wn, rows, d, heads2, hs2, G4A_PAST, qscale2, kscale, G4A_CAP)
                check_rows(t, "tower_g4a_attn (128-wide stamp) windowed RPE attention over scaled queries vs the CPU loop", ao_knee_dev, ao_capped_ref, BAR_APPROX, d, hs2)
                let ao_cap4_ref <- g4a_attn_oracle(q_at_cap_knee, k, v, rpe, wn, rows, d, heads2, hs2, G4A_PAST, qscale2, kscale, 4.0 * G4A_CAP)
                let moved_cap4 = mismatch_approx(ao_knee_dev, ao_cap4_ref)
                t |> success(moved_cap4 > nd / 4, "tower_g4a_attn 128 control: the softcap engages on the scaled queries - the oracle at four times the cap lands elsewhere ({moved_cap4} of {nd} moved)")
            }
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
    let scale = 1.0 / sqrt(float(hs))
    for (hh in range(heads)) {
        let hoff = hh * hs
        for (i in range(rows)) {
            let base = rows - 1 - i
            attn_row_scored(v, hoff, d, hs, 0, rows, ao, i * d + hoff, false, 0.0) $(j : int) : float {
                var cs = 0.0
                var rs = 0.0
                for (n in range(hs)) {
                    let qv = q[i * d + hoff + n]
                    cs += (qv + wn[buoff + hoff + n]) * k[j * d + hoff + n]
                    rs += (qv + wn[bvoff + hoff + n]) * posl[(base + j) * d + hoff + n]
                }
                return (cs + rs) * scale
            }
        }
    }
    return <- ao
}

[test]
def test_vkt_tower_fastconformer(t0 : T?) {
    t0 |> run("the FastConformer conv module tail (the centered depthwise conv + folded BN + silu) == the CPU form") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_cn_dw_cls() || !ensure_tower_pk_dw_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let rows = ROWS_N
            let nd = rows * d
            let glu_out <- row_fill(nd, 31, 7, 0.06, 2.5)
            let wn <- row_fill(2 * d + d * CN_CONV_KERNEL + 2 * d, 7, 1, 0.01, 0.3)
            let taps_off = 2 * d
            let bnw_off = taps_off + d * CN_CONV_KERNEL
            let bnb_off = bnw_off + d
            let kpad = (CN_CONV_KERNEL - 1) / 2
            with_cell_planes() $(var cp : CellPlanes) {
                let gp = cp |> in_(glu_out, 0, kpad * d)   // the rows past npos carry garbage: a tap past the source's end the bound lets through reads it
                let cpl = cp |> out_(nd)
                let wp = cp |> in_(wn)
                var wn_tm := wn   //! parakeet's layout: the taps tap-major [k][c], the BatchNorm rows as they are
                for (c in range(d)) {
                    for (kk in range(CN_CONV_KERNEL)) {
                        wn_tm[taps_off + kk * d + c] = wn[taps_off + c * CN_CONV_KERNEL + kk]
                    }
                }
                let cpl_pk = cp |> out_(nd)
                let wp_tm = cp |> in_(wn_tm)
                var s_dw = cp |> cls_set(fixed_array(gp, cpl, wp), @@set_tower_cn_dw_cls)
                var s_dw_pk = cp |> cls_set(fixed_array(gp, cpl_pk, wp_tm), @@set_tower_pk_dw_cls)
                var pa_dw = GkDwConvRowsArgs(c = uint(d), k = uint(CN_CONV_KERNEL), t_in = uint(rows), t_out = uint(rows), woff = uint(taps_off), bnwoff = uint(bnw_off),
                    bnboff = uint(bnb_off))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_cn_dw_cls(raw, h, s_dw, pa_dw, int64((nd + 255) / 256))
                    enc_tower_pk_dw_cls(raw, h, s_dw_pk, pa_dw, int64((nd + 255) / 256))
                }
                let c_dev <- cp |> plane_run(cpl)
                let c_pk_dev <- cp |> plane_run(cpl_pk)
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
                        c_ref[r * d + c] = bv * sigmoid_f32(bv)
                    }
                }
                check_rows(t, "tower_cn_dw centered taps, folded BatchNorm and silu vs the CPU loop", c_dev, c_ref, BAR_APPROX, d)
                check_rows(t, "tower_pk_dw the same over tap-major taps vs the CPU loop", c_pk_dev, c_ref, BAR_APPROX, d)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkt_tower_dw2d(t0 : T?) {
    t0 |> run("the subsample depthwise conv2d (k3 s2 p1) on both tap layouts == the CPU loop, the rows past the valid length masked") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_pk_dw2d_cls() || !ensure_tower_dw_conv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let ih = 11   //! odd along one axis, even along the other: each edge drops its own taps
            let iw = 10
            let ch = 8
            let valid = 7
            let oh = int(conv_s2_out(int64(ih)))
            let ow = int(conv_s2_out(int64(iw)))
            let total = oh * ow * ch
            let xv <- [for (i in range(ih * iw * ch)); float(float16(0.11 * float((i * 5) % 37) - 2.0))]
            let wv <- [for (i in range(9 * ch)); float(float16(0.07 * float((i * 7) % 29) - 1.0))]   //! tap-major [9][ch], parakeet's
            let bv <- [for (i in range(ch)); float(float16(0.3 * float(i % 5) - 0.6))]
            let want <- dw_s2_front_ref(false, false, iw, ch, oh, ow, ih, xv, wv, bv)
            let masked <- dw_s2_front_ref(false, false, iw, ch, oh, ow, valid, xv, wv, bv)
            var wn_pk := wv
            wn_pk |> push_from(bv)
            var wn_cn : array<float>   //! canary's [ch][9] taps
            wn_cn |> resize(9 * ch)
            for (c in range(ch)) {
                for (k9 in range(9)) {
                    wn_cn[c * 9 + k9] = wv[k9 * ch + c]
                }
            }
            wn_cn |> push_from(bv)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(xv)
                let wpk = cp |> in_(wn_pk)
                let wcn = cp |> in_(wn_cn)
                let ypk = cp |> out_(total)
                let ycn = cp |> out_(total)
                let ymask = cp |> out_(total)
                var s_pk = cp |> cls_set(fixed_array(xp, ypk, wpk), @@set_tower_pk_dw2d_cls)
                var s_cn = cp |> cls_set(fixed_array(xp, ycn, wcn), @@set_tower_dw_conv_cls)
                var s_mask = cp |> cls_set(fixed_array(xp, ymask, wpk), @@set_tower_pk_dw2d_cls)
                var pa = GkDwConv2dArgs(iw = uint(iw), ih = uint(ih), ih_valid = uint(ih), ch = uint(ch), woff = 0u, boff = uint(9 * ch))
                var pa_mask = pa
                pa_mask.ih_valid = uint(valid)
                let wgs = int64((total + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_pk_dw2d_cls(raw, h, s_pk, pa, wgs)
                    enc_tower_dw_conv_cls(raw, h, s_cn, pa, wgs)
                    enc_tower_pk_dw2d_cls(raw, h, s_mask, pa_mask, wgs)
                }
                let pk_dev <- cp |> plane_run(ypk)
                let cn_dev <- cp |> plane_run(ycn)
                let mask_dev <- cp |> plane_run(ymask)
                check_rows(t, "tower_pk_dw2d over tap-major taps vs the CPU loop", pk_dev, want, BAR_APPROX, ch)
                check_rows(t, "tower_dw_conv over channel-major taps vs the CPU loop", cn_dev, want, BAR_APPROX, ch)
                check_rows(t, "tower_pk_dw2d under the length mask vs the CPU loop over the valid rows", mask_dev, masked, BAR_APPROX, ch)
                t |> success(mismatch_exact(want, masked) > 0, "tower_pk_dw2d control: the mask moves the rows past the valid length")
            }
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
            if (!vk_moe_init() || !ensure_tower_ln_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_post_add_ln_cls() || !ensure_tower_post_add_ln_nb_cls()
                    || !ensure_tower_hpad_cls() || !ensure_tower_hpad16_cls() || !ensure_tower_hunpad_cls() || !ensure_tower_rope_tab_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let rows = ROWS_N
            let rows_pad = (rows + 63) / 64 * 64   // the padded panels' 64-aligned row cap
            let nd = rows * d
            let d_compact = ROWS_HEADS * LN_HS
            let d_padded = ROWS_HEADS * LN_HSP
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.3, 14.0)   // wide enough to reach both tails of the GELU (past +-10)
            let q <- row_fill(rows * d_compact, 29, 11, 0.06, 2.8)
            let qkv <- row_fill(rows * 3 * d_compact, 37, 5, 0.05, 2.2)   // a fused [q | k | v] row: the rope runs on its k slot in place
            let half = LN_HS / 2
            var cos_t <- row_fill(rows * half, 31, 2, 0.02, 0.97)
            var sin_t <- row_fill(rows * half, 19, 4, 0.03, 0.55)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // rows: ln w at 0, ln b at d, the bias at 2d, the next ln w at 3d (its bias reads the first ln's)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x)
                let xbp = cp |> out_(nd)
                let yp = cp |> in_(y)
                let act_arms = fixed_array(BIAS_ACT_GELU_TANH, BIAS_ACT_NONE, BIAS_ACT_GELU_ERF, BIAS_ACT_SILU, BIAS_ACT_RELU)
                let act_p <- [for (_a in act_arms); cp |> io_(g)]
                let xb2p = cp |> out_(nd)
                let x0p = cp |> io_(x)    // the ln_on = 0 arm's x: the seam with no next layernorm
                let xb0p = cp |> out_(nd)   // ... and its layernorm output, which must stay untouched
                let x_half_p = cp |> io_(x)    // the seam at ascale = 0.5, ln_on = 0
                let xb_half_p = cp |> out_(nd)
                let x_nb_p = cp |> io_(x)    // the bias-less seam (parakeet's blocks): the branch added bare, the next layernorm behind it
                let xb_nb_p = cp |> out_(nd)
                let wp = cp |> in_(wn)
                let qp = cp |> in_(q)
                let qpp = cp |> out_(rows_pad * d_padded)
                let qphp = cp |> out_words(int64(rows_pad * d_padded) * 2l, F16_SENTINEL_BITS)
                let qcp = cp |> out_(rows * d_compact)
                let qkvp = cp |> io_(qkv)
                let cosp = cp |> in_(cos_t)
                let sinp = cp |> in_(sin_t)
                var s_rope = cp |> cls_set(fixed_array(qkvp, cosp, sinp), @@set_tower_rope_tab_cls)
                var pa_rope = GkRopeTabArgs(rows = uint(rows), qs = uint(3 * d_compact), col = uint(d_compact), d = uint(ROWS_HEADS * LN_HS), dh = uint(LN_HS),
                    coff = 0u, soff = 0u)
                var s_ln = cp |> cls_set(fixed_array(xp, xbp, wp), @@set_tower_ln_cls)
                var s_acts <- [for (ap in act_p); cp |> cls_set(fixed_array(ap, wp), @@set_tower_bias_act_cls)]
                var s_post = cp |> cls_set(fixed_array(xp, yp, xb2p, wp), @@set_tower_post_add_ln_cls)
                var s_post0 = cp |> cls_set(fixed_array(x0p, yp, xb0p, wp), @@set_tower_post_add_ln_cls)
                var s_post_half = cp |> cls_set(fixed_array(x_half_p, yp, xb_half_p, wp), @@set_tower_post_add_ln_cls)
                var s_post_nb = cp |> cls_set(fixed_array(x_nb_p, yp, xb_nb_p, wp), @@set_tower_post_add_ln_nb_cls)
                var s_pad = cp |> cls_set(fixed_array(qp, qpp), @@set_tower_hpad_cls)
                var s_pad16 = cp |> cls_set(fixed_array(qp, qphp), @@set_tower_hpad16_cls)
                var s_unpad = cp |> cls_set(fixed_array(qpp, qcp), @@set_tower_hunpad_cls)
                var pa_ln = GkPostAddLnArgs(d = uint(d), rows = uint(rows), eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = uint(d), xboff = 0u)
                var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0, xboff = 0u)
                var pa_post0 = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 0u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0, xboff = 0u)
                var pa_post_half = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 0u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 0.5, xboff = 0u)
                var pa_rs = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP), sstride = uint(d_compact), soff = 0u)
                let wg4 = int64((nd / 4 + 255) / 256)
                let wg_rs = int64((rows_pad * d_padded + 255) / 256)   // the pads cover the 64-aligned cap: the rows past `rows` come out zero
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_ln_cls(raw, h, s_ln, pa_ln, int64(rows))
                    for (act, s_act in act_arms, s_acts) {
                        var pa_act = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = act)
                        enc_tower_bias_act_cls(raw, h, s_act, pa_act, wg4)
                    }
                    enc_tower_hpad_cls(raw, h, s_pad, pa_rs, wg_rs)
                    enc_tower_hpad16_cls(raw, h, s_pad16, pa_rs, wg_rs)
                    enc_tower_hunpad_cls(raw, h, s_unpad, pa_rs, wg_rs)
                    enc_tower_rope_tab_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * half + 255) / 256))
                    enc_tower_post_add_ln_cls(raw, h, s_post0, pa_post0, int64(rows))
                    enc_tower_post_add_ln_cls(raw, h, s_post_half, pa_post_half, int64(rows))
                    enc_tower_post_add_ln_nb_cls(raw, h, s_post_nb, pa_post, int64(rows))
                    enc_tower_post_add_ln_cls(raw, h, s_post, pa_post, int64(rows))   // last: it rewrites x, which the layernorm reads whole
                }
                let xb_dev <- cp |> plane_run(xbp)
                let g_dev <- cp |> plane_run(act_p[0])
                let x_dev <- cp |> plane_run(xp)
                let xb2_dev <- cp |> plane_run(xb2p)
                let qp_dev <- cp |> plane_run(qpp)
                let qph_dev <- cp |> plane_as(qphp, type<uint16>, rows_pad * d_padded)
                let qc_dev <- cp |> plane_run(qcp)
                let qkv_dev <- cp |> plane_run(qkvp)
                let g0_dev <- cp |> plane_run(act_p[1])
                let x0_dev <- cp |> plane_run(x0p)
                let xb0_dev <- cp |> plane_as(xb0p, type<uint>, nd)
                let gerf_dev <- cp |> plane_run(act_p[2])
                let gsilu_dev <- cp |> plane_run(act_p[3])
                let grelu_dev <- cp |> plane_run(act_p[4])
                let xhalf_dev <- cp |> plane_run(x_half_p)
                let xnb_dev <- cp |> plane_run(x_nb_p)
                let xbnb_dev <- cp |> plane_run(xb_nb_p)
                var xb_ref : array<float>
                xb_ref |> resize(nd)
                layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_ln layernorm rows vs layernorm_batch", xb_dev, xb_ref, BAR_APPROX, d)
                let xnb_ref <- [for (xv, yv in x, y); xv + yv]   //! the bias-less seam: the branch bare
                check_rows(t, "tower_post_add_ln_nb the seam without the bias row vs x + y", xnb_dev, xnb_ref, BAR_APPROX, d)
                var xbnb_ref : array<float>
                xbnb_ref |> resize(nd)
                layernorm_batch(xbnb_ref, xnb_ref, wn, int64(3 * d), int64(d), int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_post_add_ln_nb the next layernorm over the bare seam vs layernorm_batch", xbnb_dev, xbnb_ref, BAR_APPROX, d)
                t |> success(mismatch_rel("tower_post_add_ln_nb control (MUST mismatch)", x_dev, xnb_ref, 1e-4) > 0, "tower_post_add_ln_nb control: the biased stamp's seam lands off the bare one (the bias row is live)")
                var g_ref := g
                add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(rows))
                check_rows(t, "tower_bias_act BIAS_ACT_NONE the bias alone vs add_bias_rows", g0_dev, g_ref, BAR_APPROX, d)
                var gerf_ref := g_ref
                var gsilu_ref := g_ref
                let grelu_ref <- [for (v in g_ref); max(v, 0.0)]
                check_rows(t, "tower_bias_act BIAS_ACT_RELU the bias and relu vs max(x + b, 0)", grelu_dev, grelu_ref, BAR_APPROX, d)
                gelu_tanh_lut_batch(g_ref, int64(d), int64(rows))
                check_rows(t, "tower_bias_act the bias and the tanh GELU vs add_bias_rows + gelu_tanh_lut_batch", g_dev, g_ref, BAR_APPROX, d)
                // the erf arm evaluates the CPU's rational erfc in f32 where the CPU evaluates it in double: 1e-5 relative holds the
                // f32 rounding of the polynomial and the exp (a few ulp) and is well under the erf-vs-tanh gap (4.7e-4 at x = -2.7),
                // which the approx bar (2e-3) could not tell apart
                gelu_erf_batch(gerf_ref, int64(d), int64(rows))
                let erf_bar = 1e-5
                check_rows(t, "tower_bias_act BIAS_ACT_GELU_ERF the bias and the erf GELU vs add_bias_rows + gelu_erf_batch within {erf_bar}", gerf_dev, gerf_ref, Bar(rel = erf_bar, poison = 0.5), d)
                let tanh_off_erf = mismatch_rel("tower_bias_act tanh arm vs the erf oracle (MUST mismatch)", g_dev, gerf_ref, erf_bar)
                t |> success(tanh_off_erf > nd / 4, "tower_bias_act BIAS_ACT_GELU_ERF control: the tanh arm's rows miss the erf oracle at this bar ({tanh_off_erf} of {nd} off; the tails past +-10 agree by construction) - the bar tells the forms apart")
                silu(gsilu_ref, int64(nd))
                check_rows(t, "tower_bias_act BIAS_ACT_SILU the bias and silu vs add_bias_rows + silu", gsilu_dev, gsilu_ref, BAR_APPROX, d)
                t |> success(mismatch_approx(gsilu_dev, g_ref) > nd / 4, "tower_bias_act BIAS_ACT_SILU control: the silu arm is not the tanh GELU arm ({mismatch_approx(gsilu_dev, g_ref)} of {nd} apart; the positive tail agrees by construction)")
                var x_ref := x
                var yb := y
                add_bias_rows(yb, wn, int64(2 * d), int64(d), int64(rows))
                add_inplace_rows(x_ref, yb, int64(nd))
                check_rows(t, "tower_post_add_ln x += y + b", x_dev, x_ref, BAR_APPROX, d)
                var xb2_ref : array<float>
                xb2_ref |> resize(nd)
                layernorm_batch(xb2_ref, x_ref, wn, int64(3 * d), int64(d), int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_post_add_ln the next layernorm over the updated rows", xb2_dev, xb2_ref, BAR_APPROX, d)
                check_rows(t, "tower_post_add_ln ln_on = 0 the seam alone lands the same x", x0_dev, x_ref, BAR_APPROX, d)
                t |> equal(count(xb0_dev, $(w) => w == NAN_SENTINEL_BITS), nd, "tower_post_add_ln ln_on = 0: the layernorm output stays untouched (every word the sentinel)")
                var xhalf_ref := x
                for (i in range(nd)) {
                    xhalf_ref[i] += 0.5 * yb[i]
                }
                check_rows(t, "tower_post_add_ln ascale = 0.5 x += 0.5 . (y + b)", xhalf_dev, xhalf_ref, BAR_APPROX, d)
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
                let bad_tail = count(each(range(tail_lo, tail_hi)), $(i) => qp_dev[i] != 0.0)
                let bad_tail16 = count(each(range(tail_lo, tail_hi)), $(i) => uint(qph_dev[i]) != 0u)
                t |> success(rows_pad > rows && bad_tail == 0, "tower_hpad: the {rows_pad - rows} rows past the live count are zero ({bad_tail} off)")
                t |> success(bad_tail16 == 0, "tower_hpad16: the rows past the live count are zero in f16 ({bad_tail16} off)")
                var qp_poison := qp_dev
                var qph_poison := qph_dev
                qp_poison[tail_hi - 1] += 0.5
                qph_poison[tail_hi - 1] = uint16(f32_to_f16(0.5))
                t |> success(count(each(range(tail_lo, tail_hi)), $(i) => qp_poison[i] != 0.0) > 0 && count(each(range(tail_lo, tail_hi)), $(i) => uint(qph_poison[i]) != 0u) > 0, "tower_hpad / hpad16: the tail check reds a poisoned element")
                check_rows(t, "tower_hunpad compact rows back off the padded panel", qc_dev, q, BAR_EXACT, d_compact, LN_HS)
                var k_ref, k_dev : array<float>
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
                check_rows(t, "tower_rope_tab the k slot of the fused rows vs rope_neox_tab_rows", k_dev, k_ref, BAR_APPROX, d_compact, LN_HS)
                t |> success(bad_slots == 0, "tower_rope_tab: the q and v slots stay untouched ({bad_slots} moved)")
            }
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
            let d = ROWS_D
            let rows = ROWS_N
            let rows_pad = (rows + 63) / 64 * 64
            let nd = rows * d
            let d_compact = ROWS_HEADS * LN_HS
            let d_padded = ROWS_HEADS * LN_HSP
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            let g <- row_fill(nd, 13, 5, 0.3, 14.0)
            let q <- row_fill(rows * d_compact, 29, 11, 0.06, 2.8)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // ln w at 0, ln b at d, the bias at 2d (the q bias reads its first d_compact entries), the next ln w at 3d
            let bxh = int64(nd) * 2l
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x)
                let x_post16_p = cp |> io_(x)    // the f16 post-add's own x: both post-adds update x in place from the same start
                let x_post16z_p = cp |> io_(x)   // the f16 post-add at ln_on = 0: the seam alone
                let xb2h0p = cp |> out_words(bxh, F16_SENTINEL_BITS)       // ... and its half pre-norm output, which must stay untouched
                let act_arms = fixed_array(BIAS_ACT_GELU_TANH, BIAS_ACT_GELU_ERF, BIAS_ACT_RELU)
                let act_p <- [for (_a in act_arms); cp |> io_(g)]
                let act_hp <- [for (_a in act_arms); cp |> out_words(bxh, F16_SENTINEL_BITS)]
                let xbp = cp |> out_(nd)
                let xbhp = cp |> out_words(bxh, F16_SENTINEL_BITS)
                let yp = cp |> in_(y)
                let xb2p = cp |> out_(nd)
                let xb2hp = cp |> out_words(bxh, F16_SENTINEL_BITS)
                let wp = cp |> in_(wn)
                let qp = cp |> in_(q)
                let qpp = cp |> out_(rows_pad * d_padded)
                let qphp = cp |> out_words(int64(rows_pad * d_padded) * 2l, F16_SENTINEL_BITS)
                let qchp = cp |> out_words(int64(rows * d_compact) * 2l, F16_SENTINEL_BITS)
                var s_ln = cp |> cls_set(fixed_array(xp, xbp, wp), @@set_tower_ln_cls)
                var s_ln16 = cp |> cls_set(fixed_array(xp, wp, xbhp), @@set_tower_ln16_cls)
                var s_acts <- [for (ap in act_p); cp |> cls_set(fixed_array(ap, wp), @@set_tower_bias_act_cls)]
                var s_acts16 <- [for (ap, ahp in act_p, act_hp); cp |> cls_set(fixed_array(ap, wp, ahp), @@set_tower_bias_act_cvt16_cls)]
                var s_post = cp |> cls_set(fixed_array(xp, yp, xb2p, wp), @@set_tower_post_add_ln_cls)
                var s_post16 = cp |> cls_set(fixed_array(x_post16_p, yp, wp, xb2hp), @@set_tower_post_add_ln16_cls)
                var s_post16z = cp |> cls_set(fixed_array(x_post16z_p, yp, wp, xb2h0p), @@set_tower_post_add_ln16_cls)
                var s_padb = cp |> cls_set(fixed_array(qp, qpp, wp), @@set_tower_hpad_b_cls)
                var s_pad16b = cp |> cls_set(fixed_array(qp, qphp, wp), @@set_tower_hpad16_b_cls)
                var s_unpad16 = cp |> cls_set(fixed_array(qpp, qchp), @@set_tower_hunpad16_cls)
                var pa_ln = GkPostAddLnArgs(d = uint(d), rows = uint(rows), eps = ROWS_EPS, ln_on = 1u, lnoff = 0u, lnboff = uint(d), xboff = 0u)
                var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 1u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0, xboff = 0u)
                var pa_post0 = GkPostAddLnArgs(d = uint(d), rows = uint(rows), boff = uint(2 * d), eps = ROWS_EPS, ln_on = 0u, lnoff = uint(3 * d), lnboff = uint(d), ascale = 1.0, xboff = 0u)
                var pa_rs = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP),
                    sstride = uint(d_compact), soff = 0u, boff = uint(2 * d))
                let wg4 = int64((nd / 4 + 255) / 256)
                let wg_rs = int64((rows_pad * d_padded + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_ln_cls(raw, h, s_ln, pa_ln, int64(rows))
                    enc_tower_ln16_cls(raw, h, s_ln16, pa_ln, int64(rows))
                    for (act, s_act, s_act16 in act_arms, s_acts, s_acts16) {
                        var pa_act = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = act)
                        enc_tower_bias_act_cvt16_cls(raw, h, s_act16, pa_act, wg4)   // reads the rows before the in-place stamp rewrites them
                        enc_tower_bias_act_cls(raw, h, s_act, pa_act, wg4)
                    }
                    enc_tower_hpad_b_cls(raw, h, s_padb, pa_rs, wg_rs)
                    enc_tower_hpad16_b_cls(raw, h, s_pad16b, pa_rs, wg_rs)
                    enc_tower_hunpad16_cls(raw, h, s_unpad16, pa_rs, wg_rs)
                    enc_tower_post_add_ln_cls(raw, h, s_post, pa_post, int64(rows))   // last on x: the layernorms read x whole
                    enc_tower_post_add_ln16_cls(raw, h, s_post16, pa_post, int64(rows))
                    enc_tower_post_add_ln16_cls(raw, h, s_post16z, pa_post0, int64(rows))
                }
                let xb_dev <- cp |> plane_run(xbp)
                let xbh_dev <- cp |> plane_as(xbhp, type<uint16>, nd)
                let g_dev <- cp |> plane_run(act_p[0])
                let gh_dev <- cp |> plane_as(act_hp[0], type<uint16>, nd)
                let xb2h_dev <- cp |> plane_as(xb2hp, type<uint16>, nd)
                let qp_dev <- cp |> plane_run(qpp)
                let qph_dev <- cp |> plane_as(qphp, type<uint16>, rows_pad * d_padded)
                let qch_dev <- cp |> plane_as(qchp, type<uint16>, rows * d_compact)
                let gerf_dev <- cp |> plane_run(act_p[1])
                let gherf_dev <- cp |> plane_as(act_hp[1], type<uint16>, nd)
                let grelu_dev <- cp |> plane_run(act_p[2])
                let ghrelu_dev <- cp |> plane_as(act_hp[2], type<uint16>, nd)
                let x16z_dev <- cp |> plane_run(x_post16z_p)
                let xb2h0_dev <- cp |> plane_as(xb2h0p, type<uint>, nd / 2)
                var bad_post16 = 0
                check_halves(t, "tower_ln16: the f16 rows are the f32 stamp's rows rounded to half", xbh_dev, xb_dev)
                var xb_ref : array<float>
                xb_ref |> resize(nd)
                layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_ln16 half rows vs layernorm_batch", [for (hv in xbh_dev); f16_to_f32(uint(hv))], xb_ref, BAR_APPROX, d)
                check_halves(t, "tower_bias_act_cvt16: the f16 rows are the in-place stamp's rows rounded to half", gh_dev, g_dev)
                check_halves(t, "tower_bias_act_cvt16 BIAS_ACT_GELU_ERF: the f16 rows are the in-place stamp's rows rounded to half", gherf_dev, gerf_dev)
                check_halves(t, "tower_bias_act_cvt16 BIAS_ACT_RELU: the f16 rows are the in-place stamp's rows rounded to half", ghrelu_dev, grelu_dev)
                var x_ref := x
                var yb := y
                add_bias_rows(yb, wn, int64(2 * d), int64(d), int64(rows))
                add_inplace_rows(x_ref, yb, int64(nd))
                check_rows(t, "tower_post_add_ln16 ln_on = 0 the seam alone lands x += y + b", x16z_dev, x_ref, BAR_APPROX, d)
                t |> equal(count(xb2h0_dev, $(w) => w == F16_SENTINEL_BITS), nd / 2, "tower_post_add_ln16 ln_on = 0: the half pre-norm output stays untouched (every word the sentinel)")
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
                let bad_tail16 = count(each(range(tail_lo, tail_hi)), $(i) => qp_dev[i] != 0.0) + count(each(range(tail_lo, tail_hi)), $(i) => uint(qph_dev[i]) != 0u)
                t |> success(rows_pad > rows && bad_tail16 == 0, "tower_hpad_b / hpad16_b: the {rows_pad - rows} rows past the live count are zero ({bad_tail16} off)")
                var qp_poison := qp_dev
                var qph_poison := qph_dev
                qp_poison[tail_hi - 1] += 0.5
                qph_poison[tail_hi - 1] = uint16(f32_to_f16(0.5))
                t |> success(count(each(range(tail_lo, tail_hi)), $(i) => qp_poison[i] != 0.0) > 0 && count(each(range(tail_lo, tail_hi)), $(i) => uint(qph_poison[i]) != 0u) > 0, "tower_hpad_b / hpad16_b: the tail check reds a poisoned element")
            }
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
    let vf <- [for (hv in vh); f16_to_f32(uint(hv))]
    for (r in range(rows)) {
        let len = causal ? len0 + r : len0
        for (h in range(heads)) {
            attn_row_scored(vf, h * tstride * hs, hs, hs, 0, len, y, r * ydim + h * hs, false, 0.0) $(tt : int) : float {
                var s = 0.0
                for (j in range(hs)) {
                    s += (q[r * dim + h * hs + j] + wn[qboff + h * hs + j]) * f16_to_f32(uint(kh[(h * tstride + tt) * hs + j]))
                }
                return s * scale
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
            if (!vk_moe_init() || !ensure_tower_wdec_kv_cls() || !ensure_tower_wdec_kv16_cls() || !ensure_tower_wdec_attn_part_cls() || !ensure_tower_wdec_attn_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let hs = int(WDEC_HS)
            let heads = WD_HEADS
            let d = heads * hs
            let dim = 3 * d
            let tstride = WD_TSTRIDE
            let rows = WD_ROWS
            let p0 = WD_P0
            let scale = 1.0 / sqrt(float(hs))
            let kscale = wdec_kscale(int64(hs))
            let src <- row_fill(rows * dim, 31, 5, 0.05, 2.3)         // the batch's fused [q | k | v] rows
            let hist <- row_fill(p0 * dim, 23, 7, 0.04, 1.7)          // the positions before the batch, the same row shape
            let wn <- row_fill(2 * d, 11, 3, 0.02, 0.9)               // the q bias row at 0, the v bias row at d
            let bkv = int64(heads * tstride * hs) * 2l
            let nch = (tstride + int(WDEC_CHUNK) - 1) / int(WDEC_CHUNK)
            with_cell_planes() $(var cp : CellPlanes) {
                let srcp = cp |> in_(src)
                let histp = cp |> in_(hist)
                let wp = cp |> in_(wn)
                let kp = cp |> out_words(bkv, F16_SENTINEL_BITS)
                let vp = cp |> out_words(bkv, F16_SENTINEL_BITS)
                let kxp = cp |> out_(heads * tstride * hs)
                let vxp = cp |> out_(heads * tstride * hs)
                let pmsp = cp |> out_(nch * heads * rows * 2)
                let pyp = cp |> out_(nch * heads * rows * hs)
                let yp = cp |> out_(rows * d)
                let ycp = cp |> out_(rows * d)
                var s_kh = cp |> cls_set(fixed_array(histp, kp, wp), @@set_tower_wdec_kv16_cls)
                var s_vh = cp |> cls_set(fixed_array(histp, vp, wp), @@set_tower_wdec_kv16_cls)
                var s_kb = cp |> cls_set(fixed_array(srcp, kp, wp), @@set_tower_wdec_kv16_cls)
                var s_vb = cp |> cls_set(fixed_array(srcp, vp, wp), @@set_tower_wdec_kv16_cls)
                var s_kbx = cp |> cls_set(fixed_array(srcp, kxp, wp), @@set_tower_wdec_kv_cls)
                var s_vbx = cp |> cls_set(fixed_array(srcp, vxp, wp), @@set_tower_wdec_kv_cls)
                var s_attn = cp |> cls_set(fixed_array(srcp, kp, vp, pmsp, pyp, wp), @@set_tower_wdec_attn_part_cls)
                var s_comb = cp |> cls_set(fixed_array(pmsp, pyp, yp), @@set_tower_wdec_attn_comb_cls)
                var s_combc = cp |> cls_set(fixed_array(pmsp, pyp, ycp), @@set_tower_wdec_attn_comb_cls)
                var pa_kh = GkWdecKvArgs(rows = uint(p0), sstride = uint(dim), soff = uint(d), nheads = uint(heads), tstride = uint(tstride), p0 = 0u,
                    dst_off = 0u, boff = 0u, bias_on = 0u, mode = WDEC_KV_F16, hs = uint(hs), kscale = wdec_kscale(int64(hs)))
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
                var pa_as = GkWdecAttnArgs(dim = uint(dim), tstride = uint(tstride), len0 = uint(p0 + 1), causal = 1u, nheads = uint(heads),
                    nrows = uint(rows), koff = 0u, voff = 0u, qboff = 0u, hs = uint(hs), scale = 1.0 / sqrt(float(hs)))
                let len_cross = 150   // inside the first chunk, so the cross rows reach the combine with the second chunk's partial empty
                var pa_ac = pa_as
                pa_ac.len0 = uint(len_cross)
                pa_ac.causal = 0u
                var pa_c = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(heads), nrows = uint(rows), hs = uint(hs))
                let wg_h = int64((p0 * d + 255) / 256)
                let wg_b = int64((rows * d + 255) / 256)
                let wg_attn = int64(nch * heads * rows)
                let wg_comb = int64(heads * rows)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_wdec_kv16_cls(raw, h, s_kh, pa_kh, wg_h)
                    enc_tower_wdec_kv16_cls(raw, h, s_vh, pa_vh, wg_h)
                    enc_tower_wdec_kv16_cls(raw, h, s_kb, pa_kb, wg_b)
                    enc_tower_wdec_kv16_cls(raw, h, s_vb, pa_vb, wg_b)
                    enc_tower_wdec_kv_cls(raw, h, s_kbx, pa_kx, wg_b)
                    enc_tower_wdec_kv_cls(raw, h, s_vbx, pa_vx, wg_b)
                    enc_tower_wdec_attn_part_cls(raw, h, s_attn, pa_as, wg_attn)
                    enc_tower_wdec_attn_comb_cls(raw, h, s_comb, pa_c, wg_comb)
                    enc_tower_wdec_attn_part_cls(raw, h, s_attn, pa_ac, wg_attn)
                    enc_tower_wdec_attn_comb_cls(raw, h, s_combc, pa_c, wg_comb)
                }
                let k_dev <- cp |> plane_as(kp, type<uint16>, heads * tstride * hs)
                let v_dev <- cp |> plane_as(vp, type<uint16>, heads * tstride * hs)
                let kx_dev <- cp |> plane_run(kxp)
                let vx_dev <- cp |> plane_run(vxp)
                let y_dev <- cp |> plane_run(yp)
                let yc_dev <- cp |> plane_run(ycp)
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
                check_rows(t, "tower_wdec_attn part + comb, causal, across the chunk boundary vs the CPU form", y_dev, y_ref, BAR_APPROX, d, hs)
                let yc_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, len_cross, 0, false, scale)
                check_rows(t, "tower_wdec_attn part + comb, cross, over {len_cross} keys (the second chunk empty) vs the CPU form", yc_dev, yc_ref, BAR_APPROX, d, hs)
                t |> success(mismatch_approx(y_dev, yc_ref) > rows * d / 4, "tower_wdec_attn control: the causal rows are not the cross rows ({mismatch_approx(y_dev, yc_ref)} of {rows * d} apart)")
                let ys_ref <- wdec_attn_ref(src, k_dev, v_dev, wn, rows, dim, d, heads, hs, tstride, p0 + 1, 0, true, scale * 2.0)
                t |> success(mismatch_approx(y_dev, ys_ref) > rows * d / 4, "tower_wdec_attn control: the keys scaled twice move the rows ({mismatch_approx(y_dev, ys_ref)} of {rows * d} apart)")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PAT_HEADS = 16   //! the SigLIP head count: sixteen 72-wide heads through the padded 128 route

[test, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
def test_vkt_tower_padded_attn(t0 : T?) {
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
            let bph = int64(rows_pad * dp) * 2l
            with_cell_planes() $(var cp : CellPlanes) {
                let qp = cp |> in_(q)
                let kp = cp |> in_(k)
                let vp = cp |> in_(v)
                let qpp = cp |> out_(rows_pad * dp)
                let kpp = cp |> out_words(bph, F16_SENTINEL_BITS)
                let vpp = cp |> out_words(bph, F16_SENTINEL_BITS)
                let attpp = cp |> out_(rows_pad * dp)
                let attp = cp |> out_(rows * dc)
                var s_pq = cp |> cls_set(fixed_array(qp, qpp), @@set_tower_hpad_cls)
                var s_pk = cp |> cls_set(fixed_array(kp, kpp), @@set_tower_hpad16_cls)
                var s_pv = cp |> cls_set(fixed_array(vp, vpp), @@set_tower_hpad16_cls)
                let fa = fixed_array(qpp, kpp, vpp, attpp)
                var s_fa = fa_stamp_set(khr, gather_planes(cp.dev, fa), gather_planes(cp.bytes, fa), gather_planes(cp.hazard_bit, fa))
                var s_un = cp |> cls_set(fixed_array(attpp, attp), @@set_tower_hunpad_cls)
                var pa_rs = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(PAT_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP), sstride = uint(dc), soff = 0u)
                var pa_fa = FaArgs(rows = uint(rows), w0 = 0u, qd = uint(dp), kvd = uint(dp), kv_mul = 1u, kvbase = 0u, scale = scale, window = 0u, sinkoff = 0u)
                let wg_rs = int64((rows_pad * dp + 255) / 256)   // the pads cover the 64-aligned cap: the K/V shadows past `rows` are zero, not the sentinel
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_hpad_cls(raw, h, s_pq, pa_rs, wg_rs)
                    enc_tower_hpad16_cls(raw, h, s_pk, pa_rs, wg_rs)
                    enc_tower_hpad16_cls(raw, h, s_pv, pa_rs, wg_rs)
                    fa_stamp_enc(khr, int64(LN_HSP), false, false, false, false, raw, h, s_fa, pa_fa, fa_stamp_wgs(khr, int64(PAT_HEADS), int64(rows)), true)
                    enc_tower_hunpad_cls(raw, h, s_un, pa_rs, wg_rs)
                }
                let att_dev <- cp |> plane_run(attp)
                var att_ref, hp, attsc : array<float>
                att_ref |> resize(rows * dc)
                with_dasllama_jobque_() {
                    attention_bidir(att_ref, q, k, v, hp, attsc, int64(rows), int64(dc), int64(PAT_HEADS), scale)
                }
                // the bar: `approx`'s 2e-3 covers the tile's f16 staging of the padded Q panel and of the softmax rows (P) that the
                // f32 oracle keeps in f32, and the coopmat's reassociated sums; K and V are rounded to f16 on both sides
                check_rows(t, "the padded route over {PAT_HEADS} heads of {LN_HS} vs attention_bidir", att_dev, att_ref, BAR_APPROX, dc, LN_HS)
            }
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
            if (!ensure_tower_win_attn_cls() || !ensure_tower_post_add_rms_cls() || !ensure_tower_bias_gate16_cls()) {
                t |> skip("the window route's classes declined on this device")
                return
            }
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
            let d = ROWS_D
            let nrows = ROWS_N
            let nd = nrows * d
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            var y <- row_fill(nd, 17, 3, 0.04, 1.9)
            var g <- row_fill(nd, 13, 5, 0.09, 4.0)
            var u <- row_fill(nd, 23, 9, 0.07, 3.3)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // rows: the o bias at 0, the rms weight at d, the gate bias at 2d, the up bias at 3d
            with_cell_planes() $(var cp : CellPlanes) {
                let qp = cp |> in_(q)
                let kp = cp |> in_(k)
                let vp = cp |> in_(v)
                let attp = cp |> out_(rows * dc)
                let wlp = cp |> in_plain(wlo)
                let xp = cp |> io_(x)
                let yp = cp |> in_(y)
                let xbp = cp |> out_(nd)
                let x0p = cp |> io_(x)    // the ln_on = 0 arm's x: the seam with no next rms
                let xb0p = cp |> out_(nd)   // ... and its rms output, which must stay untouched
                let gp = cp |> in_(g)
                let up = cp |> in_(u)
                let wp = cp |> in_(wn)
                let gated_h16p = cp |> out_words(int64(nd) * 2l, F16_SENTINEL_BITS)
                var s_win = cp |> cls_set(fixed_array(qp, kp, vp, attp, wlp), @@set_tower_win_attn_cls)
                var s_post = cp |> cls_set(fixed_array(xp, yp, xbp, wp), @@set_tower_post_add_rms_cls)
                var s_post0 = cp |> cls_set(fixed_array(x0p, yp, xb0p, wp), @@set_tower_post_add_rms_cls)
                var s_glu = cp |> cls_set(fixed_array(gp, up, wp, gated_h16p), @@set_tower_bias_gate16_cls)
                var pa_win = GkWinAttnArgs(nheads = uint(WIN_HEADS), hs = uint(WIN_HS), scale = scale)
                var pa_post = GkPostAddLnArgs(d = uint(d), rows = uint(nrows), boff = 0u, eps = ROWS_EPS, ln_on = 1u, lnoff = uint(d), lnboff = 0u, ascale = 1.0, xboff = 0u)
                var pa_post0 = GkPostAddLnArgs(d = uint(d), rows = uint(nrows), boff = 0u, eps = ROWS_EPS, ln_on = 0u, lnoff = uint(d), lnboff = 0u, ascale = 1.0, xboff = 0u)
                var pa_glu = GkBiasGateArgs(d = uint(d), nelem = uint(nd), goff = uint(2 * d), uoff = uint(3 * d))
                let wg4 = int64((nd / 4 + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_win_attn_cls(raw, h, s_win, pa_win, int64(nslot * WIN_HEADS))
                    enc_tower_bias_gate16_cls(raw, h, s_glu, pa_glu, wg4)
                    enc_tower_post_add_rms_cls(raw, h, s_post0, pa_post0, int64(nrows))
                    enc_tower_post_add_rms_cls(raw, h, s_post, pa_post, int64(nrows))
                }
                let att_dev <- cp |> plane_run(attp)
                let x_dev <- cp |> plane_run(xp)
                let xb_dev <- cp |> plane_run(xbp)
                let gh_dev <- cp |> plane_as(gated_h16p, type<uint16>, nd)
                let x0_dev <- cp |> plane_run(x0p)
                let xb0_dev <- cp |> plane_as(xb0p, type<uint>, nd)
                let g_dev <- [for (hv in gh_dev); f16_to_f32(uint(hv))]
                var att_ref, hpp, attsc : array<float>
                att_ref |> resize(rows * dc)
                with_dasllama_jobque_() {
                    attention_bidir_windows(att_ref, q, k, v, hpp, attsc, win_lo, int64(dc), int64(WIN_HEADS), scale)
                }
                check_rows(t, "tower_win_attn over {nslot} windows, one ragged, vs attention_bidir_windows", att_dev, att_ref, BAR_APPROX, dc, WIN_HS)
                // the leak control: the same rows attended in full differ on the first row, so the windows' walls are the class's doing
                var full_ref : array<float>
                full_ref |> resize(rows * dc)
                with_dasllama_jobque_() {
                    attention_bidir(full_ref, q, k, v, hpp, attsc, int64(rows), int64(dc), int64(WIN_HEADS), scale)
                }
                let moved = logits_maxdiff(copy_span(full_ref, 0, dc), copy_span(att_dev, 0, dc))
                t |> success(moved > 0.01, "tower_win_attn control: full attention over the same rows lands elsewhere (maxdiff {moved})")
                var x_ref := x
                var yb := y
                add_bias_rows(yb, wn, 0l, int64(d), int64(nrows))
                add_inplace_rows(x_ref, yb, int64(nd))
                check_rows(t, "tower_post_add_rms x += y + b", x_dev, x_ref, BAR_APPROX, d)
                var xb_ref : array<float>
                xb_ref |> resize(nd)
                let wplane = PlaneF(p = unsafe(addr<float const?>(wn[0])), n = int64(4 * d))
                rms_rows(xb_ref, x_ref, wplane, int64(d), int64(d), int64(nrows), ROWS_EPS)
                check_rows(t, "tower_post_add_rms the next weighted rms over the updated rows", xb_dev, xb_ref, BAR_APPROX, d)
                check_rows(t, "tower_post_add_rms ln_on = 0 the seam alone lands the same x", x0_dev, x_ref, BAR_APPROX, d)
                t |> equal(count(xb0_dev, $(w) => w == NAN_SENTINEL_BITS), nd, "tower_post_add_rms ln_on = 0: the rms output stays untouched (every word the sentinel)")
                var g_ref := g
                var u_ref := u
                add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(nrows))
                add_bias_rows(u_ref, wn, int64(3 * d), int64(d), int64(nrows))
                swiglu(g_ref, u_ref, int64(nd))
                check_rows(t, "actf16_b (the gated hidden) silu(g + bg) . (u + bu) as halves vs the CPU form", g_dev, g_ref, BAR_APPROX, d)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! `check_exact` of half rows against `want` rounded to half, saturating at the half range
def private check_halves(t : T?; tag : string; got : array<uint16>; want : array<float>) {
    var want_h <- cvt_f32_to_f16_sat_array(want)
    check_exact(t, tag, got, want_h)
    delete want_h
}

let private FM_IW = 7          //! the k3 s2 p1 conv over the 7 x 5 image gives the 4 x 3 output
let private FM_IH = 5
let private FM_CIN = 3
let private FM_CHUNKS = 2
let private FM_OW = int(conv_s2_out(int64(FM_IW)))
let private FM_OH = int(conv_s2_out(int64(FM_IH)))
let private FM_KDP = 64        //! the column row's padded width: cin x 9 = 27 live columns, the rest zero

[test]
def test_vkt_tower_front_movers(t0 : T?) {   // nolint:STYLE038 — one flat gate over the data movers: the fixture, one command buffer, the exact oracles and their controls
    t0 |> run("the audio fronts' data movers (im2col, the stem's half im2col, the shuffles, the one-head restride, clamp + halfword, add plane, the q3a finish's bias passes, ln + relu) == their forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_im2col_cls() || !ensure_tts_im2col16_cls() || !ensure_tower_q3a_shuffle_cls() || !ensure_tower_feat_shuffle_cls()
                    || !ensure_tower_hpad16_cls() || !ensure_tower_hpad16_b_cls() || !ensure_tower_clamp_cvt_cls() || !ensure_tower_bias_act_cls()
                    || !ensure_tower_ln_relu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
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
            let kdp2 = 3 * nch2 + 2   // the half im2col's four-column lattice: the two columns past 3 nch read zero
            let rows2 = (len1 + 1) / 2
            let q3_nch = 5
            let q3_tok = int(QWEN3A_TOK_PER_CHUNK)   // the kernels carry the family's positions a chunk
            let q3_feat = 16 * q3_nch
            let q3_pos = FM_CHUNKS * q3_tok
            let fs_t = 3
            let fs_w = 4
            let fs_c = 5
            let add_n = 4 * d
            let src <- row_fill(n_src, 31, 5, 0.07, 3.3)
            let mel1 <- row_fill(nch1 * len1, 23, 7, 0.09, 2.0)
            let cv2 <- row_fill(len1 * nch2, 19, 3, 0.08, 2.2)
            let y3 <- row_fill(FM_CHUNKS * 16 * q3_tok * q3_nch, 11, 2, 0.06, 2.9)
            let fs_src <- row_fill(fs_t * fs_w * fs_c, 13, 9, 0.05, 1.5)
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            let wn <- row_fill((3 + q3_tok) * d, 7, 1, 0.01, -0.6)   // the panel's bias row at 0, the add plane at d, the finish's bias at 2d and its tok position rows at 3d, the ln weight at 0
            with_cell_planes() $(var cp : CellPlanes) {
                let srcp = cp |> in_(src)
                let colp = cp |> out_words(int64(n_col) * 2l, F16_SENTINEL_BITS)
                let mel1p = cp |> in_(mel1)
                let col1p = cp |> out_words(int64(len1 * kdp1) * 2l, F16_SENTINEL_BITS)
                let cv2p = cp |> in_(cv2)
                let col2p = cp |> out_words(int64(rows2 * kdp2) * 2l, F16_SENTINEL_BITS)
                let y3p = cp |> in_(y3)
                let featp = cp |> out_words(int64(q3_pos * q3_feat) * 2l, F16_SENTINEL_BITS)
                let fsp = cp |> in_(fs_src)
                let fsop = cp |> out_(fs_t * fs_w * fs_c)
                let xp = cp |> in_(x)
                let wp = cp |> in_(wn)
                let gqp = cp |> out_words(int64(rows * hs) * 2l, F16_SENTINEL_BITS)
                let gpp = cp |> out_words(int64(rows * hs) * 2l, F16_SENTINEL_BITS)
                let cvtp = cp |> out_words(int64(nd) * 2l, F16_SENTINEL_BITS)
                let addp = cp |> io_(x)
                let finp = cp |> io_(x)
                let lnp = cp |> io_(x)
                var s_im = cp |> cls_set(fixed_array(srcp, colp), @@set_tower_im2col_cls)
                var s_im1 = cp |> cls_set(fixed_array(mel1p, col1p), @@set_tts_im2col16_cls)
                var s_im2 = cp |> cls_set(fixed_array(cv2p, col2p), @@set_tts_im2col16_cls)
                var s_sh = cp |> cls_set(fixed_array(y3p, featp), @@set_tower_q3a_shuffle_cls)
                var s_fs = cp |> cls_set(fixed_array(fsp, fsop), @@set_tower_feat_shuffle_cls)
                var s_gq = cp |> cls_set(fixed_array(xp, gqp, wp), @@set_tower_hpad16_b_cls)
                var s_gp = cp |> cls_set(fixed_array(xp, gpp), @@set_tower_hpad16_cls)
                var s_cvt = cp |> cls_set(fixed_array(xp, cvtp), @@set_tower_clamp_cvt_cls)
                var s_add = cp |> cls_set(fixed_array(addp, wp), @@set_tower_bias_act_cls)
                var s_fin = cp |> cls_set(fixed_array(finp, wp), @@set_tower_bias_act_cls)
                var s_ln = cp |> cls_set(fixed_array(lnp, lnp, wp), @@set_tower_ln_relu_cls)
                var pa_im = GkIm2col2dArgs(iw = uint(FM_IW), ih = uint(FM_IH), cin = uint(FM_CIN), sstride = uint(FM_CIN), kdp = uint(FM_KDP), rows = uint(FM_CHUNKS * FM_OH * FM_OW), soff = 0u)
                var pa_im1 = GkIm2col1dArgs(cin = uint(nch1), tin = uint(len1), tout = uint(len1), k = 3u, stride = 1u, pad_l = 1u, dilation = 1u, kdim = uint(kdp1),
                    row_major = 0u, r0 = 0u, mp = uint(len1))
                var pa_im2 = GkIm2col1dArgs(cin = uint(nch2), tin = uint(len1), tout = uint(rows2), k = 3u, stride = 2u, pad_l = 1u, dilation = 1u, kdim = uint(kdp2),
                    row_major = 1u, r0 = 0u, mp = uint(rows2))
                var pa_sh = GkQ3aShuffleArgs(nch = uint(q3_nch), ncp = uint(q3_nch), chunk_stride = uint(16 * 13 * q3_nch), npos = uint(q3_pos))
                var pa_fs = GkFeatShuffleArgs(w = uint(fs_w), ch = uint(fs_c), t = uint(fs_t), rows_pad = uint(fs_t))
                var pa_gq = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d),
                    soff = uint(second_head_base), boff = uint(second_head_base))
                var pa_gp = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d),
                    soff = uint(second_head_base))
                var pa_cvt = GkClampArgs(nelem = uint(nd), lo = -1.5, hi = 1.5, scale = 1.0)
                var pa_add = GkBiasActArgs(d = uint(add_n), nelem = uint(add_n), boff = uint(d), act = BIAS_ACT_NONE)
                var pa_fin_b = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_NONE)
                var pa_fin_p = GkBiasActArgs(d = uint(q3_tok * d), nelem = uint(nd), boff = uint(3 * d), act = BIAS_ACT_NONE)
                var pa_ln = GkLnReluRowsArgs(cout = uint(d), ldin = uint(d), ldout = uint(d), rows = uint(rows), woff = 0u, eps = ROWS_EPS)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_im2col_cls(raw, h, s_im, pa_im, int64((n_col + 255) / 256))
                    enc_tts_im2col16_cls(raw, h, s_im1, pa_im1, int64(len1 * kdp1))
                    enc_tts_im2col16_cls(raw, h, s_im2, pa_im2, int64(rows2 * kdp2))
                    enc_tower_q3a_shuffle_cls(raw, h, s_sh, pa_sh, int64((q3_pos * q3_feat + 255) / 256))
                    enc_tower_feat_shuffle_cls(raw, h, s_fs, pa_fs, int64((fs_t * fs_w * fs_c + 255) / 256))
                    enc_tower_hpad16_b_cls(raw, h, s_gq, pa_gq, int64((rows * hs + 255) / 256))
                    enc_tower_hpad16_cls(raw, h, s_gp, pa_gp, int64((rows * hs + 255) / 256))
                    enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, int64((nd / 4 + 255) / 256))
                    enc_tower_bias_act_cls(raw, h, s_add, pa_add, int64((add_n / 4 + 255) / 256))
                    enc_tower_bias_act_cls(raw, h, s_fin, pa_fin_b, int64((nd / 4 + 255) / 256))
                    enc_tower_bias_act_cls(raw, h, s_fin, pa_fin_p, int64((nd / 4 + 255) / 256))
                    enc_tower_ln_relu_cls(raw, h, s_ln, pa_ln, int64((rows + 255) / 256))
                }
                let col_dev <- cp |> plane_as(colp, type<uint16>, n_col)
                let col1_dev <- cp |> plane_as(col1p, type<uint16>, len1 * kdp1)
                let col2_dev <- cp |> plane_as(col2p, type<uint16>, rows2 * kdp2)
                let feat_dev <- cp |> plane_as(featp, type<uint16>, q3_pos * q3_feat)
                let fs_dev <- cp |> plane_run(fsop)
                let gq_dev <- cp |> plane_as(gqp, type<uint16>, rows * hs)
                let gp_dev <- cp |> plane_as(gpp, type<uint16>, rows * hs)
                let cvt_dev <- cp |> plane_as(cvtp, type<uint16>, nd)
                let add_dev <- cp |> plane_run(addp)
                let fin_dev <- cp |> plane_run(finp)
                let ln_dev <- cp |> plane_run(lnp)
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
                check_halves(t, "tower_im2col: the k3 s2 p1 columns over two chunks, the pad and the zero tail", col_dev, col_want)
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
                    col2_want[gid] = k < 3 * nch2 && sp >= 0 && sp < len1 ? cv2[sp * nch2 + k % nch2] : 0.0
                }
                check_halves(t, "the stem's im2col (tts_im2col16, channel-major, stride 1): the mel's k3 columns with the zero pad and tail", col1_dev, col1_want)
                check_halves(t, "the stem's im2col (tts_im2col16, row-major, stride 2): the conv output's k3 columns at every second position and the zero pad columns", col2_dev, col2_want)
                var feat_want : array<float>
                feat_want |> resize(q3_pos * q3_feat)
                for (gid in range(q3_pos * q3_feat)) {
                    let row = gid / q3_feat
                    let j = gid % q3_feat
                    feat_want[gid] = y3[(row / q3_tok) * (16 * q3_tok * q3_nch) + ((j % 16) * q3_tok + row % q3_tok) * q3_nch + j / 16]
                }
                check_halves(t, "tower_q3a_shuffle: the feature rows off the third conv stage", feat_dev, feat_want)
                var fs_want : array<float>
                fs_want |> resize(fs_t * fs_w * fs_c)
                for (gid in range(fs_t * fs_w * fs_c)) {
                    let tt = gid / (fs_w * fs_c)
                    let rem = gid % (fs_w * fs_c)
                    fs_want[gid] = fs_src[(tt * fs_w + rem % fs_w) * fs_c + rem / fs_w]
                }
                check_rows(t, "tower_feat_shuffle [time][ch][freq] rows off the [time][freq][ch] image", fs_dev, fs_want, BAR_EXACT, fs_w * fs_c)
                var gq_want, gp_want : array<float>
                gq_want |> resize(rows * hs)
                gp_want |> resize(rows * hs)
                for (r in range(rows)) {
                    for (n in range(hs)) {
                        gp_want[r * hs + n] = x[r * d + second_head_base + n]
                        gq_want[r * hs + n] = x[r * d + second_head_base + n] + wn[second_head_base + n]
                    }
                }
                check_halves(t, "tower_hpad16_b restride at one head (the canary panel): the head's rows plus the bias row as halves", gq_dev, gq_want)
                check_halves(t, "tower_hpad16 restride at one head (the canary panel): the head's rows as halves", gp_dev, gp_want)
                var gq_h <- cvt_f32_to_f16_sat_array(gq_want)
                t |> success(mismatch_exact(gp_dev, gq_h) > rows * hs / 2, "the one-head restride: the plain arm misses the biased rows")
                delete gq_h
                var cvt_want : array<float>
                cvt_want |> resize(nd)
                var clamped = 0
                for (i in range(nd)) {
                    cvt_want[i] = clamp(x[i], -1.5, 1.5)
                    if (cvt_want[i] != x[i]) {
                        clamped++
                    }
                }
                check_halves(t, "tower_clamp_cvt: the clamped rows as halves", cvt_dev, cvt_want)
                t |> success(clamped > nd / 8, "tower_clamp_cvt: the fixture reaches the clamp ({clamped} of {nd} elements clamped)")
                var add_want := x
                for (i in range(add_n)) {
                    add_want[i] = x[i] + wn[d + i]
                }
                check_rows(t, "tower_bias_act as the plane add (d = nelem): the plane added element for element and nothing past it touched", add_dev, add_want, BAR_EXACT, d)
                t |> success(add_dev[0] != x[0], "tower_bias_act as the plane add control: the rows moved off x")
                var fin_want : array<float>
                fin_want |> resize(nd)
                for (gid in range(nd)) {
                    let e = gid % d
                    fin_want[gid] = x[gid] + wn[2 * d + e] + wn[3 * d + ((gid / d) % q3_tok) * d + e]
                }
                check_rows(t, "tower_bias_act twice as the q3a finish: the conv_out bias and the per-chunk position rows added", fin_dev, fin_want, BAR_APPROX, d)
                t |> success(!approx(fin_dev[d], x[d]), "tower_bias_act as the q3a finish control: the rows moved off x")
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
                check_rows(t, "tower_ln_relu channel LayerNorm then ReLU vs the CPU loop", ln_dev, ln_want, BAR_APPROX, d)
                let live_ln = find_index_if(ln_want, $(v) => v != 0.0)
                t |> success(ln_dev[live_ln] != x[live_ln], "tower_ln_relu control: a live element moved off x")
            }
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
            let n_pw = MEL_NLEN * MEL_NBINS
            let n_mel = MEL_NLEN * MEL_NMEL
            let n_norm = NORM_NLEN * MEL_NMEL
            let dw_in = FM_IH * FM_IW * DW_CH
            let dw_out = FM_OH * FM_OW * DW_CH
            let tt = ROWS_N
            let ww = 2 * tt - 1
            let n_rel = ww * REL_D
            let xw <- row_fill(MEL_NLEN * MEL_NFFT, 37, 5, 0.03, 1.4)
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
            let mel_in <- row_fill(n_norm, 29, 11, 0.07, 2.4)
            let dw_x <- row_fill(dw_in, 23, 9, 0.06, 2.7)
            let dw_w <- row_fill(DW_CH * 9 + DW_CH, 7, 2, 0.05, 0.9)   // the taps at 0, the bias row at ch x 9
            let dw_lanes = (dw_out + 255) / 256 * 256   // the dispatch's lanes: the output past dw_out keeps the sentinel
            with_cell_planes() $(var cp : CellPlanes) {
                let xwp = cp |> in_(xw)
                let twp = cp |> in_(tw)
                let magp = cp |> out_(n_pw)
                let powp = cp |> out_(n_pw)
                let pwp = cp |> in_(pw)
                let filp = cp |> in_(fil)
                let g4ap = cp |> out_(n_mel)
                let cnp = cp |> out_(n_mel)
                let whp = cp |> out_(n_mel)
                let normp = cp |> io_(mel_in)
                let dwxp = cp |> in_(dw_x, 0, FM_IW * DW_CH)   // an image row of garbage past the source: a tap below the image the row guard lets through reads it
                let dwyp = cp |> out_(dw_out, 0, dw_lanes - dw_out)
                let dwwp = cp |> in_(dw_w)
                let relp = cp |> out_(n_rel)
                var s_mag = cp |> cls_set(fixed_array(xwp, twp, magp), @@set_tower_spec_dft_cls)
                var s_pow = cp |> cls_set(fixed_array(xwp, twp, powp), @@set_tower_spec_dft_cls)
                var s_g4a = cp |> cls_set(fixed_array(pwp, filp, g4ap), @@set_tower_mel_log_cls)
                var s_cn = cp |> cls_set(fixed_array(pwp, filp, cnp), @@set_tower_mel_log_cls)
                var s_wh = cp |> cls_set(fixed_array(pwp, filp, whp), @@set_tower_mel_log_cls)
                var s_norm = cp |> cls_set(fixed_array(normp), @@set_tower_mel_norm_cls)
                var s_dw = cp |> cls_set(fixed_array(dwxp, dwyp, dwwp), @@set_tower_dw_conv_cls)
                var s_rel = cp |> cls_set(fixed_array(relp), @@set_tower_rel_sincos_cls)
                var pa_mag = TowerSpecDftArgs(n_fft = uint(MEL_NFFT), n_frames = uint(MEL_NLEN), mag = 1u)
                var pa_pow = TowerSpecDftArgs(n_fft = uint(MEL_NFFT), n_frames = uint(MEL_NLEN), mag = 0u)
                var pa_g4a = TowerMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), n_frames = uint(MEL_NLEN),
                    floor = MEL_MAX_FLOOR_G4A, scale = 1.0, frame_major = 1u, add_floor = 0u)
                var pa_cn = TowerMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), n_frames = uint(MEL_NLEN),
                    floor = MEL_FLOOR_24, scale = 1.0, frame_major = 1u, add_floor = 1u)
                var pa_wh = TowerMelLogArgs(n_bins = uint(MEL_NBINS), n_mel = uint(MEL_NMEL), n_frames = uint(MEL_NLEN),
                    floor = MEL_FLOOR_24, scale = MEL_LOG10_E, frame_major = 0u, add_floor = 0u)
                var pa_norm = GkMelNormArgs(n_mel = uint(MEL_NMEL), n_frames = uint(NORM_NLEN), valid = uint(NORM_VALID))
                var pa_dw = GkDwConv2dArgs(iw = uint(FM_IW), ih = uint(FM_IH), ih_valid = uint(FM_IH), ch = uint(DW_CH), woff = 0u, boff = uint(DW_CH * 9))
                var pa_rel = GkRelSinCosArgs(d = uint(REL_D), tt = uint(tt), rows_pad = uint(2 * tt - 1))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_spec_dft_cls(raw, h, s_mag, pa_mag, int64((n_pw + 255) / 256))
                    enc_tower_spec_dft_cls(raw, h, s_pow, pa_pow, int64((n_pw + 255) / 256))
                    enc_tower_mel_log_cls(raw, h, s_g4a, pa_g4a, int64((n_mel + 255) / 256))
                    enc_tower_mel_log_cls(raw, h, s_cn, pa_cn, int64((n_mel + 255) / 256))
                    enc_tower_mel_log_cls(raw, h, s_wh, pa_wh, int64((n_mel + 255) / 256))
                    enc_tower_mel_norm_cls(raw, h, s_norm, pa_norm, int64((MEL_NMEL + 63) / 64))
                    enc_tower_dw_conv_cls(raw, h, s_dw, pa_dw, int64((dw_out + 255) / 256))
                    enc_tower_rel_sincos_cls(raw, h, s_rel, pa_rel, int64((n_rel + 255) / 256))
                }
                let mag_dev <- cp |> plane_run(magp)
                let pow_dev <- cp |> plane_run(powp)
                let g4a_dev <- cp |> plane_run(g4ap)
                let cn_dev <- cp |> plane_run(cnp)
                let wh_dev <- cp |> plane_run(whp)
                let norm_dev <- cp |> plane_run(normp)
                let dw_dev <- cp |> plane_run(dwyp)
                let dw_idle <- cp |> plane_as(dwyp, type<uint>, dw_lanes - dw_out, dw_out)
                let rel_dev <- cp |> plane_run(relp)
                var mag_want, pow_want : array<float>
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
                check_rows(t, "tower_spec_dft (magnitude) frames' spectra vs the double DFT", mag_dev, mag_want, BAR_APPROX, MEL_NBINS)
                check_rows(t, "tower_spec_dft (power) vs re^2 + im^2", pow_dev, pow_want, BAR_APPROX, MEL_NBINS)
                t |> success(!approx(pow_dev[1], mag_want[1]), "tower_spec_dft control: the power arm is not the magnitude")
                var g4a_want, cn_want, wh_want : array<float>
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
                check_rows(t, "tower_mel_log (frame-major, max floor) vs the gemma4a form", g4a_dev, g4a_want, BAR_APPROX, MEL_NMEL)
                check_rows(t, "tower_mel_log (frame-major, added floor) vs the canary form", cn_dev, cn_want, BAR_APPROX, MEL_NMEL)
                check_rows(t, "tower_mel_log (mel-major, log10) vs the whisper preprocessors' form", wh_dev, wh_want, BAR_APPROX, MEL_NLEN)
                t |> success(approx(g4a_dev[0], log(MEL_MAX_FLOOR_G4A)) && cn_dev[0] < g4a_dev[0] - 1.0, "tower_mel_log: the quiet frame floors on the max arm and reads under the floor on the add arm")
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
                check_rows(t, "tower_mel_norm column statistics over the valid frames and the zero tail vs the CPU loop", norm_dev, norm_want, BAR_APPROX, MEL_NMEL)
                let tail_off = count(subarray(norm_dev, range(NORM_VALID * MEL_NMEL, n_norm)), $(v) => v != 0.0)
                t |> success(tail_off == 0 && norm_dev[0] != mel_in[0], "tower_mel_norm: the tail is exact zeros ({tail_off} tail words live) and the rows moved off the input")
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
                check_rows(t, "tower_dw_conv k3 s2 p1 depthwise taps with the bias vs the CPU form", dw_dev, dw_want, BAR_APPROX, DW_CH)
                t |> equal(count(dw_idle, $(w) => w == NAN_SENTINEL_BITS), dw_lanes - dw_out, "tower_dw_conv: the idle lanes past the output write nothing (every word past it the sentinel)")
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
                check_rows(t, "tower_rel_sincos table vs the CPU loop", rel_dev, rel_want, BAR_APPROX, REL_D)
                var zero_row_off = 0
                for (k in range(REL_D / 2)) {
                    if (rel_dev[(tt - 1) * REL_D + 2 * k] != 0.0 || rel_dev[(tt - 1) * REL_D + 2 * k + 1] != 1.0) {
                        zero_row_off++
                    }
                }
                t |> success(zero_row_off == 0, "tower_rel_sincos: the rel 0 row is exactly (0, 1) ({zero_row_off} of the zero row off)")
            }
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
def test_vkt_tower_cn_attn_rel_plane(t0 : T?) {
    t0 |> run("the rel-plane FastConformer attention (both head widths) over CPU-built planes == the CPU loop") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_cn_attn_r64_cls() || !ensure_tower_cn_attn_r_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let rows = ROWS_N
            let hs = d / ROWS_HEADS
            let heads2 = ROWS_HEADS / 2
            let hs2 = d / heads2
            let nd = rows * d
            let ww = 2 * rows - 1
            let q <- row_fill(nd, 41, 3, 0.03, 1.1)
            let k <- row_fill(nd, 17, 11, 0.04, 1.3)
            let v <- row_fill(nd, 29, 13, 0.05, 1.7)
            let posl <- row_fill(ww * d, 13, 2, 0.02, 0.9)
            let wn <- row_fill(2 * d, 7, 1, 0.01, 0.3)   // bias_u at 0, bias_v at d
            let wg = int64((rows + 63) / 64)
            let ao_rows = int(wg) * 64   // a row a lane: the idle lanes' rows past `rows` keep the sentinel
            let nidle = (ao_rows - rows) * d
            with_cell_planes() $(var cp : CellPlanes) {
                let qp = cp |> in_(q)
                let kp = cp |> in_(k)
                let vp = cp |> in_(v)
                let aop = cp |> out_(nd, 0, nidle)
                let ao2p = cp |> out_(nd, 0, nidle)
                let wp = cp |> in_(wn)
                let head_rel_planes <- [for (hh in range(ROWS_HEADS + heads2));   // a plane a head: the 64-wide heads first, then the two 128-wide ones
                    cp |> in_(cn_rel_plane(q, posl, wn, rows, ww, d, hh < ROWS_HEADS ? hs : hs2, hh < ROWS_HEADS ? hh * hs : (hh - ROWS_HEADS) * hs2, d))]
                var sets <- [for (hh in range(ROWS_HEADS + heads2));
                    hh < ROWS_HEADS
                        ? cp |> cls_set(fixed_array(qp, kp, vp, aop, head_rel_planes[hh], wp), @@set_tower_cn_attn_r64_cls)
                        : cp |> cls_set(fixed_array(qp, kp, vp, ao2p, head_rel_planes[hh], wp), @@set_tower_cn_attn_r_cls)]
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    for (hh in range(ROWS_HEADS)) {
                        var pa = TowerCnAttnRArgs(d = uint(d), npos = uint(rows), buoff = 0u, hoff = uint(hh * hs))
                        enc_tower_cn_attn_r64_cls(raw, h, sets[hh], pa, wg)
                    }
                    for (hh in range(heads2)) {
                        var pa = TowerCnAttnRArgs(d = uint(d), npos = uint(rows), buoff = 0u, hoff = uint(hh * hs2))
                        enc_tower_cn_attn_r_cls(raw, h, sets[ROWS_HEADS + hh], pa, wg)
                    }
                }
                let ao_dev <- cp |> plane_run(aop)
                let ao2_dev <- cp |> plane_run(ao2p)
                let ao_idle <- cp |> plane_as(aop, type<uint>, nidle, nd)
                let ao2_idle <- cp |> plane_as(ao2p, type<uint>, nidle, nd)
                t |> success(ao_rows > rows, "the dispatch carries idle lanes past the live rows ({ao_rows - rows} of {ao_rows})")
                t |> equal(count(ao_idle, $(w) => w == NAN_SENTINEL_BITS) + count(ao2_idle, $(w) => w == NAN_SENTINEL_BITS), 2 * (ao_rows - rows) * d, "tower_cn_attn_r: the idle lanes write nothing past the live rows on both stamps (every word the sentinel)")
                let ao_ref <- cn_attn_ref(q, k, v, posl, wn, rows, d, ROWS_HEADS, hs, 0, d)
                let ao2_ref <- cn_attn_ref(q, k, v, posl, wn, rows, d, heads2, hs2, 0, d)
                check_rows(t, "tower_cn_attn_r64 rel-plane attention over four 64-wide heads vs the CPU loop", ao_dev, ao_ref, BAR_APPROX, d, hs)
                check_rows(t, "tower_cn_attn_r rel-plane attention over two 128-wide heads vs the CPU loop", ao2_dev, ao2_ref, BAR_APPROX, d, hs2)
                let k_scaled <- scaled_copy(k, 1.5)
                let posl_scaled <- scaled_copy(posl, 1.5)
                let ctl_k <- cn_attn_ref(q, k_scaled, v, posl, wn, rows, d, ROWS_HEADS, hs, 0, d)
                let ctl_p <- cn_attn_ref(q, k, v, posl_scaled, wn, rows, d, heads2, hs2, 0, d)
                let moved_k = mismatch_approx(ao_dev, ctl_k)
                let moved_p = mismatch_approx(ao2_dev, ctl_p)
                t |> success(moved_k > nd / 4 && moved_p > nd / 4, "tower_cn_attn_r controls: scaled keys ({moved_k} moved) and a scaled rel table ({moved_p} moved) land the oracle elsewhere")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
