options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

module test_vulkan_tower_kernels

require dastest/testing_boost public
require math   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if arms; the no-vulkan lane sees it unused
require daslib/f16_cvt   // nolint:STYLE030,LINT019 — used only inside the vulkan static_if arms; the no-vulkan lane sees it unused
require dasllama/dasllama_plane   // nolint:STYLE030,LINT019 — the same
require dasllama/dasllama_tower   // nolint:STYLE030,LINT019 — the same
require dasllama/dasllama_common
require dasllama/dasllama_convert   // nolint:STYLE030,LINT019 — the same
require dasllama/dasllama_qwen3a   // nolint:STYLE030,LINT019 — QWEN3A_TOK_PER_CHUNK: the front movers' qwen3a fixture, inside the vulkan arm alone
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require _attn_oracle   // nolint:STYLE030,LINT019 — attn_row_scored: the softmax row the attention oracles share, inside the vulkan arms alone
require _kernel_oracles   // nolint:STYLE030,LINT019 — pos_add_2d_ref, inside the vulkan arms alone
require dasllama/dasllama_qwen3v   // nolint:STYLE030,LINT019 — q3v_reorder_src, the same
require _vk_kq_fixtures   // nolint:STYLE030,LINT019 — f16r, inside the vulkan arms alone
require _nmt_oracle   // nolint:STYLE030,LINT019 — the Bergamot step cells' fixtures and references, inside the vulkan arm alone
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 - the oracles' device-math twins, used only inside the vulkan static_if arms (a build without the module drops them)
require daslib/jobque_boost
require daslib/linq   // nolint:STYLE030,LINT019 — count: the sentinel words a dispatch left, the pads' zero tails, inside the vulkan arms alone
require _discrete_cells   // nolint:STYLE030,LINT019 — the case bodies the CUDA tower cells run too, called inside the vulkan arms alone

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

//! whether the device carries the flash tile family `khr` names - the cm2 trio, or KHR cooperative matrices at subgroup 32;
//! a pipeline of a family the device lacks is never built, since a driver without coopmat can fault in its compile
[unused_argument(khr)]
def private vkt_fa_arm_on(khr : bool) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return khr ? khr_kq_tile_on() : cm2_fa_on()
    } else {
        return false
    }
}

