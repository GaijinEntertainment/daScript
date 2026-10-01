options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_kernels

require dastest/testing_boost public
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require _tts_kernel_oracles   // nolint:STYLE030,LINT019 — the in-test CPU forms the cells share, inside the vulkan arms alone
require dasllama/dasllama_math
require dasllama/dasllama_tts_blocks   // nolint:STYLE030,LINT019 — attention_rows, leaky_relu: the CPU oracles, inside the vulkan arms alone
require daslib/jobque_boost
require math   // nolint:STYLE030,LINT019 — sqrt, tanh: the oracles inside the vulkan arms alone; the no-Vulkan lane sees it unused

//! The TTS tower's Vulkan kernel classes against their CPU oracles (a Vulkan device, else the
//! cells skip): the f32-exact tile GEMM the front end feeds on, in both its stamps, at shapes off
//! every tile and workgroup multiple, and the PL-BERT seat's row classes - the embedding gather, the
//! tanh GELU, the leaky ReLU and the bidirectional attention - and the predictor and decoder row classes - the concat,
//! the duration sigmoid sums, the column statistics with the fused AdaIN stamps and the residual join - with the
//! poisoned-element control on every compare.

//! y[rows x d] = x[rows x n] w[d x n]^T (+ b) summed in k order - the tile's own order, so the compare is tight
def private f32_gemm_ref(x, w, b : array<float>; rows, d, n, wstride, xstride : int; bias : bool) : array<float> {
    var y : array<float>
    y |> resize(rows * d)
    for (p in range(rows)) {
        for (o in range(d)) {
            var acc = 0.0
            for (k in range(n)) {
                acc += x[p * xstride + k] * w[o * wstride + k]
            }
            y[p * d + o] = bias ? acc + b[o] : acc
        }
    }
    return <- y
}

let private G_ROWS = 37     //! positions: off the 64-position tile
let private G_D = 100       //! weight rows: off the 64-row tile, so the last tile masks 28 rows
let private G_N = 96        //! K: three 32-wide steps, the slab's lattice
let private G_WSTRIDE = 128 //! the slab's padded row: the pad past n never reads
let private G_XSTRIDE = 96

[test]
def test_vkt_f32_gemm(t0 : T?) {
    t0 |> run("the f32 tile GEMM, plain and biased, == the k-ordered fma sum at off-tile shapes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_f32_gemm_cls() || !ensure_f32_gemm_b_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let rows = G_ROWS
            let d = G_D
            let n = G_N
            let x <- row_fill(rows * G_XSTRIDE, 41, 7, 0.05, 2.1)
            var w <- row_fill(d * G_WSTRIDE, 17, 3, 0.04, 1.9)
            let b <- row_fill(d, 29, 11, 0.06, 2.8)
            for (o in range(d)) {   //! the pad past n carries garbage: a read past n lands it in the sum
                for (k in range(n, G_WSTRIDE)) {
                    w[o * G_WSTRIDE + k] = POISON
                }
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(x)
                let wp = cp |> in_(w)
                let bp = cp |> in_(b)
                let yp = cp |> out_(rows * d)
                let ybp = cp |> out_(rows * d)
                var s_plain = cp |> cls_set(fixed_array(wp, xp, yp), @@set_f32_gemm_cls)
                var s_bias = cp |> cls_set(fixed_array(wp, xp, ybp, bp), @@set_f32_gemm_b_cls)
                var pa = F32GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, wstride = uint(G_WSTRIDE), boff = 0u,
                    xoff = 0u, xstride = uint(G_XSTRIDE), yoff = 0u, ystride = uint(d))
                let wgs = f32_gemm_wgs(int64(rows), int64(d))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_f32_gemm_cls(raw, h, s_plain, pa, wgs)
                    enc_f32_gemm_b_cls(raw, h, s_bias, pa, wgs)
                }
                let y_dev <- cp |> plane_run(yp)
                check_rows(t, "f32_gemm plain stamp vs the k-ordered sum", y_dev, f32_gemm_ref(x, w, b, rows, d, n, G_WSTRIDE, G_XSTRIDE, false), BAR_APPROX, d)
                check_rows(t, "f32_gemm_b biased stamp vs the sum plus the bias row", cp |> plane_run(ybp), f32_gemm_ref(x, w, b, rows, d, n, G_WSTRIDE, G_XSTRIDE, true), BAR_APPROX, d)
                t |> equal(nan_count(y_dev), 0, "every output element is stored (no sentinel survives)")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private TTS_REL = 1e-5      //! the f32 row classes' bar: the device's reassociation and transcendentals sit well inside it, the f16 LUT GELU does not

let private GA_C = 72       //! the row width: a row straddles the 256-element workgroups
let private GA_ROWS = 23
let private GA_VOCAB = 40
let private GA_WORD = 5     //! the word rows' element base in the slab
let private GA_OUT = 9      //! the output rows' element base: the prefix before it is never written

[test]
def test_vkt_row_gather(t0 : T?) {
    t0 |> run("the embedding gather == the CPU word + position + type sum, exact, at element bases off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_row_gather_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let c = GA_C
            let rows = GA_ROWS
            let total = rows * c
            let pos_off = GA_WORD + GA_VOCAB * c
            let type_off = pos_off + rows * c
            var slab <- row_fill(type_off + c, 37, 5, 0.03, 1.4)
            var ids <- [for (r in range(rows)); (r * 7 + 3) % GA_VOCAB]
            ids[1] = ids[0]   //! two positions on one word: only the position rows part them
            with_cell_planes() $(var cp : CellPlanes) {
                let sp = cp |> in_(slab)
                let ip = cp |> in_plain(ids)
                let op = cp |> out_(total, GA_OUT)
                var s = cp |> cls_set(fixed_array(sp, ip, op), @@set_tts_row_gather_cls)
                var pa = TtsGatherArgs(c = uint(c), rows = uint(rows), word_off = uint(GA_WORD), pos_off = uint(pos_off), type_off = uint(type_off),
                    out_off = uint(GA_OUT), adds = 1u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_row_gather_cls(raw, h, s, pa, int64(total))
                }
                let got <- cp |> plane_run(op)
                let want <- gather_ref(slab, ids, c, GA_WORD, pos_off, type_off)
                check_rows(t, "tts_row_gather vs the CPU word + position + type sum", got, want, BAR_EXACT, c)
                to_log(LOG_INFO, "tts_row_gather: the widest miss's row reads id {ids[maxdiff_at(got, want) / c]}\n")
                var zero_ids : array<int>
                zero_ids |> resize(rows)
                t |> success(mismatch_exact(got, gather_ref(slab, zero_ids, c, GA_WORD, pos_off, type_off)) > 0, "tts_row_gather control: the rows follow their ids (the id-0 sum misses the device)")
                t |> success(mismatch_exact(subarray(got, range(c)), subarray(got, range(c, 2 * c))) > 0, "tts_row_gather control: two positions on one word differ by their position rows")
                t |> success(outside_run(cp |> plane_whole(op), GA_OUT, total, NAN_SENTINEL), "tts_row_gather: the prefix before out_off keeps its sentinel")
                cp |> plane_refill(op)
                pa.adds = 0u
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_row_gather_cls(raw, h, s, pa, int64(total))
                }
                let got_plain <- cp |> plane_run(op)
                var want_plain : array<float>
                want_plain |> resize(total)
                gather_rows(want_plain, subarray(slab, range(GA_WORD, pos_off)), ids, int64(c))
                check_rows(t, "tts_row_gather adds = 0 vs the word row alone", got_plain, want_plain, BAR_EXACT, c)
                t |> success(mismatch_exact(got_plain, want) > 0, "tts_row_gather adds = 0 control: the position rows are out of the sum")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private EL_OFF = 7      //! the mapped run's element base: the prefix before it stays as it was
let private EL_N = 5203     //! five whole 1024-element workgroups and 83 more: the last live invocation maps the three-element tail
let private EL_SLACK = 5    //! elements past the run, left as they were
let private EL_SLOPE = 0.2

//! the elementwise cells' plane: x[EL_OFF] and x[EL_OFF + 1] at +-30, past the tanh clamp
def private elem_fixture() : array<float> {
    var x <- row_fill(EL_OFF + EL_N + EL_SLACK, 53, 1, 0.11, 5.3)
    x[EL_OFF] = 30.0
    x[EL_OFF + 1] = -30.0
    return <- x
}

[test]
def test_vkt_gelu_tanh(t0 : T?) {
    t0 |> run("the tanh GELU == the CPU gelu at the f32 bar, the run's tail and the tanh clamp included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_gelu_tanh_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            var x <- elem_fixture()
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x)
                var s = cp |> cls_set(fixed_array(xp), @@set_tts_gelu_tanh_cls)
                var pa = TtsElemArgs(nelem = uint(EL_N), off = uint(EL_OFF), slope = 0.0)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_gelu_tanh_cls(raw, h, s, pa, int64(EL_N))
                }
                let got <- cp |> plane_run(xp)
                var want := x
                gelu(unsafe(addr(want[EL_OFF])), int64(EL_N))
                check_rows(t, "tts_gelu_tanh vs the CPU gelu", copy_span(got, EL_OFF, EL_N), copy_span(want, EL_OFF, EL_N), Bar(rel = TTS_REL, poison = 1e-4), FLAT_ROWS)
                t |> success(outside_run(got, EL_OFF, EL_N, x), "tts_gelu_tanh: the elements outside the run are untouched")
                var lut_off = 0
                for (i in range(EL_OFF, EL_OFF + EL_N)) {
                    if (!bar_ok(tower_gelu_tanh(x[i]), want[i], TTS_REL)) {
                        lut_off++
                    }
                }
                t |> success(lut_off > 0, "tts_gelu_tanh control: the bar tells the f32 tanh form from the f16 LUT form ({lut_off} of {EL_N} apart)")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkt_leaky(t0 : T?) {
    t0 |> run("the leaky ReLU == the CPU leaky_relu, exact, the run's tail included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_leaky_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let x <- elem_fixture()
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x)
                var s = cp |> cls_set(fixed_array(xp), @@set_tts_leaky_cls)
                var pa = TtsElemArgs(nelem = uint(EL_N), off = uint(EL_OFF), slope = EL_SLOPE)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_leaky_cls(raw, h, s, pa, int64(EL_N))
                }
                let got <- cp |> plane_run(xp)
                var run <- [for (i in range(EL_N)); x[EL_OFF + i]]
                leaky_relu(run, int64(EL_N), EL_SLOPE)
                check_rows(t, "tts_leaky vs the CPU leaky_relu", copy_span(got, EL_OFF, EL_N), run, BAR_EXACT, FLAT_ROWS)
                t |> success(outside_run(got, EL_OFF, EL_N, x), "tts_leaky: the elements outside the run are untouched")
                let neg = EL_OFF + 1
                t |> success(got[neg] == x[neg] * EL_SLOPE && got[neg] != x[neg], "tts_leaky control: a negative element is scaled by the slope")
                t |> success(got[EL_OFF] == x[EL_OFF], "tts_leaky control: a positive element passes unchanged")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one attention shape at element bases off zero, over the keys as they are and with the last key row zeroed, against `attention_rows`
def private attn_arm(t : T?; rows, heads, hs : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_attn t{rows} {heads}x{hs}"
        let d = heads * hs
        let n = rows * d
        let qoff = 5
        let koff = 7
        let voff = 11
        let ooff = 13
        var q <- row_fill(n, 41, 7, 0.02, 0.95)
        var k <- row_fill(n, 17, 3, 0.02, 0.97)
        var v <- row_fill(n, 53, 11, 0.04, 1.9)
        var kz := k
        for (e in range(d)) {
            kz[(rows - 1) * d + e] = 0.0
        }
        with_cell_planes() $(var cp : CellPlanes) {
            let qp = cp |> in_(q, qoff)
            let kp = cp |> in_(k, koff)
            let kzp = cp |> in_(kz, koff)
            let vp = cp |> in_(v, voff)
            let op = cp |> out_(n, ooff)
            let ozp = cp |> out_(n, ooff)
            var s = cp |> cls_set(fixed_array(qp, kp, vp, op), @@set_tts_attn_cls)
            var sz = cp |> cls_set(fixed_array(qp, kzp, vp, ozp), @@set_tts_attn_cls)
            var pa = TtsAttnArgs(t = uint(rows), d = uint(d), hs = uint(hs), heads = uint(heads), scale = 1.0 / sqrt(float(hs)),
                qoff = uint(qoff), koff = uint(koff), voff = uint(voff), ooff = uint(ooff))
            let wgs = tts_attn_wgs(int64(rows), int64(heads))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_attn_cls(raw, h, s, pa, wgs)
                enc_tts_attn_cls(raw, h, sz, pa, wgs)
            }
            let got <- cp |> plane_run(op)
            let gotz <- cp |> plane_run(ozp)
            var want : array<float>
            var wantz : array<float>
            attention_rows(q, k, v, int64(rows), int64(d), int64(heads), want)
            attention_rows(q, kz, v, int64(rows), int64(d), int64(heads), wantz)
            check_rows(t, "{tag} vs the CPU attention_rows", got, want, Bar(rel = TTS_REL, poison = 1e-4), d, hs)
            check_rows(t, "{tag} the last key zeroed vs the CPU attention_rows", gotz, wantz, Bar(rel = TTS_REL, poison = 1e-4), d, hs)
            var moved = 0
            for (e in range(d)) {
                if (!bar_ok(got[e], gotz[e], TTS_REL)) {
                    moved++
                }
            }
            t |> success(moved > 0, "{tag} control: zeroing the last key moves the first query row (every key is read, not a causal prefix)")
            t |> success(outside_run(cp |> plane_whole(op), ooff, n, NAN_SENTINEL), "{tag}: the prefix before ooff keeps its sentinel")
        }
    }
}

[test]
def test_vkt_attn(t0 : T?) {
    t0 |> run("the bidirectional attention == the CPU attention_rows at hs 64 and 128, one and two keys an invocation") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            with_dasllama_jobque_() {   //! attention_rows threads its heads
                attn_arm(t, 37, 4, 64)
                attn_arm(t, 130, 2, 128)
                attn_arm(t, 512, 2, 64)   //! the staged score row full: every invocation scores two keys
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private CC_ROWS = 37
let private CC_C = 40
let private CC_RC = 13      //! the style row's columns: the duration encoder appends one row to every position
let private CC_F0S = 3      //! the F0 column's row stride
let private CC_NS = 2       //! the noise column's row stride
let private CC_WIDTH = 48   //! the padded arm's width: six zero columns past x, f0 and n

[test]
def test_vkt_concat(t0 : T?) {
    t0 |> run("the concat rows == the CPU concat, exact: one style row broadcast, then f0 and n at strides and a zero pad") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_concat_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let rows = CC_ROWS
            let c = CC_C
            let wr = c + CC_RC
            let x <- row_fill(rows * c, 41, 7, 0.05, 2.1)
            let style <- row_fill(CC_RC, 29, 11, 0.06, 2.8)
            let f0 <- row_fill(rows * CC_F0S, 17, 3, 0.04, 1.9)
            let nd <- row_fill(rows * CC_NS, 53, 1, 0.11, 5.3)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(x)
                let sp = cp |> in_(style)
                let fp = cp |> in_(f0)
                let np = cp |> in_(nd)
                let orp = cp |> out_(rows * wr)
                let opp = cp |> out_(rows * CC_WIDTH)
                var s_res = cp |> cls_set(fixed_array(xp, sp, fp, np, orp), @@set_tts_concat_cls)
                var s_pad = cp |> cls_set(fixed_array(xp, xp, fp, np, opp), @@set_tts_concat_cls)
                var pr = TtsConcatArgs(c = uint(c), rc = uint(CC_RC), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(wr), rows = uint(rows))
                var pp = TtsConcatArgs(c = uint(c), rc = uint(CC_RC), rs = uint(CC_RC), f0s = uint(CC_F0S), ns = uint(CC_NS), with_res = 0u,
                    width = uint(CC_WIDTH), rows = uint(rows))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_concat_cls(raw, h, s_res, pr, int64(rows * wr))
                    enc_tts_concat_cls(raw, h, s_pad, pp, int64(rows * CC_WIDTH))
                }
                let got_p <- cp |> plane_run(opp)
                check_rows(t, "tts_concat with_res rs 0 vs the CPU concat", cp |> plane_run(orp), concat_ref(x, style, f0, nd, rows, c, CC_RC, 0, 0, 0, wr, true), BAR_EXACT, wr)
                check_rows(t, "tts_concat f0/n at strides, zero pad vs the CPU concat", got_p, concat_ref(x, style, f0, nd, rows, c, CC_RC, CC_RC, CC_F0S, CC_NS, CC_WIDTH, false), BAR_EXACT, CC_WIDTH)
                let want_s1 <- concat_ref(x, style, f0, nd, rows, c, CC_RC, CC_RC, 1, 1, CC_WIDTH, false)
                t |> success(mismatch_exact(got_p, want_s1) > 0, "tts_concat control: the f0 and n columns follow their strides")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private SG_T = 37
let private SG_NB = 50      //! the duration bins summed
let private SG_LS = 64      //! the logit rows' stride: the columns past SG_NB carry POISON
let private SG_SPEED = 1.3

[test]
def test_vkt_sigsum(t0 : T?) {
    t0 |> run("the sigmoid sums == the CPU raw durations over 50 of 64 columns at speed 1.3") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_sigsum_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            var logits <- row_fill(SG_T * SG_LS, 41, 7, 0.1, 4.8)
            for (r in range(SG_T)) {
                for (j in range(SG_NB, SG_LS)) {
                    logits[r * SG_LS + j] = POISON
                }
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let lp = cp |> in_(logits)
                let rp = cp |> out_(SG_T)
                var s = cp |> cls_set(fixed_array(lp, rp), @@set_tts_sigsum_cls)
                var pa = TtsSigSumArgs(t = uint(SG_T), nb = uint(SG_NB), ls = uint(SG_LS), speed = SG_SPEED)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_sigsum_cls(raw, h, s, pa, int64(SG_T))
                }
                let got <- cp |> plane_run(rp)
                check_rows(t, "tts_sigsum vs the CPU sum", got, sigsum_ref(logits, SG_T, SG_NB, SG_LS, SG_SPEED), BAR_APPROX, 1)
                let want_all <- sigsum_ref(logits, SG_T, SG_LS, SG_LS, SG_SPEED)
                t |> success(mismatch_approx(got, want_all) > 0, "tts_sigsum control: the columns past nb stay out of the sum")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the channel means, then the sums of squared deviations from them, of x [rows][c], in double
def private colstats_ref(x : array<float>; rows, c : int) : array<float> {
    var st : array<float>
    st |> resize(2 * c)
    for (ch in range(c)) {
        var s = 0.0lf
        for (r in range(rows)) {
            s += double(x[r * c + ch])
        }
        let mean = s / double(rows)
        var q = 0.0lf
        for (r in range(rows)) {
            let dv = double(x[r * c + ch]) - mean
            q += dv * dv
        }
        st[ch] = float(mean)
        st[c + ch] = float(q)
    }
    return <- st
}

//! the four column-stats passes over `xp` [rows][c] into `stp`'s stats rows; its arguments untyped, as a vulkan-less build compiles it
def private colstats_enc(raw; var h; cp; xp, stp, rows, c : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nblk = tts_stats_blocks(int64(rows))
        var pa = TtsStatsArgs(t = uint(rows), c = uint(c), nblk = uint(nblk), poff = uint(2 * c))
        var s1 = cp |> cls_set(fixed_array(xp, stp), @@set_tts_colpart_sum_cls)
        enc_tts_colpart_sum_cls(raw, h, s1, pa, nblk)
        var s2 = cp |> cls_set(fixed_array(stp, stp), @@set_tts_colreduce_mean_cls)
        enc_tts_colreduce_mean_cls(raw, h, s2, pa, int64(c))
        var s3 = cp |> cls_set(fixed_array(xp, stp, stp), @@set_tts_colpart_sq_cls)
        enc_tts_colpart_sq_cls(raw, h, s3, pa, nblk)
        var s4 = cp |> cls_set(fixed_array(stp, stp), @@set_tts_colreduce_sq_cls)
        enc_tts_colreduce_sq_cls(raw, h, s4, pa, int64(c))
    }
}

//! the stats plane's floats for `rows` x `c`: the two stats rows and the block partials
def private colstats_floats(rows, c : int) : int {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return int(tts_stats_floats(int64(rows), int64(c)))
    } else {
        return 0
    }
}

def private ensure_colstats : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return ensure_tts_colpart_sum_cls() && ensure_tts_colreduce_mean_cls() && ensure_tts_colpart_sq_cls() && ensure_tts_colreduce_sq_cls()
    } else {
        return false
    }
}