[test(description = "The bidirectional flash tile (cm2 and KHR, h64 and h128) over 77 rows matches the all-keys CPU attention at the 2e-3 approx bar, keeps sentinel slack rows past kvlen masked, and differs from its causal twin on row 0 only")]
def test_vkt_fa_bidir(t0 : T?) {
    t0 |> run("bidirectional flash attention (h64 and the padded h128, cm2 and KHR arms) == the CPU oracle over every key") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_clamp_cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            var served = 0
            if (!vkt_fa_arm_on(false) && !vkt_fa_arm_on(true)) {
                t |> skip("the device has neither the cm2 flash trio nor KHR cooperative matrices at subgroup 32: no flash tile builds")
                return
            }
            for (arm in fixed_array((khr = false, hs = 64), (khr = false, hs = 128), (khr = true, hs = 64), (khr = true, hs = 128))) {
                if (!vkt_fa_arm_on(arm.khr) || !fa_stamp_ensure(arm.khr, int64(arm.hs), false, false, false, false, true) || !fa_stamp_ensure(arm.khr, int64(arm.hs), false, false, false, false)) {
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

[test(description = "The layernorm, post-add, bias-GELU and attention combine fused with the Q8_0 requant land the separate pass's Q8_0 bytes and scales exactly at widths 256, 384 and 1280, and their rows match the CPU forms at the approx bar")]
def test_vkt_wdec_fused_rq(t0 : T?) {
    t0 |> run("the layernorm, the post-add, the bias-activation and the attention combine fused with the Q8_0 requant == the pass then TowerClampRq and the CPU forms, at the other cells' width, whisper tiny's and turbo's") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_ln_rq_cls() || !ensure_tower_post_add_ln_rq_cls() || !ensure_tower_bias_act_rq_cls() || !ensure_tower_wdec_attn_comb_rq_cls()
                || !ensure_tower_ln_cls() || !ensure_tower_post_add_ln_cls() || !ensure_tower_bias_act_cls() || !ensure_tower_clamp_rq_cls() || !ensure_tower_wdec_attn_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                wdec_fused_rq_case(t, cp, true)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "`vt_tile_rows` rounds a GEMM record's rows up to a whole 256-wide l column (1500 to 1536, 1152 to 1280) and leaves the raw count for the m and s columns and the q8 batch tile")]
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


[test(description = "The tower row classes match the CPU tower helpers over 37 rows: clamp, clamp copy (source untouched) and the f16 clamp feed bit for bit, rope2d and geglu at the approx bar, plus the shared rms, post-add, clamp+rq and plain post-add cases")]
def test_vkt_tower_rows(t0 : T?) {   // nolint:STYLE038 — one flat gate over the five inline row classes: the fixture, one command buffer, five oracles and their controls
    t0 |> run("the tower row classes (clamp, clamp copy, clamp+cvt, rope2d, geglu; then the shared rms, post-add, clamp+rq and post-add plain cases) == the CPU tower helpers") <| @(t : T?) {
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
            let x <- row_fill(nd, 41, 7, 0.05, 2.1)
            let y <- row_fill(nd, 17, 3, 0.04, 1.9)
            let q <- row_fill(nd, 29, 11, 0.06, 2.8)
            let g <- row_fill(nff, 13, 5, 0.09, 4.0)
            let u <- row_fill(nff, 23, 9, 0.07, 3.3)
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
                let qp = cp |> io_(q)
                let gp = cp |> io_(g)
                let up = cp |> in_(u)
                let tp = cp |> in_(tabs)
                let xhp = cp |> out_words(int64(nd) * 2l, F16_SENTINEL_BITS)
                var s_clamp = cp |> cls_set(fixed_array(yp), @@set_tower_clamp_cls)
                var s_ccopy = cp |> cls_set(fixed_array(cc_src_p, cc_dst_p), @@set_tower_clamp_copy_cls)
                var s_cvt = cp |> cls_set(fixed_array(xp, xhp), @@set_tower_clamp_cvt_cls)
                var s_rope = cp |> cls_set(fixed_array(qp, tp), @@set_tower_rope2d_cls)
                var s_geglu = cp |> cls_set(fixed_array(gp, up), @@set_tower_geglu_cls)
                var pa_clamp = GkClampArgs(nelem = uint(nd), lo = lo, hi = hi, scale = 1.0)
                var pa_cvt = GkClampArgs(nelem = uint(nd), lo = lo2, hi = hi2, scale = 1.0)
                var pa_rope = GkRope2dArgs(nheads = uint(ROWS_HEADS), hs = uint(hs), rows = uint(rows))
                var pa_geglu = GkGegluArgs(nelem = uint(nff), glo = -2.0, ghi = 2.5, ulo = -1.5, uhi = 1.75, dlo = -1.0, dhi = 1.2)
                let wg4 = int64((nd / 4 + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, wg4)
                    enc_tower_rope2d_cls(raw, h, s_rope, pa_rope, int64((rows * ROWS_HEADS * pairs + 255) / 256))
                    enc_tower_geglu_cls(raw, h, s_geglu, pa_geglu, int64((nff / 4 + 255) / 256))
                    enc_tower_clamp_cls(raw, h, s_clamp, pa_clamp, wg4)
                    enc_tower_clamp_copy_cls(raw, h, s_ccopy, pa_clamp, wg4)
                }
                let y_dev <- cp |> plane_run(yp)
                let ys_dev <- cp |> plane_run(cc_src_p)
                let yc_dev <- cp |> plane_run(cc_dst_p)
                let g_dev <- cp |> plane_run(gp)
                let xh_dev <- cp |> plane_as(xhp, type<uint16>, nd)
                let q_dev <- cp |> plane_run(qp)
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
                var g_ref : array<float>
                g_ref |> resize(nff)
                for (i in range(nff)) {
                    let gv = clamp(g[i], -2.0, 2.5)
                    let uv = clamp(u[i], -1.5, 1.75)
                    g_ref[i] = clamp(gelu_quick_lut_f32(gv) * uv, -1.0, 1.2)
                }
                check_rows(t, "tower_geglu clamped GEGLU-quick vs the CPU table form", g_dev, g_ref, BAR_APPROX, ff)
            }
            with_cell_planes() $(var cp : CellPlanes) {
                rms_case(t, cp)
                post_add_case(t, cp)
                clamp_rq_case(t, cp)
                post_add_plain_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private VR_W = 96             //! the vision rows' canvas: 6 x 4 patches of 16 - an even grid for the merge-adjacent walk, 24 rows a 4-multiple
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
[test(description = "The vision stem and tail stamps match the CPU forms: patch im2col with the [-1, 1] map and zero pad rows, the 2-axis position add and the qwen3v merge-walk assemble bit for bit, grid mean pool and affine rows at the approx bar")]
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
                var pa_im = GkPatchIm2colArgs(w = uint(VR_W), p = uint(VR_P), row_dim = uint(VR_PD), npos = uint(npos),
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
                let x_want <- pos_add_2d_ref(x, tbl, npos, d, VR_PW, VR_POS_ENTRIES)
                check_rows(t, "tower_pos_add_2d (row + x-table row) + y-table row vs the CPU form", x_dev, x_want, BAR_EXACT, d)
                t |> success(moved_off(x_want, x, 0, BAR_EXACT), "tower_pos_add_2d: the rows moved off x")
                var asm_want : array<float>
                asm_want |> resize(nd)
                for (r in range(npos)) {
                    let src = int(q3v_reorder_src(int64(r), int64(VR_PW)))
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
                t |> success(moved_off(aff_want, ax, 0, BAR_APPROX) && moved_off(aff0_want, ax, 0, BAR_APPROX), "tower_affine_rows: both arms moved off x")
                check_rows(t, "tower_affine_rows x . scale on the scale-only arm", ax0_dev, aff0_want, BAR_APPROX, d)
                delete col_cpu
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the chat projector tails' row classes on the templates both homes stamp, on the CUDA cell's case body
[test(description = "The chat projector tails' row-pair mean pool and swapped SwiGLU at an off-multiple width of 70 match the CPU forms at the approx bar with idle lanes untouched, and the unswapped gate lands elsewhere")]
def test_vkt_tower_tail_rows(t0 : T?) {
    t0 |> run("the pair pool and the swapped SwiGLU rows == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_pool_rows_cls() || !ensure_tower_swiglu_swapped_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                tail_rows_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the Conformer row classes: the unbiased silu stamp here, the GLU, the causal depthwise taps and the windowed RPE attention on
//! both head widths on the CUDA cell's case body
[test(description = "The Conformer stamps (silu, GLU, causal depthwise taps, windowed RPE attention at head widths 64 and 128) match the CPU loops at the approx bar; scaled keys and a 4x softcap oracle land elsewhere, proving the cap engages")]
def test_vkt_tower_conformer(t0 : T?) {
    t0 |> run("the Conformer row classes (silu, GLU, depthwise conv, windowed RPE attention) == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_act_cls() || !ensure_tower_glu_sig_cls() || !ensure_tower_dw_conv5_cls()
                    || !ensure_tower_g4a_attn64_cls() || !ensure_tower_g4a_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = ROWS_D
            let nd = ROWS_N * d
            var s <- row_fill(nd, 23, 5, 0.09, 6.0)   // silu's rows: both tails
            with_cell_planes() $(var cp : CellPlanes) {
                let sp = cp |> io_(s)
                var s_silu = cp |> cls_set(fixed_array(sp), @@set_tower_act_cls)
                var pa_silu = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = 0u, act = BIAS_ACT_SILU)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_act_cls(raw, h, s_silu, pa_silu, int64((nd / 4 + 255) / 256))
                }
                let s_dev <- cp |> plane_run(sp)
                var s_ref := s
                silu(s_ref, int64(nd))
                check_rows(t, "tower_act (the unbiased silu stamp) vs silu", s_dev, s_ref, BAR_APPROX, d)
                conformer_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The FastConformer conv tail (centered depthwise conv, folded BatchNorm, silu) on channel-major and tap-major taps, and gigaam's biased form, match the CPU loop at the approx bar with poison past npos never read")]
def test_vkt_tower_fastconformer(t0 : T?) {
    t0 |> run("the FastConformer conv module tail (the centered depthwise conv + folded BN + silu) == the CPU form") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_cn_dw_cls() || !ensure_tower_pk_dw_cls() || !ensure_tower_ga_dw_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                fastconformer_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! gigaam's depthwise conv over the carrier's f16 taps (`TowerGaDwH`: FORM 3, tap-major, TAP_F16) on the shapes the Metal cell runs
//! vs the CPU loop over the same halfwords (`fc_dw_bias_rows_f16`), itself held to the f32 loop over the widened taps to the
//! bit; the control a poisoned halfword tap
[test(description = "Gigaam's f16-tap depthwise conv matches the CPU loop over the same halfwords within 1e-6 relative at k5 and k31, that loop is bit-exact to the f32 loop, and a poisoned halfword tap reds the bar")]
def test_vkt_tower_ga_dwh(t0 : T?) {
    t0 |> run("the f16-tap depthwise conv == the CPU loop over the same halfwords") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_ga_dwh_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                ga_dwh_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Parakeet's k3 s2 p1 depthwise conv2d over tap-major taps matches the CPU loop at the approx bar, unmasked and under a valid-length mask that measurably changes the rows, idle lanes untouched")]
def test_vkt_tower_dw2d(t0 : T?) {
    t0 |> run("parakeet's subsample depthwise conv2d (k3 s2 p1) over tap-major taps == the CPU loop, the rows past the valid length masked") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_pk_dw2d_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                pk_dw2d_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private LN_HS = 72   //! the SigLIP head width: off the flash tiles' 64 / 128, so the restride pads it to 128
let private LN_HSP = 128

[test(description = "The biased-block classes match the CPU helpers: layernorm, five bias-activations (erf GELU at 1e-5, told apart from tanh), post-add+ln with its ln_on 0 and ascale 0.5 seams, exact head pad/unpad with zero tails, and the k-slot rope")]
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
                var xb_ref : array<float>
                xb_ref |> resize(nd)
                layernorm_batch(xb_ref, x, wn, 0l, int64(d), int64(d), int64(rows), ROWS_EPS)
                check_rows(t, "tower_ln layernorm rows vs layernorm_batch", xb_dev, xb_ref, BAR_APPROX, d)
                var g_ref := g
                add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(rows))
                check_rows(t, "tower_bias_act BIAS_ACT_NONE the bias alone vs add_bias_rows", g0_dev, g_ref, BAR_APPROX, d)
                var gerf_ref := g_ref
                var gsilu_ref := g_ref
                let grelu_ref <- [for (v in g_ref); max(v, 0.0)]
                check_rows(t, "tower_bias_act BIAS_ACT_RELU the bias and relu vs max(x + b, 0)", grelu_dev, grelu_ref, BAR_APPROX, d)
                gelu_tanh_lut_batch(g_ref, int64(d), int64(rows))
                check_rows(t, "tower_bias_act the bias and the tanh GELU vs add_bias_rows + gelu_tanh_lut_batch", g_dev, g_ref, BAR_APPROX, d)
                // the erf arm evaluates the CPU's rational erfc fit in the shader's f32 as the CPU kernel does on its lattice: 1e-5
                // relative holds the two evaluations' rounding of the polynomial and the exp (a few ulp) and is well under the
                // erf-vs-tanh gap (4.7e-4 at x = -2.7), which the approx bar (2e-3) could not tell apart
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
            with_cell_planes() $(var cp : CellPlanes) {
                post_add_ln_nb_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Each f16-feed stamp is its f32 twin rounded to half bit for bit: bias_act_cvt16 on three activations, the biased head pads with zero tails, hunpad16, and the ln16 and post_add_ln16 halves, which also hold the layernorm bar")]
def test_vkt_tower_f16_feeds(t0 : T?) {   // nolint:STYLE038 — one flat gate over the four inline stamps: the fixture, one command buffer, the twins and their controls
    t0 |> run("the f16-feed stamps (hpad_b, hpad16_b, hunpad16, bias_act_cvt16; then the shared ln16 and post_add_ln16 cases) == their f32 stamps rounded to half") <| @(t : T?) {
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
            let g <- row_fill(nd, 13, 5, 0.3, 14.0)
            let q <- row_fill(rows * d_compact, 29, 11, 0.06, 2.8)
            let wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // the bias at 2d (the q bias reads its first d_compact entries)
            let bxh = int64(nd) * 2l
            with_cell_planes() $(var cp : CellPlanes) {
                let act_arms = fixed_array(BIAS_ACT_GELU_TANH, BIAS_ACT_GELU_ERF, BIAS_ACT_RELU)
                let act_p <- [for (_a in act_arms); cp |> io_(g)]
                let act_hp <- [for (_a in act_arms); cp |> out_words(bxh, F16_SENTINEL_BITS)]
                let wp = cp |> in_(wn)
                let qp = cp |> in_(q)
                let qpp = cp |> out_(rows_pad * d_padded)
                let qphp = cp |> out_words(int64(rows_pad * d_padded) * 2l, F16_SENTINEL_BITS)
                let qchp = cp |> out_words(int64(rows * d_compact) * 2l, F16_SENTINEL_BITS)
                var s_acts <- [for (ap in act_p); cp |> cls_set(fixed_array(ap, wp), @@set_tower_bias_act_cls)]
                var s_acts16 <- [for (ap, ahp in act_p, act_hp); cp |> cls_set(fixed_array(ap, wp, ahp), @@set_tower_bias_act_cvt16_cls)]
                var s_padb = cp |> cls_set(fixed_array(qp, qpp, wp), @@set_tower_hpad_b_cls)
                var s_pad16b = cp |> cls_set(fixed_array(qp, qphp, wp), @@set_tower_hpad16_b_cls)
                var s_unpad16 = cp |> cls_set(fixed_array(qpp, qchp), @@set_tower_hunpad16_cls)
                var pa_rs = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows_pad), nheads = uint(ROWS_HEADS), hs = uint(LN_HS), hsp = uint(LN_HSP),
                    sstride = uint(d_compact), soff = 0u, boff = uint(2 * d))
                let wg4 = int64((nd / 4 + 255) / 256)
                let wg_rs = int64((rows_pad * d_padded + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    for (act, s_act, s_act16 in act_arms, s_acts, s_acts16) {
                        var pa_act = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = act)
                        enc_tower_bias_act_cvt16_cls(raw, h, s_act16, pa_act, wg4)   // reads the rows before the in-place stamp rewrites them
                        enc_tower_bias_act_cls(raw, h, s_act, pa_act, wg4)
                    }
                    enc_tower_hpad_b_cls(raw, h, s_padb, pa_rs, wg_rs)
                    enc_tower_hpad16_b_cls(raw, h, s_pad16b, pa_rs, wg_rs)
                    enc_tower_hunpad16_cls(raw, h, s_unpad16, pa_rs, wg_rs)
                }
                let g_dev <- cp |> plane_run(act_p[0])
                let gh_dev <- cp |> plane_as(act_hp[0], type<uint16>, nd)
                let qp_dev <- cp |> plane_run(qpp)
                let qph_dev <- cp |> plane_as(qphp, type<uint16>, rows_pad * d_padded)
                let qch_dev <- cp |> plane_as(qchp, type<uint16>, rows * d_compact)
                let gerf_dev <- cp |> plane_run(act_p[1])
                let gherf_dev <- cp |> plane_as(act_hp[1], type<uint16>, nd)
                let grelu_dev <- cp |> plane_run(act_p[2])
                let ghrelu_dev <- cp |> plane_as(act_hp[2], type<uint16>, nd)
                check_halves(t, "tower_bias_act_cvt16: the f16 rows are the in-place stamp's rows rounded to half", gh_dev, g_dev)
                check_halves(t, "tower_bias_act_cvt16 BIAS_ACT_GELU_ERF: the f16 rows are the in-place stamp's rows rounded to half", gherf_dev, gerf_dev)
                check_halves(t, "tower_bias_act_cvt16 BIAS_ACT_RELU: the f16 rows are the in-place stamp's rows rounded to half", ghrelu_dev, grelu_dev)
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
            with_cell_planes() $(var cp : CellPlanes) {
                ln16_case(t, cp)
                post_add_ln16_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The whisper decoder's K/V store writes its F16, KX and VX layouts bit for bit as the CPU chain, and the chunked attention pair matches the CPU form at the approx bar on causal rows across a chunk boundary and on cross rows")]
def test_vkt_wdec_classes(t0 : T?) {
    t0 |> run("the whisper decoder's K/V store and chunked attention pair == the CPU decoder forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_wdec_kv_cls() || !ensure_tower_wdec_kv16_cls() || !ensure_tower_wdec_attn_part_cls() || !ensure_tower_wdec_attn_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                wdec_classes_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PAT_HEADS = 16   //! the SigLIP head count: sixteen 72-wide heads through the padded 128 route

[test(description = "The padded attention route (pad 72-wide heads to 128, the h128 bidirectional flash tile, unpad) over sixteen heads and 77 rows matches `attention_bidir` at the 2e-3 approx bar"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
def test_vkt_tower_padded_attn(t0 : T?) {
    t0 |> run("the padded attention route (pad, the h128 bidirectional tile, unpad) == attention_bidir at 72-wide heads") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            let khr = !cm2_fa_on()
            if (!vkt_fa_arm_on(khr)) {
                t |> skip("the device has neither the cm2 flash trio nor KHR cooperative matrices at subgroup 32: no flash tile builds")
                return
            }
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

[test(description = "The window-attention class over 982 rows in sixteen windows, one ragged, matches `attention_bidir_windows` at the approx bar while full attention lands elsewhere; the f16 gated hidden and the rms seam match their CPU forms"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-tower-routes")]
def test_vkt_tower_win_rows(t0 : T?) {   // nolint:STYLE038 — the window route end to end plus the gated hidden: the fixture, the command buffer, the oracles and their controls
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
            var g <- row_fill(nd, 13, 5, 0.09, 4.0)
            var u <- row_fill(nd, 23, 9, 0.07, 3.3)
            var wn <- row_fill(4 * d, 7, 1, 0.01, -0.6)   // rows: the gate bias at 2d, the up bias at 3d
            with_cell_planes() $(var cp : CellPlanes) {
                let qp = cp |> in_(q)
                let kp = cp |> in_(k)
                let vp = cp |> in_(v)
                let attp = cp |> out_(rows * dc)
                let wlp = cp |> in_plain(wlo)
                let gp = cp |> in_(g)
                let up = cp |> in_(u)
                let wp = cp |> in_(wn)
                let gated_h16p = cp |> out_words(int64(nd) * 2l, F16_SENTINEL_BITS)
                var s_win = cp |> cls_set(fixed_array(qp, kp, vp, attp, wlp), @@set_tower_win_attn_cls)
                var s_glu = cp |> cls_set(fixed_array(gp, up, wp, gated_h16p), @@set_tower_bias_gate16_cls)
                var pa_win = GkWinAttnArgs(nheads = uint(WIN_HEADS), hs = uint(WIN_HS), scale = scale)
                var pa_glu = GkBiasGateArgs(d = uint(d), nelem = uint(nd), goff = uint(2 * d), uoff = uint(3 * d))
                let wg4 = int64((nd / 4 + 255) / 256)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_win_attn_cls(raw, h, s_win, pa_win, int64(nslot * WIN_HEADS))
                    enc_tower_bias_gate16_cls(raw, h, s_glu, pa_glu, wg4)
                }
                let att_dev <- cp |> plane_run(attp)
                let gh_dev <- cp |> plane_as(gated_h16p, type<uint16>, nd)
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
                var g_ref := g
                var u_ref := u
                add_bias_rows(g_ref, wn, int64(2 * d), int64(d), int64(nrows))
                add_bias_rows(u_ref, wn, int64(3 * d), int64(d), int64(nrows))
                swiglu(g_ref, u_ref, int64(nd))
                check_rows(t, "actf16_b (the gated hidden) silu(g + bg) . (u + bu) as halves vs the CPU form", g_dev, g_ref, BAR_APPROX, d)
            }
            with_cell_planes() $(var cp : CellPlanes) {
                post_add_rms_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The audio fronts' data movers reproduce their CPU forms: im2col and shuffles as exact halves, one-head restrides with and without bias, clamp+halfword, the exact plane add touching nothing past it, and the qwen3a finish's bias and position rows")]
def test_vkt_tower_front_movers(t0 : T?) {   // nolint:STYLE038 — one flat gate over the Vulkan-only movers: the fixture, one command buffer, the exact oracles and their controls
    t0 |> run("the audio fronts' data movers (im2col, the stem's half im2col, the shuffles and ln + relu on the CUDA cells' case bodies; the one-head restride, clamp + halfword, add plane, the q3a finish's bias passes) == their forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_im2col_cls() || !ensure_tts_im2col16_cls() || !ensure_tower_q3a_shuffle_cls() || !ensure_tower_feat_shuffle_cls()
                    || !ensure_tower_hpad16_cls() || !ensure_tower_hpad16_b_cls() || !ensure_tower_clamp_cvt_cls() || !ensure_tower_bias_act_cls()
                    || !ensure_tower_ln_relu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                im2col_case(t, cp)
                im2col16_case(t, cp)
                q3a_shuffle_case(t, cp)
                feat_shuffle_case(t, cp)
                ln_relu_case(t, cp)
            }
            let d = ROWS_D
            let rows = ROWS_N
            let nd = rows * d
            let hs = d / ROWS_HEADS
            let second_head_base = hs
            let q3_tok = int(QWEN3A_TOK_PER_CHUNK)   // the kernels carry the family's positions a chunk
            let add_n = 4 * d
            var x <- row_fill(nd, 41, 7, 0.05, 2.1)
            let wn <- row_fill((3 + q3_tok) * d, 7, 1, 0.01, -0.6)   // the panel's bias row at 0, the add plane at d, the finish's bias at 2d and its tok position rows at 3d
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(x)
                let wp = cp |> in_(wn)
                let gqp = cp |> out_words(int64(rows * hs) * 2l, F16_SENTINEL_BITS)
                let gpp = cp |> out_words(int64(rows * hs) * 2l, F16_SENTINEL_BITS)
                let cvtp = cp |> out_words(int64(nd) * 2l, F16_SENTINEL_BITS)
                let addp = cp |> io_(x)
                let finp = cp |> io_(x)
                var s_gq = cp |> cls_set(fixed_array(xp, gqp, wp), @@set_tower_hpad16_b_cls)
                var s_gp = cp |> cls_set(fixed_array(xp, gpp), @@set_tower_hpad16_cls)
                var s_cvt = cp |> cls_set(fixed_array(xp, cvtp), @@set_tower_clamp_cvt_cls)
                var s_add = cp |> cls_set(fixed_array(addp, wp), @@set_tower_bias_act_cls)
                var s_fin = cp |> cls_set(fixed_array(finp, wp), @@set_tower_bias_act_cls)
                var pa_gq = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d),
                    soff = uint(second_head_base), boff = uint(second_head_base))
                var pa_gp = GkRestrideArgs(rows = uint(rows), rows_pad = uint(rows), nheads = 1u, hs = uint(hs), hsp = uint(hs), sstride = uint(d),
                    soff = uint(second_head_base))
                var pa_cvt = GkClampArgs(nelem = uint(nd), lo = -1.5, hi = 1.5, scale = 1.0)
                var pa_add = GkBiasActArgs(d = uint(add_n), nelem = uint(add_n), boff = uint(d), act = BIAS_ACT_NONE)
                var pa_fin_b = GkBiasActArgs(d = uint(d), nelem = uint(nd), boff = uint(2 * d), act = BIAS_ACT_NONE)
                var pa_fin_p = GkBiasActArgs(d = uint(q3_tok * d), nelem = uint(nd), boff = uint(3 * d), act = BIAS_ACT_NONE)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tower_hpad16_b_cls(raw, h, s_gq, pa_gq, int64((rows * hs + 255) / 256))
                    enc_tower_hpad16_cls(raw, h, s_gp, pa_gp, int64((rows * hs + 255) / 256))
                    enc_tower_clamp_cvt_cls(raw, h, s_cvt, pa_cvt, int64((nd / 4 + 255) / 256))
                    enc_tower_bias_act_cls(raw, h, s_add, pa_add, int64((add_n / 4 + 255) / 256))
                    enc_tower_bias_act_cls(raw, h, s_fin, pa_fin_b, int64((nd / 4 + 255) / 256))
                    enc_tower_bias_act_cls(raw, h, s_fin, pa_fin_p, int64((nd / 4 + 255) / 256))
                }
                let gq_dev <- cp |> plane_as(gqp, type<uint16>, rows * hs)
                let gp_dev <- cp |> plane_as(gpp, type<uint16>, rows * hs)
                let cvt_dev <- cp |> plane_as(cvtp, type<uint16>, nd)
                let add_dev <- cp |> plane_run(addp)
                let fin_dev <- cp |> plane_run(finp)
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
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The audio fronts' spectral classes match their CPU forms at the approx bar: the DFT magnitude and power arms, three mel-log floor laws, canary's mel normalization with an exact zero tail, depthwise conv and the rel sin-cos table")]
def test_vkt_tower_front_mel(t0 : T?) {
    t0 |> run("the audio fronts' spectral classes (spec_dft, mel_log, mel_norm, dw_conv, rel_sincos) == their CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_spec_dft_cls() || !ensure_tower_mel_log_cls() || !ensure_tower_mel_norm_cls() || !ensure_tower_dw_conv_cls()
                    || !ensure_tower_rel_sincos_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                spec_dft_case(t, cp)
                mel_log_floor_case(t, cp)
                mel_norm_case(t, cp)
                dw_conv_case(t, cp)
                rel_sincos_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Gigaam's device front writes the windowed frames with their zero pads bit for bit, and its clamped-log mel matches the CPU form within 1e-6, a zero frame reading the floor's log and a loud one the ceiling's")]
def test_vkt_tower_ga_mel(t0 : T?) {
    t0 |> run("gigaam's device mel classes (ga_frames, mel_log's clamp form) == the CPU frame loop and the clamped-log form") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_ga_frames_cls() || !ensure_tower_mel_log_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                ga_frames_case(t, cp)
                mel_log_clamp_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Canary's rel-plane FastConformer attention over four 64-wide and two 128-wide heads matches the CPU rel-pos loop with u/v biases at the approx bar; scaled keys and a scaled rel table each land elsewhere")]
def test_vkt_tower_cn_attn_rel_plane(t0 : T?) {
    t0 |> run("the rel-plane FastConformer attention (both head widths) over CPU-built planes == the CPU loop") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_cn_attn_r64_cls() || !ensure_tower_cn_attn_r_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                cn_attn_rel_plane_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the silero VAD's two classes against `_vad_oracle.das`'s references (the kernels' order in f32; the device parts by its fmas,
//! exp, tanh, sqrt and the decoder's subgroup fold, a few ulp a stage, inside the approx bar): the encoder over three chunks,
//! the recurrence at one chunk and five from a nonzero state in place
[test(description = "The silero VAD encoder over three chunks and its LSTM recurrence at one and five chunks from a nonzero state match the `_vad_oracle` references at the approx bar, the state moving off its input")]
def test_vkt_tower_vad(t0 : T?) {
    t0 |> run("the silero VAD's encoder and recurrence == the CPU references") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tower_vad_enc_cls() || !ensure_tower_vad_lstm_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                vad_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the transducer decode's four stamps on the legs the Metal cells run (`test_metal_rnnt_kernels`), at both fixture shapes
//! (`rc_shapes`), against `_rnnt_oracle.das`
[test(description = "The transducer decode's gates, predictor projection, joint window and pick stamps match the `_rnnt_oracle` references on the Metal cells' legs at both fixture shapes, including states that must leave outputs untouched")]
def test_vkt_rnnt(t0 : T?) {
    t0 |> run("the transducer decode's gates, projection, joint and pick == the CPU references") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_rnnt_gates_cls() || !ensure_rnnt_proj_cls() || !ensure_rnnt_joint_cls() || !ensure_rnnt_pick_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                rnnt_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! The CTC head's pick (`CtcPick` on `GkCtcPick`) on the Metal cell's `ctc_pick_cases` (three widths, a tie, NaN pad slots, an
//! all-NaN row picking class 0) against the CPU row loop: every record written, each class exact, each plog within 1e-5
//! relative; the control a poisoned bias element moving a pick or a plog off the reference.
[test(description = "The CTC pick writes every row's record with the CPU loop's exact class (a tie taking the lower class, an all-NaN row class 0) and each plog within 1e-5 relative, and a poisoned bias element moves the plogs")]
def test_vkt_ctc_pick(t0 : T?) {
    t0 |> run("the CTC head's pick == the CPU row loop, a tie picking the lower class, a NaN row picking class 0") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_ctc_pick_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                ctc_pick_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! The Bergamot step's three stamps (`NmtEmbed`, `NmtSsru`, `NmtPick`) on the legs the Metal cells run (`test_metal_nmt_kernels`),
//! against `_nmt_oracle.das`
[test(description = "The Bergamot step's embed rows bit for bit, SSRU cell within 1e-6 relative with layer 0 kept, and greedy pick records exact at vocabularies 200, 1000 and 32000 match the `_nmt_oracle` references, each with a poisoned-input control")]
def test_vkt_nmt(t0 : T?) {
    t0 |> run("the Bergamot step's embed, SSRU cell and pick == their CPU references") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_nmt_embed_cls() || !ensure_nmt_ssru_cls() || !ensure_nmt_pick_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            vkt_nmt_embed(t)
            vkt_nmt_ssru(t)
            vkt_nmt_pick(t)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the embed rows bit for bit in both forms: the encoder's over four ids at positions 0..3 (a `NMT_NO_TOKEN` row among them) and
//! the step's, one row whose token and position the state words name; the plane sized to the dispatch's 256-lane grid, its slack
//! under the sentinel; the control a poisoned table element of a gathered row, through `check_exceeds`
[unused_argument(t)]
def private vkt_nmt_embed(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        for (step in fixed_array(false, true)) {
            let tag = "nmt_embed {step ? "step" : "encode"}"
            var inscope fx <- nmt_embed_fix(8)
            var inscope ids <- step ? [5u, 2u, 0u, 1u] : [3u, NMT_NO_TOKEN, 0u, 4u]
            let rows = step ? 1 : 4
            let n = rows * NMT_FIX_D
            var inscope want_ids <- step ? [2u] : [3u, NMT_NO_TOKEN, 0u, 4u]
            let scale = sqrt(float(NMT_FIX_D))
            with_cell_planes() $(var cp : CellPlanes) {
                let ip = cp |> in_plain(ids)
                let wp = cp |> in_(fx.wn)
                let pp = cp |> in_(fx.pos)
                let xp = cp |> out_(n, 0, nmt_slack(n))
                var s = cp |> cls_set(fixed_array(ip, wp, pp, xp), @@set_nmt_embed_cls)
                var ka = GkNmtEmbedArgs(d = uint(NMT_FIX_D), rows = uint(rows), emb_off = uint(NMT_FIX_LEAD), scale = scale, from_state = step ? 1u : 0u)
                for (leg in range(2)) {
                    if (leg == 1) {
                        fx.wn[NMT_FIX_LEAD + int(want_ids[0]) * NMT_FIX_D + 3]++
                        cp |> plane_upload(wp, fx.wn)
                        cp |> plane_refill(xp)
                    }
                    cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                        enc_nmt_embed_cls(raw, h, s, ka, int64((n + 255) / 256))
                    }
                    var inscope got <- cp |> plane_run(xp)
                    var inscope fresh <- nmt_embed_fix(8)
                    var inscope want <- nmt_embed_ref(fresh, want_ids, step ? 5 : 0, scale)
                    if (leg == 0) {
                        check_exact(t, "{tag}: the rows", got, want)
                    } else {
                        check_exceeds(t, "{tag}: the control - a poisoned table element of a gathered row moves the rows", got, want, BAR_EXACT)
                    }
                    var inscope whole <- cp |> plane_whole(xp)
                    nmt_check_slack(t, "{tag} leg {leg}", whole, n)
                }
            }
        }
    }
}

//! the SSRU cell on layer 1 of two cells at 1e-6 relative (the device's exp against the CPU's), at a width off the thread grid: the
//! cells after it - moved off their input past the bar - and the output row; layer 0's cells and the cell plane's slack kept, the
//! output row's slack under the sentinel; the control a poisoned forget-bias element moving the rows, through `check_exceeds`
[unused_argument(t)]
def private vkt_nmt_ssru(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "nmt_ssru"
        var inscope fx <- nmt_ssru_fix()
        var inscope want <- nmt_ssru_ref(fx)
        let bar = Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true)
        let slack = nmt_slack(NMT_FIX_D)
        var inscope cell_in <- at_base(fx.cell, 0, slack)
        let moved_at = NMT_FIX_D + 5
        t |> success(moved_off(want.cell, fx.cell, moved_at, bar), "{tag}: the reference's layer-1 cell {moved_at} sits past the bar off its input ({want.cell[moved_at]} from {fx.cell[moved_at]})")
        with_cell_planes() $(var cp : CellPlanes) {
            let hp = cp |> in_(fx.h)
            let wp = cp |> in_(fx.wn)
            let cellp = cp |> io_(fx.cell, 0, slack)
            let op = cp |> out_(NMT_FIX_D, 0, slack)
            var s = cp |> cls_set(fixed_array(hp, wp, cellp, op), @@set_nmt_ssru_cls)
            var ka = GkNmtSsruArgs(d = uint(NMT_FIX_D), bf_off = uint(NMT_FIX_LEAD), cell_off = uint(NMT_FIX_D))
            for (leg in range(2)) {
                if (leg == 1) {
                    fx.wn[NMT_FIX_LEAD + 5] += 2.0
                    cp |> plane_upload(wp, fx.wn)
                    cp |> plane_upload(cellp, fx.cell)
                    cp |> plane_refill(op)
                }
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_nmt_ssru_cls(raw, h, s, ka, int64((NMT_FIX_D + 255) / 256))
                }
                var inscope o <- cp |> plane_run(op)
                var inscope cell <- cp |> plane_run(cellp)
                if (leg == 0) {
                    check_rows(t, "{tag}: the output row", o, want.o, bar, FLAT_ROWS)
                    check_rows(t, "{tag}: the cells", cell, want.cell, bar, FLAT_ROWS)
                    t |> success(moved_off(cell, fx.cell, moved_at, bar), "{tag}: the device's layer-1 cell {moved_at} moved off its input ({cell[moved_at]} from {fx.cell[moved_at]})")
                } else {
                    check_exceeds(t, "{tag}: the control - a poisoned forget bias moves the output row", o, want.o, bar)
                }
                var inscope ow <- cp |> plane_whole(op)
                nmt_check_slack(t, "{tag} output leg {leg}", ow, NMT_FIX_D)
                var inscope cw <- cp |> plane_whole(cellp)
                nmt_check_kept(t, "{tag} cells leg {leg}: layer 0 and the slack", cw, cell_in, NMT_FIX_D, NMT_FIX_D)
            }
        }
    }
}

//! the greedy pick at vocabularies of 200 (under one 256-lane stride), 1000 and 32000 over `nmt_pick_cases`: the state words and
//! the whole record plane exact, `NMT_PICK_SLACK` words past the cap among them; the control, on every case,
//! `nmt_pick_poisoned` - the device's words the poisoned case's reference's and off the clean case's
[unused_argument(t)]
def private vkt_nmt_pick(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        for (nv in fixed_array(200, 1000, 32000)) {
            var inscope cases <- nmt_pick_cases(nv)
            for (c in cases) {
                let tag = "nmt_pick nv {nv} {c.tag}"
                var inscope pc <- nmt_pick_poisoned(c)
                let nrec = int(c.cap) + NMT_PICK_SLACK
                with_cell_planes() $(var cp : CellPlanes) {
                    let lp = cp |> in_(c.lg)
                    let wp = cp |> in_(c.wb)
                    let sp = cp |> out_words(int64(NMT_ST_WORDS) * 4l, 0u)
                    let tp = cp |> out_words(int64(nrec) * 4l, NMT_NO_TOKEN)
                    var s = cp |> cls_set(fixed_array(lp, wp, sp, tp), @@set_nmt_pick_cls)
                    var ka = GkNmtPickArgs(nv = uint(nv), boff = uint(NMT_FIX_LEAD), eos = c.eos, cap = c.cap)
                    for (leg in range(2)) {
                        if (leg == 1) {
                            cp |> plane_upload(wp, pc.wb)
                            cp |> plane_refill(tp)
                        }
                        cp |> plane_upload(sp, leg == 0 ? c.st : pc.st)
                        cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                            enc_nmt_pick_cls(raw, h, s, ka, 1l)
                        }
                        var inscope st <- cp |> plane_as(sp, 0u, int(NMT_ST_WORDS))
                        var inscope toks <- cp |> plane_as(tp, 0u, nrec)
                        if (leg == 0) {
                            nmt_pick_check(t, tag, st, toks, c)
                        } else {
                            nmt_pick_check(t, "{tag} poisoned", st, toks, pc)
                            nmt_pick_moved(t, tag, st, toks, c)
                        }
                    }
                }
            }
        }
    }
}

let private NMT_ATTN_HEADS = 4
let private NMT_ATTN_ROWS = 3
let private NMT_ATTN_KV_LEAD = 37   //! the halves ahead of the K/V run: a kernel reading from element 0 instead of `koff` misses
let private NMT_ATTN_Q_LEAD = 7     //! the floats ahead of the q bias row in `wn`

//! The Bergamot attention pair (`NmtAttnPart` + `NmtAttnComb`, head width `pa.hs`) at heads of 32, 48 and 64 over 300 keys (two
//! chunks) and 150 against `wdec_attn_ref` over the same halves; q rows strided twice the width, the K/V run behind a lead, the
//! partial and output slack under the sentinel. The control the keys scaled twice, through `check_exceeds`.
[test(description = "The Bergamot attention part+combine pair at head widths 32, 48 and 64 over 300 keys (two chunks) and 150 matches `wdec_attn_ref` at the approx bar with the K/V run behind a lead, slack kept, doubled key scale landing elsewhere")]
def test_vkt_nmt_attn(t0 : T?) {
    t0 |> run("the Bergamot attention pair at heads of 32, 48 and 64 == the CPU attention") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_nmt_attn_part_cls() || !ensure_nmt_attn_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            for (hs in fixed_array(32, 48, 64)) {
                for (len in fixed_array(300, 150)) {
                    vkt_nmt_attn(t, hs, len)
                }
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one attention pair dispatch at head width `hs` over `len` keys, its compare and control
[unused_argument(t, hs, len)]
def private vkt_nmt_attn(t : T?; hs, len : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "nmt_attn hs {hs} keys {len}"
        let heads = NMT_ATTN_HEADS
        let rows = NMT_ATTN_ROWS
        let d = heads * hs
        let dim = 2 * d
        let kvn = heads * len * hs
        let nch = (len + int(WDEC_CHUNK) - 1) / int(WDEC_CHUNK)
        let scale = 1.0 / sqrt(float(hs))
        let q <- row_fill(rows * dim, 31, 5, 0.05, 2.3)
        let wn <- row_fill(NMT_ATTN_Q_LEAD + d, 11, 3, 0.02, 0.9)
        let kh <- [for (i in range(NMT_ATTN_KV_LEAD + kvn)); uint16(f32_to_f16(float((i * 29) % 41) * 0.05 - 1.0))]
        let vh <- [for (i in range(NMT_ATTN_KV_LEAD + kvn)); uint16(f32_to_f16(float((i * 17) % 23) * 0.08 - 0.9))]
        let krun <- copy_span(kh, NMT_ATTN_KV_LEAD, kvn)
        let vrun <- copy_span(vh, NMT_ATTN_KV_LEAD, kvn)
        let y_ref <- wdec_attn_ref(q, krun, vrun, wn, rows, dim, d, heads, hs, len, len, NMT_ATTN_Q_LEAD, false, scale)
        let ys_ref <- wdec_attn_ref(q, krun, vrun, wn, rows, dim, d, heads, hs, len, len, NMT_ATTN_Q_LEAD, false, scale * 2.0)
        let npy = nch * heads * rows * hs
        with_cell_planes() $(var cp : CellPlanes) {
            let qp = cp |> in_(q)
            let kp = cp |> in_plain(kh)
            let vp = cp |> in_plain(vh)
            let wp = cp |> in_(wn)
            let pmsp = cp |> out_(nch * heads * rows * 2)
            let pyp = cp |> out_(npy, 0, 128)
            let yp = cp |> out_(rows * d, 0, 64)
            var s_part = cp |> cls_set(fixed_array(qp, kp, vp, pmsp, pyp, wp), @@set_nmt_attn_part_cls)
            var s_comb = cp |> cls_set(fixed_array(pmsp, pyp, yp), @@set_nmt_attn_comb_cls)
            var pa = GkWdecAttnArgs(dim = uint(dim), tstride = uint(len), len0 = uint(len), causal = 0u, nheads = uint(heads), nrows = uint(rows),
                koff = uint(NMT_ATTN_KV_LEAD), voff = uint(NMT_ATTN_KV_LEAD), qboff = uint(NMT_ATTN_Q_LEAD), hs = uint(hs), scale = scale)
            var pc = GkWdecCombArgs(dim = uint(d), nchunks = uint(nch), nheads = uint(heads), nrows = uint(rows), hs = uint(hs))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_nmt_attn_part_cls(raw, h, s_part, pa, int64(nch * heads * rows))
                enc_nmt_attn_comb_cls(raw, h, s_comb, pc, int64(heads * rows))
            }
            let y <- cp |> plane_run(yp)
            check_rows(t, "{tag}: the rows against the CPU attention", y, y_ref, BAR_APPROX, d, hs)
            check_exceeds(t, "{tag}: the control - the keys scaled twice move the rows", y, ys_ref, BAR_APPROX)
            let yw <- cp |> plane_whole(yp)
            nmt_check_slack(t, "{tag} the output rows", yw, rows * d)
            let pw <- cp |> plane_whole(pyp)
            nmt_check_slack(t, "{tag} the partial rows", pw, npy)
        }
    }
}