//! one statistics shape: x [rows][c] off a mean, the stats plane under a NaN fill
def private colstats_arm(t : T?; rows, c : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = "tts_colstats t{rows} c{c}"
        let x <- row_fill(rows * c, 53, 11, 0.05, 1.3)
        with_cell_planes() $(var cp : CellPlanes) {
            let xp = cp |> in_(x)
            let stp = cp |> out_(colstats_floats(rows, c))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                colstats_enc(raw, h, cp, xp, stp, rows, c)
            }
            let got <- subarray(cp |> plane_run(stp), range(2 * c))
            let want <- colstats_ref(x, rows, c)
            // row 0 the means, row 1 the squared deviations; a sum of 300 of them sits past the relative bar's reach of 0.5, so the poison is a percent
            check_rows(t, "{tag} means and squared deviations vs the double-precision sums", got, want, Bar(rel = APPROX_REL, poison = 0.01, poison_scaled = true), c)
            t |> success(!approx(got[c - 1], want[c - 1] + 0.5), "{tag}: the bar reds a poisoned mean")
        }
    }
}

[test]
def test_vkt_colstats(t0 : T?) {
    t0 |> run("the column statistics == the double-precision mean and squared deviations, one and two strides a lane") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_colstats()) {
                t |> skip("no Vulkan device")
                return
            }
            colstats_arm(t, 45, 72)
            colstats_arm(t, 300, 20)     //! three blocks, the last one short
            colstats_arm(t, 1000, 300)   //! eight blocks; lanes 0..43 take a second channel
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private AD_T = 45
let private AD_C = 72
let private AD_SD = 16      //! the style vector's width
let private AD_EPS = 1e-5
let private AD_XOFF = 5     //! each plane's element base: the prefix before it is never written
let private AD_YOFF = 9
let private AD_HOFF = 7
let private AD_GWOFF = 3    //! the slab's norm scale row, then its shift row and Snake's alpha row, POISON between

//! an AdaIN over AD_C channels from an AD_SD-wide style: fc w [2c][sd] and b, the norm's scale and shift rows
def private make_adain() : TtsAdaIn {
    var a = TtsAdaIn()
    a.fc.nin = int64(AD_SD)
    a.fc.nout = int64(2 * AD_C)
    a.fc.w <- row_fill(2 * AD_C * AD_SD, 37, 5, 0.004, 0.19)
    a.fc.b <- row_fill(2 * AD_C, 29, 11, 0.01, 0.45)
    a.norm.w <- row_fill(AD_C, 17, 3, 0.01, -0.5)
    a.norm.b <- row_fill(AD_C, 53, 1, 0.02, 0.9)
    return <- a
}

struct private AdainFixture {
    x : array<float>
    style_fc : array<float>   //! fc(style): gamma [c], then beta [c]
    slab : array<float>       //! the norm rows and alpha at AD_GWOFF, `gboff`, `aoff`
    want_leaky : array<float>
    want_snake : array<float>
    gboff : int
    aoff : int
}

def private adain_fixture() : AdainFixture {
    let c = AD_C
    var f = AdainFixture()
    var a <- make_adain()
    var style <- row_fill(AD_SD, 23, 9, 0.06, 2.8)
    var alpha <- row_fill(c, 23, 9, 0.02, -0.3)
    f.x <- row_fill(AD_T * c, 41, 7, 0.05, 2.1)
    put_run(f.slab, a.norm.w, AD_GWOFF)
    f.gboff = put_run(f.slab, a.norm.b)
    f.aoff = put_run(f.slab, alpha, 4)
    f.slab |> push_from(poison_plane(5))
    let n = AD_T * c
    with_dasllama_jobque_() {
        linear_vec(a.fc, style, f.style_fc)
        adain_rows_into(a, f.x, f.want_leaky, int64(AD_T), int64(c), style, AD_EPS)
    }
    f.want_snake := f.want_leaky
    leaky_relu(f.want_leaky, int64(n), 0.2)
    snake_rows(f.want_snake, int64(AD_T), int64(c), alpha)
    delete a
    delete style
    delete alpha
    return <- f
}

[test]
def test_vkt_adain(t0 : T?) {
    t0 |> run("the column stats + fused AdaIN, leaky and Snake stamps, == the CPU adain_rows_into, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_colstats() || !ensure_tts_adain_leaky_cls() || !ensure_tts_adain_snake_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let c = AD_C
            let n = AD_T * c
            var f <- adain_fixture()
            with_cell_planes() $(var cp : CellPlanes) {
                let x0p = cp |> in_(f.x)
                let xp = cp |> in_(f.x, AD_XOFF)
                let xip = cp |> io_(f.x, AD_XOFF)
                let hp = cp |> in_(f.style_fc, AD_HOFF)
                let wp = cp |> in_(f.slab)
                let stp = cp |> out_(colstats_floats(AD_T, c))
                let ylp = cp |> out_(n, AD_YOFF)
                let ysp = cp |> out_(n, AD_YOFF)
                var s_l = cp |> cls_set(fixed_array(xp, ylp, stp, hp, wp), @@set_tts_adain_leaky_cls)
                var s_s = cp |> cls_set(fixed_array(xp, ysp, stp, hp, wp), @@set_tts_adain_snake_cls)
                var s_i = cp |> cls_set(fixed_array(xip, xip, stp, hp, wp), @@set_tts_adain_leaky_cls)
                static_assert(AD_EPS == TTS_ADAIN_EPS, "the CPU oracle's eps is the stamps' literal")
                var pa = TtsAdainArgs(c = uint(c), t = uint(AD_T), xoff = uint(AD_XOFF), yoff = uint(AD_YOFF), hoff = uint(AD_HOFF),
                    gwoff = uint(AD_GWOFF), gboff = uint(f.gboff), aoff = uint(f.aoff))
                var pi = pa
                pi.yoff = uint(AD_XOFF)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    colstats_enc(raw, h, cp, x0p, stp, AD_T, c)
                    enc_tts_adain_leaky_cls(raw, h, s_l, pa, int64(n))
                    enc_tts_adain_snake_cls(raw, h, s_s, pa, int64(n))
                    enc_tts_adain_leaky_cls(raw, h, s_i, pi, int64(n))
                }
                let got_l <- cp |> plane_run(ylp)
                let got_i <- cp |> plane_run(xip)
                check_rows(t, "tts_adain_leaky vs the CPU adain_rows_into and leaky_relu", got_l, f.want_leaky, BAR_APPROX, AD_C)
                check_rows(t, "tts_adain_snake vs the CPU adain_rows_into and snake_rows", cp |> plane_run(ysp), f.want_snake, BAR_APPROX, AD_C)
                check_rows(t, "tts_adain_leaky in place vs the CPU adain_rows_into and leaky_relu", got_i, f.want_leaky, BAR_APPROX, AD_C)
                t |> success(got_i[0] != f.x[0], "tts_adain_leaky in place control: the input row is overwritten")
                t |> equal(mismatch_exact(got_i, got_l), 0, "tts_adain_leaky: in place == out of place, bit for bit")
                t |> success(outside_run(cp |> plane_whole(ylp), AD_YOFF, n, NAN_SENTINEL) && outside_run(cp |> plane_whole(ysp), AD_YOFF, n, NAN_SENTINEL),
                    "tts_adain: the prefix before yoff keeps its sentinel")
                t |> success(outside_run(cp |> plane_whole(xip), AD_XOFF, n, POISON), "tts_adain in place: the prefix before xoff is untouched")
            }
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private AS_N = 1030     //! four whole workgroups and six more
let private AS_AOFF = 3
let private AS_BOFF = 7
let private AS_OOFF = 11
let private AS_SLACK = 5    //! elements past the run, left at the sentinel

[test]
def test_vkt_add_scale(t0 : T?) {
    t0 |> run("the add-and-scale == (a + b) * 1/sqrt(2) in f32, exact, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_add_scale_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = AS_N
            let scale = 1.0 / sqrt(2.0)
            let a <- row_fill(n, 41, 7, 0.05, 2.1)
            let b <- row_fill(n, 17, 3, 0.04, 1.9)
            with_cell_planes() $(var cp : CellPlanes) {
                let ap = cp |> in_(a, AS_AOFF)
                let aip = cp |> io_(a, AS_AOFF)
                let bp = cp |> in_(b, AS_BOFF)
                let op = cp |> out_(n, AS_OOFF, AS_SLACK)
                var s_o = cp |> cls_set(fixed_array(ap, bp, op), @@set_tts_add_scale_cls)
                var s_i = cp |> cls_set(fixed_array(aip, bp, aip), @@set_tts_add_scale_cls)
                var po = TtsAddScaleArgs(nelem = uint(n), scale = scale, aoff = uint(AS_AOFF), boff = uint(AS_BOFF), ooff = uint(AS_OOFF))
                var pi = po
                pi.ooff = uint(AS_AOFF)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_add_scale_cls(raw, h, s_o, po, int64(n))
                    enc_tts_add_scale_cls(raw, h, s_i, pi, int64(n))
                }
                let got_i <- cp |> plane_run(aip)
                let want <- [for (i in range(n)); (a[i] + b[i]) * scale]
                check_rows(t, "tts_add_scale vs (a + b) * scale", cp |> plane_run(op), want, BAR_EXACT, FLAT_ROWS)
                check_rows(t, "tts_add_scale in place vs (a + b) * scale", got_i, want, BAR_EXACT, FLAT_ROWS)
                t |> success(got_i[0] != a[0], "tts_add_scale in place control: the input row is overwritten")
                t |> success(outside_run(cp |> plane_whole(op), AS_OOFF, n, NAN_SENTINEL), "tts_add_scale: the elements outside the run keep their sentinel")
                t |> success(outside_run(cp |> plane_whole(aip), AS_AOFF, n, POISON), "tts_add_scale in place: the prefix before aoff is untouched")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
