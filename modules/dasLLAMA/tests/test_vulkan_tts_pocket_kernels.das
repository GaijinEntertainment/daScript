options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_pocket_kernels

require dastest/testing_boost public
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_pocket   // modulate: the flow head's adaLN, the GEMV cell's CPU chain
require dasllama/dasllama_rope
require dasllama/dasllama_math
require daslib/jobque_boost

//! The Pocket TTS family's Vulkan classes against the CPU chain (a Vulkan device, else the cells skip): the codec
//! stream's row copies with and without the ELU, the layer scale, the causal cached attention row over the caches' rows,
//! the row GEMV's five stamps and the frame loop's four fused stamps, and the rows rope; every written-only output under a
//! NaN fill, every compare with its poisoned element.

let private SLACK = 5      //! the elements past a run, left at their fill

let private PR_SROWS = 30
let private PR_SS = 40
let private PR_SC = 5
let private PR_SR0 = 3
let private PR_DROWS = 12
let private PR_DS = 24
let private PR_DC = 6
let private PR_DR0 = 2
let private PR_ROWS = 7
let private PR_WIDTH = 13   //! the window: 91 elements, off every workgroup and float4 multiple

//! the window's elements out of a whole dst plane, row by row
def private pk_window(dst : array<float>) : array<float> {
    return <- [for (e in range(PR_ROWS * PR_WIDTH)); dst[(PR_DR0 + e / PR_WIDTH) * PR_DS + PR_DC + e % PR_WIDTH]]
}

[test]
def test_vkt_pk_rows(t0 : T?) {
    t0 |> run("the row copies, plain and ELU, == the CPU window copy and elu_rows at strides and offsets off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_rows_cls() || !ensure_tts_pk_rows_elu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let nwin = PR_ROWS * PR_WIDTH
            let ndst = PR_DROWS * PR_DS
            let src <- row_fill(PR_SROWS * PR_SS, 41, 7, 0.05, 2.1)
            let want <- [for (e in range(nwin)); src[(PR_SR0 + e / PR_WIDTH) * PR_SS + PR_SC + e % PR_WIDTH]]
            var want_e := want
            want_e |> push(0.0)
            with_dasllama_jobque_() {
                elu_rows(want_e, int64(nwin + 1))
            }
            want_e |> resize(nwin)
            with_cell_planes() $(var cp : CellPlanes) {
                let sp = cp |> in_(src)
                let dp = cp |> out_(ndst)
                let ep = cp |> out_(ndst)
                var s_p = cp |> cls_set(fixed_array(sp, dp), @@set_tts_pk_rows_cls)
                var s_e = cp |> cls_set(fixed_array(sp, ep), @@set_tts_pk_rows_elu_cls)
                var pa = TtsPkRowsArgs(rows = uint(PR_ROWS), width = uint(PR_WIDTH), ss = uint(PR_SS), sc = uint(PR_SC), sr0 = uint(PR_SR0),
                    ds = uint(PR_DS), dc = uint(PR_DC), dr0 = uint(PR_DR0))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_pk_rows_cls(raw, h, s_p, pa, int64(nwin))
                    enc_tts_pk_rows_elu_cls(raw, h, s_e, pa, int64(nwin))
                }
                let dst_p <- cp |> plane_run(dp)
                let dst_e <- cp |> plane_run(ep)
                let got <- pk_window(dst_p)
                let got_e <- pk_window(dst_e)
                check_rows(t, "tts_pk_rows window vs the source rows", got, want, BAR_EXACT, PR_WIDTH)
                t |> equal(nan_count(dst_p), ndst - nwin, "tts_pk_rows: every element outside the window keeps its sentinel")
                check_rows(t, "tts_pk_rows_elu window vs the CPU elu_rows", got_e, want_e, BAR_APPROX, PR_WIDTH)
                t |> equal(nan_count(dst_e), ndst - nwin, "tts_pk_rows_elu: every element outside the window keeps its sentinel")
                var neg = 0
                var pos_kept = true
                for (e in range(nwin)) {
                    if (want[e] < 0.0 && got_e[e] != got[e]) {
                        neg++
                    }
                    if (want[e] >= 0.0 && got_e[e] != got[e]) {
                        pos_kept = false
                    }
                }
                t |> success(neg > 0, "tts_pk_rows_elu control: the negative elements are mapped ({neg} moved off the plain copy)")
                t |> success(pos_kept, "tts_pk_rows_elu control: the non-negative elements pass unchanged")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private RS_T = 37
let private RS_C = 72
let private RS_SOFF = 9

[test]
def test_vkt_pk_row_scale(t0 : T?) {
    t0 |> run("the layer scale == the CPU layer_scale_rows, exact, at t 37 x 72 with the scale row at an offset") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_row_scale_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = RS_T * RS_C
            let x <- row_fill(n, 41, 7, 0.05, 2.1)
            let scale <- row_fill(RS_C, 29, 3, 0.03, 1.2)
            var want := x
            with_dasllama_jobque_() {
                layer_scale_rows(want, int64(RS_T), int64(RS_C), scale)
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x, 0, SLACK)
                let wp = cp |> in_(scale, RS_SOFF, SLACK)
                var s = cp |> cls_set(fixed_array(xp, wp), @@set_tts_pk_row_scale_cls)
                var pa = TowerBiasArgs(d = uint(RS_C), nelem = uint(n), boff = uint(RS_SOFF))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_pk_row_scale_cls(raw, h, s, pa, int64(n))
                }
                let got <- cp |> plane_run(xp)
                check_rows(t, "tts_pk_row_scale vs the CPU layer_scale_rows", got, want, BAR_EXACT, RS_C)
                t |> success(got[0] != x[0], "tts_pk_row_scale in place control: the input row is overwritten")
                t |> success(outside_run(cp |> plane_whole(xp), 0, n, POISON), "tts_pk_row_scale: the slack past the rows is untouched")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PA_HEADS = 2
let private PA_DH = 64
let private PA_D = 128
let private PA_T = 45
let private PA_CAP = 300
let private PA_QS = 264      //! q's row stride: the query span sits at PA_QCOL among other columns
let private PA_QCOL = 8
let private PA_PROWS = 48    //! the device caches' rows: the 45 appended, then three poisoned rows no query may see
let private PA_QOFF = 5
let private PA_KOFF = 7
let private PA_VOFF = 11
let private PA_OOFF = 13
let private PA_DEC_POS = 44  //! the decode step's position: the last appended key
let private PA_DEC_CTX = 8
let private PA_WIN_CTX = 16

//! a cache's keys and values as the device's rows [pos][d]
struct PkCacheRows {
    k : array<float>
    v : array<float>
}

//! the attention cell's outputs: the prompt over every key, the decode step, the decode step over the planes with a key
//! before its window poisoned, the prompt over the 16-key window, and the prompt output's whole plane (its prefix before ooff)
struct PkAttnOut {
    a : array<float>
    b : array<float>
    x : array<float>
    c : array<float>
    a_whole : array<float>
}

//! the cache as the device's `PA_PROWS` rows [pos][d], the rows past the cache's and the row `blank` poisoned
def private pk_cache_rows(kc : TtsKvCache; blank : int64) : PkCacheRows {
    var k <- poison_plane(PA_PROWS * PA_D)
    var v <- poison_plane(PA_PROWS * PA_D)
    for (p in range64(kc.n)) {
        continue if (p == blank)
        for (h in range(PA_HEADS)) {
            for (j in range(PA_DH)) {
                k[p * int64(PA_D) + int64(h * PA_DH + j)] = kc.k[int64(h * PA_DH + j) * int64(PA_CAP) + p]
                v[p * int64(PA_D) + int64(h * PA_DH + j)] = kc.v[(int64(h * PA_CAP) + p) * int64(PA_DH) + int64(j)]
            }
        }
    }
    return <- PkCacheRows(k <- k, v <- v)
}

//! the CPU outputs: the prompt over every key (a), the decode step over its window (b), the prompt over the 16-key window (c)
def private pk_attn_wants(var kc : TtsKvCache; q : array<float>) : PkAttnOut {
    let d = PA_D
    let kr <- row_fill(PA_T * d, 17, 3, 0.02, 0.97)
    let vr <- row_fill(PA_T * d, 53, 11, 0.04, 1.9)
    let qdec <- [for (e in range(d)); q[PA_DEC_POS * d + e]]
    var out : PkAttnOut
    with_dasllama_jobque_() {
        kv_cache_init(kc, int64(PA_HEADS), int64(PA_DH), int64(PA_CAP))
        kv_cache_append(kc, kr, vr, int64(PA_T), int64(d))
        attention_causal_rows(q, int64(PA_T), int64(d), 0l, kc, out.a)
        attention_causal_rows(qdec, 1l, int64(d), int64(PA_DEC_CTX), kc, out.b)
        attention_causal_rows(q, int64(PA_T), int64(d), int64(PA_WIN_CTX), kc, out.c)
    }
    return <- out
}

//! the four attention dispatches over the cache's rows and the q plane `qfull`, in one command buffer
def private pk_attn_device(kc : TtsKvCache; qfull : array<float>) : PkAttnOut {
    var out : PkAttnOut
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let d = PA_D
        var inscope rows <- pk_cache_rows(kc, -1l)
        var inscope rows_x <- pk_cache_rows(kc, int64(PA_DEC_POS - PA_DEC_CTX))
        with_cell_planes() $(var cp : CellPlanes) {
            let qp = cp |> in_(qfull, PA_QOFF, SLACK)
            let kp = cp |> in_(rows.k, PA_KOFF, SLACK)
            let vp = cp |> in_(rows.v, PA_VOFF, SLACK)
            let kxp = cp |> in_(rows_x.k, PA_KOFF, SLACK)
            let vxp = cp |> in_(rows_x.v, PA_VOFF, SLACK)
            let oa = cp |> out_(PA_T * d, PA_OOFF)
            let ob = cp |> out_(d, PA_OOFF)
            let ox = cp |> out_(d, PA_OOFF)
            let oc = cp |> out_(PA_T * d, PA_OOFF)
            var s_a = cp |> cls_set(fixed_array(qp, kp, vp, oa), @@set_tts_pk_attn_cls)
            var s_b = cp |> cls_set(fixed_array(qp, kp, vp, ob), @@set_tts_pk_attn_cls)
            var s_x = cp |> cls_set(fixed_array(qp, kxp, vxp, ox), @@set_tts_pk_attn_cls)
            var s_c = cp |> cls_set(fixed_array(qp, kp, vp, oc), @@set_tts_pk_attn_cls)
            var pa = TtsPkAttnArgs(t = uint(PA_T), qs = uint(PA_QS), qcol = uint(PA_QCOL), d = uint(d), pos0 = 0u, ctx = 0u,
                qoff = uint(PA_QOFF), koff = uint(PA_KOFF), voff = uint(PA_VOFF), ooff = uint(PA_OOFF))
            var pb = pa
            pb.t = 1u
            pb.pos0 = uint(PA_DEC_POS)
            pb.ctx = uint(PA_DEC_CTX)
            pb.qoff = uint(PA_QOFF + PA_DEC_POS * PA_QS)
            var pc = pa
            pc.ctx = uint(PA_WIN_CTX)
            let heads = int64(PA_HEADS)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pk_attn_cls(raw, h, s_a, pa, tts_attn_wgs(int64(PA_T), heads))
                enc_tts_pk_attn_cls(raw, h, s_b, pb, tts_attn_wgs(1l, heads))
                enc_tts_pk_attn_cls(raw, h, s_x, pb, tts_attn_wgs(1l, heads))
                enc_tts_pk_attn_cls(raw, h, s_c, pc, tts_attn_wgs(int64(PA_T), heads))
            }
            out.a <- cp |> plane_run(oa)
            out.b <- cp |> plane_run(ob)
            out.x <- cp |> plane_run(ox)
            out.c <- cp |> plane_run(oc)
            out.a_whole <- cp |> plane_whole(oa)
        }
    }
    return <- out
}

[test]
def test_vkt_pk_attn(t0 : T?) {
    t0 |> run("the causal attention row == the CPU attention_causal_rows: a 45-query prompt, a decode step over an 8-key window, the prompt over 16") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = PA_D
            let qfull <- row_fill(PA_T * PA_QS, 41, 7, 0.02, 0.95)
            let q <- [for (e in range(PA_T * d)); qfull[(e / d) * PA_QS + PA_QCOL + e % d]]
            var inscope kc : TtsKvCache
            var inscope want <- pk_attn_wants(kc, q)
            var inscope got <- pk_attn_device(kc, qfull)
            check_rows(t, "tts_pk_attn prompt t{PA_T} every key vs the CPU attention_causal_rows", got.a, want.a, BAR_APPROX, PA_D, PA_DH)
            check_rows(t, "tts_pk_attn decode pos {PA_DEC_POS} ctx {PA_DEC_CTX} vs the CPU attention_causal_rows", got.b, want.b, BAR_APPROX, PA_D, PA_DH)
            check_rows(t, "tts_pk_attn prompt t{PA_T} ctx {PA_WIN_CTX} vs the CPU attention_causal_rows", got.c, want.c, BAR_APPROX, PA_D, PA_DH)
            t |> equal(mismatch_exact(got.x, got.b), 0, "tts_pk_attn control: a poisoned key before the decode step's window stays silent, bit for bit")
            var moved = 0
            for (e in range(PA_T * d)) {
                if (!approx(got.a[e], got.c[e])) {
                    moved++
                }
            }
            t |> success(moved > 0, "tts_pk_attn control: the 16-key window moves the prompt's late rows off the every-key rows ({moved} apart)")
            t |> success(outside_run(got.a_whole, PA_OOFF, PA_T * d, NAN_SENTINEL), "tts_pk_attn: the prefix before ooff keeps its sentinel")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PG_NIN = 200     //! off the 32-lane multiple
let private PG_NOUT = 45     //! off the four-row multiple: the last workgroup masks three rows
let private PG_STRIDE = 208  //! the weight rows' stride, the pad past nin poisoned
let private PG_EPS = 1e-5
let private PG_XOFF = 7
let private PG_YOFF = 9
let private PG_MOFF = 11
let private PG_X0OFF = 3
let private PG_LOFF = 4

//! the GEMV cell's operands: the linear and the input norm the CPU chain reads, the vectors, and the slab plane the
//! device reads with each run's element base
struct PkGemvFix {
    lin : TtsLinear = TtsLinear()
    norm : TtsNorm = TtsNorm()
    aux : array<float>    //! the slab vector: the add-SiLU's [nout], the tail's scale [nout] then shift [nout]
    x : array<float>
    mods : array<float>   //! shift [nin], scale [nin], gate [nout]
    x0 : array<float>
    y0 : array<float>     //! the gated residual's input row
    slab : array<float>
    woff : int
    boff : int
    lwoff : int
    lboff : int
    aoff : int
}

//! the five stamps' output rows (and the tail's latent row), or their whole staged planes
struct PkGemvRows {
    dot : array<float>
    ln : array<float>
    gate : array<float>
    add : array<float>
    tail : array<float>
    lat : array<float>
}

//! the fixture with its weight rows on the four lattice (`quad`: the float4 dot) or off it (the float dot)
def private pk_gemv_fixture(quad : bool) : PkGemvFix {
    let nin = PG_NIN
    let nout = PG_NOUT
    var f <- PkGemvFix(lin <- TtsLinear(w <- row_fill(nout * nin, 17, 3, 0.02, 0.97), b <- row_fill(nout, 29, 11, 0.03, 1.4),
            nin = int64(nin), nout = int64(nout)),
        norm <- TtsNorm(w <- row_fill(nin, 29, 5, 0.01, -0.5), b <- row_fill(nin, 31, 2, 0.02, 0.9)),
        aux <- row_fill(2 * nout, 37, 5, 0.03, 1.3), x <- row_fill(nin, 41, 7, 0.05, 2.1),
        mods <- row_fill(2 * nin + nout, 31, 7, 0.01, 0.5), x0 <- row_fill(nout, 43, 13, 0.04, 1.8),
        y0 <- row_fill(nout, 47, 17, 0.03, 1.1))
    var wst <- poison_plane(nout * PG_STRIDE)
    for (o in range(nout)) {
        for (k in range(nin)) {
            wst[o * PG_STRIDE + k] = f.lin.w[o * nin + k]
        }
    }
    f.woff = quad ? put_run4(f.slab, wst) : put_run(f.slab, wst)
    f.boff = put_run(f.slab, f.lin.b)
    f.lwoff = put_run(f.slab, f.norm.w)
    f.lboff = put_run(f.slab, f.norm.b)
    f.aoff = put_run(f.slab, f.aux)
    f.slab |> push_from(poison_plane(SLAB_GAP))
    return <- f
}

//! the CPU chain of each stamp: the linear over x or over the normed and modulated x, then the stamp's epilogue
def private pk_gemv_wants(f : PkGemvFix) : PkGemvRows {
    let nin = PG_NIN
    let nout = PG_NOUT
    var d_plain : array<float>
    var d_norm : array<float>
    with_dasllama_jobque_() {
        linear_vec(f.lin, f.x, d_plain)
        var xn := f.x
        layernorm_rows(xn, 1l, int64(nin), f.norm, PG_EPS)
        modulate(xn, f.mods, int64(nin))
        linear_vec(f.lin, xn, d_norm)
    }
    var out : PkGemvRows
    out.dot := d_plain
    out.ln := d_norm
    silu(out.ln, int64(nout))
    out.gate <- [for (o in range(nout)); f.y0[o] + f.mods[2 * nin + o] * d_plain[o]]
    out.add <- [for (o in range(nout)); d_plain[o] + f.aux[o]]
    silu(out.add, int64(nout))
    out.tail <- [for (o in range(nout)); d_norm[o] + f.x0[o]]
    out.lat <- [for (o in range(nout)); out.tail[o] * f.aux[o] + f.aux[nout + o]]
    return <- out
}

//! the five stamps over one command buffer, the y planes staged whole: the gated residual's over its input row, the rest under a NaN fill
def private pk_gemv_device(f : PkGemvFix) : PkGemvRows {
    var out : PkGemvRows
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nout = PG_NOUT
        with_cell_planes() $(var cp : CellPlanes) {
            let wp = cp |> in_(f.slab)
            let xp = cp |> in_(f.x, PG_XOFF, SLACK)
            let mp = cp |> in_(f.mods, PG_MOFF, SLACK)
            let x0p = cp |> in_(f.x0, PG_X0OFF, SLACK)
            let dotp = cp |> out_(nout, PG_YOFF, SLACK)
            let lnp = cp |> out_(nout, PG_YOFF, SLACK)
            let gatep = cp |> io_(f.y0, PG_YOFF, SLACK)
            let addp = cp |> out_(nout, PG_YOFF, SLACK)
            let tailp = cp |> out_(nout, PG_YOFF, SLACK)
            let latp = cp |> out_(nout, PG_LOFF, SLACK)
            var s_dot = cp |> cls_set(fixed_array(wp, xp, dotp), @@set_tts_pk_gemv_dot_cls)
            var s_ln = cp |> cls_set(fixed_array(wp, xp, lnp, mp), @@set_tts_pk_gemv_ln_silu_cls)
            var s_gate = cp |> cls_set(fixed_array(wp, xp, gatep, mp), @@set_tts_pk_gemv_gate_cls)
            var s_add = cp |> cls_set(fixed_array(wp, xp, addp), @@set_tts_pk_gemv_add_silu_cls)
            var s_tail = cp |> cls_set(fixed_array(wp, xp, tailp, mp, x0p, latp), @@set_tts_pk_gemv_tail_cls)
            var pa = TtsPkGemvArgs(nin = uint(PG_NIN), nout = uint(nout), stride = uint(PG_STRIDE), eps = PG_EPS, woff = uint(f.woff),
                boff = uint(f.boff), xoff = uint(PG_XOFF), yoff = uint(PG_YOFF), moff = uint(PG_MOFF), lwoff = uint(f.lwoff),
                lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = uint(PG_X0OFF), loff = uint(PG_LOFF))
            let rows = int64(nout)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pk_gemv_dot_cls(raw, h, s_dot, pa, rows)
                enc_tts_pk_gemv_ln_silu_cls(raw, h, s_ln, pa, rows)
                enc_tts_pk_gemv_gate_cls(raw, h, s_gate, pa, rows)
                enc_tts_pk_gemv_add_silu_cls(raw, h, s_add, pa, rows)
                enc_tts_pk_gemv_tail_cls(raw, h, s_tail, pa, rows)
            }
            out.dot <- cp |> plane_whole(dotp)
            out.ln <- cp |> plane_whole(lnp)
            out.gate <- cp |> plane_whole(gatep)
            out.add <- cp |> plane_whole(addp)
            out.tail <- cp |> plane_whole(tailp)
            out.lat <- cp |> plane_whole(latp)
        }
    }
    return <- out
}

//! the five stamps over one fixture against their CPU chains, `arm` naming the dot's path
def private pk_gemv_arm(t : T?; arm : string; quad : bool) {
    var inscope f <- pk_gemv_fixture(quad)
    t |> equal(f.woff % 4 == 0, quad, "{arm}: the weight rows sit on the lattice the arm names")
    var inscope want <- pk_gemv_wants(f)
    var inscope got <- pk_gemv_device(f)
    check_rows(t, "{arm} tts_pk_gemv_dot vs the CPU chain", copy_span(got.dot, PG_YOFF, PG_NOUT), want.dot, BAR_APPROX, 1)
    check_rows(t, "{arm} tts_pk_gemv_ln_silu vs the CPU chain", copy_span(got.ln, PG_YOFF, PG_NOUT), want.ln, BAR_APPROX, 1)
    check_rows(t, "{arm} tts_pk_gemv_gate vs the CPU chain", copy_span(got.gate, PG_YOFF, PG_NOUT), want.gate, BAR_APPROX, 1)
    check_rows(t, "{arm} tts_pk_gemv_add_silu vs the CPU chain", copy_span(got.add, PG_YOFF, PG_NOUT), want.add, BAR_APPROX, 1)
    check_rows(t, "{arm} tts_pk_gemv_tail vs the CPU chain", copy_span(got.tail, PG_YOFF, PG_NOUT), want.tail, BAR_APPROX, 1)
    check_rows(t, "{arm} tts_pk_gemv_tail latent vs the CPU chain", copy_span(got.lat, PG_LOFF, PG_NOUT), want.lat, BAR_APPROX, 1)
    t |> success(got.gate[PG_YOFF] != f.y0[0], "{arm} tts_pk_gemv_gate in place control: the residual row is moved off its input")
    let ny = length(got.dot)
    t |> equal(nan_count(got.dot) + nan_count(got.ln) + nan_count(got.add) + nan_count(got.tail), 4 * (ny - PG_NOUT),
        "{arm} tts_pk_gemv: the slots before yoff and the masked rows past nout keep their sentinel")
    t |> equal(nan_count(got.lat), PG_LOFF + SLACK, "{arm} tts_pk_gemv_tail: the latent plane outside its row keeps its sentinel")
    t |> success(outside_run(got.gate, PG_YOFF, PG_NOUT, POISON), "{arm} tts_pk_gemv_gate: the residual plane outside its row is untouched")
}

[test]
def test_vkt_pk_gemv(t0 : T?) {
    t0 |> run("the row GEMV's five stamps == the CPU linear_vec chains at nin 200 / nout 45, the slab's runs at offsets, the rows on and off the four lattice") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_gemv_dot_cls() || !ensure_tts_pk_gemv_ln_silu_cls() || !ensure_tts_pk_gemv_gate_cls()
                    || !ensure_tts_pk_gemv_add_silu_cls() || !ensure_tts_pk_gemv_tail_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            pk_gemv_arm(t, "float rows", false)
            pk_gemv_arm(t, "float4 rows", true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PO_T = 23
let private PO_D = 128
let private PO_DH = 64
let private PO_QS = 384        //! q, k and v side by side: the rope turns the k span alone
let private PO_POS0 = 37
let private PO_CAP = 64        //! the tables' positions, past the last row's
let private PO_THETA = 10000.0

[test]
def test_vkt_pk_rope(t0 : T?) {
    t0 |> run("the rows rope == the CPU rope_rows on the k span of t 23 rows from position 37, the q and v spans untouched") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_rope_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = PO_T * PO_QS
            let nk = PO_T * PO_D
            let half = PO_DH / 2
            let x <- row_fill(n, 41, 7, 0.05, 2.1)
            var cos_tab : array<float>
            var sin_tab : array<float>
            let no_freqs : array<float>
            build_rope_tabs(cos_tab, sin_tab, PO_THETA, 1.0, 1.0, no_freqs, false, 0l, int64(PO_CAP), int64(PO_DH))
            t |> equal(length(cos_tab), PO_CAP * half, "tts_pk_rope: the tables carry {PO_CAP} positions of {half} angles")
            var slab : array<float>
            let coff = put_run(slab, cos_tab) + PO_POS0 * half   // the tables bound at the first row's position: row r reads table row r
            let soff = put_run(slab, sin_tab) + PO_POS0 * half
            let kspan <- [for (e in range(nk)); x[(e / PO_D) * PO_QS + PO_D + e % PO_D]]
            var want := kspan
            rope_rows(want, int64(PO_T), int64(PO_D), int64(PO_DH), int64(PO_POS0), PO_THETA)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> io_(x, 0, SLACK)
                let wp = cp |> in_(slab)
                var s = cp |> cls_set(fixed_array(xp, wp, wp), @@set_tts_pk_rope_cls)
                var pa = RopeTabArgs(rows = uint(PO_T), qs = uint(PO_QS), col = uint(PO_D), d = uint(PO_D), dh = uint(PO_DH), coff = uint(coff), soff = uint(soff))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_pk_rope_cls(raw, h, s, pa, int64(nk / 2))
                }
                let rows <- cp |> plane_run(xp)
                let got <- [for (e in range(nk)); rows[(e / PO_D) * PO_QS + PO_D + e % PO_D]]
                check_rows(t, "tts_pk_rope k span vs the CPU rope_rows", got, want, BAR_APPROX, PO_D, PO_DH)
                t |> success(got[2] != kspan[2], "tts_pk_rope in place control: a pair at a nonzero angle is turned off its input")
                var kept = 0
                for (r in range(PO_T)) {
                    for (j in range(PO_QS)) {
                        if ((j < PO_D || j >= 2 * PO_D) && rows[r * PO_QS + j] == x[r * PO_QS + j]) {
                            kept++
                        }
                    }
                }
                t |> equal(kept, PO_T * 2 * PO_D, "tts_pk_rope: the q and v spans are untouched")
                t |> success(outside_run(cp |> plane_whole(xp), 0, n, POISON), "tts_pk_rope: the slack past the rows is untouched")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PF_D = 64        //! the backbone width: the q, k and v spans
let private PF_DH = 32       //! two heads a span
let private PF_P = 5         //! the frame's position: the tables' and the caches' row
let private PF_CAP = 8       //! the caches' rows
let private PF_STRIDE = 96   //! the weight rows' stride, the pad past the width poisoned
let private PF_XOFF = 7
let private PF_YOFF = 9

//! the frame loop's linears on the slab: the qkv rows [3d][d] and a square [d][d], their biases, the norm rows and the scale row
struct private PkFusedFix {
    qkv : TtsLinear = TtsLinear()
    sq : TtsLinear = TtsLinear()
    norm : TtsNorm = TtsNorm()
    x : array<float>
    y0 : array<float>      //! the residual row the adds land in
    scale : array<float>   //! the layer scale row
    slab : array<float>
    tabs : array<float>    //! the rope tables: cos at coff, sin at soff
    woff_q : int
    boff_q : int
    woff_s : int
    boff_s : int
    lwoff : int
    lboff : int
    aoff : int
    coff : int
    soff : int
}

def private pk_fused_fixture() : PkFusedFix {
    let d = PF_D
    var f <- PkFusedFix(qkv <- TtsLinear(w <- row_fill(3 * d * d, 17, 3, 0.02, 0.97), b <- row_fill(3 * d, 29, 11, 0.03, 1.4), nin = int64(d), nout = int64(3 * d)),
        sq <- TtsLinear(w <- row_fill(d * d, 19, 5, 0.02, 0.9), b <- row_fill(d, 23, 7, 0.03, 1.2), nin = int64(d), nout = int64(d)),
        norm <- TtsNorm(w <- row_fill(d, 29, 5, 0.01, -0.5), b <- row_fill(d, 31, 2, 0.02, 0.9)),
        x <- row_fill(d, 41, 7, 0.05, 2.1), y0 <- row_fill(d, 47, 17, 0.03, 1.1), scale <- row_fill(d, 37, 5, 0.03, 1.3))
    var wq <- poison_plane(3 * d * PF_STRIDE)
    for (o in range(3 * d)) {
        for (k in range(d)) {
            wq[o * PF_STRIDE + k] = f.qkv.w[o * d + k]
        }
    }
    var ws <- poison_plane(d * PF_STRIDE)
    for (o in range(d)) {
        for (k in range(d)) {
            ws[o * PF_STRIDE + k] = f.sq.w[o * d + k]
        }
    }
    f.woff_q = put_run(f.slab, wq)
    f.boff_q = put_run(f.slab, f.qkv.b)
    f.woff_s = put_run(f.slab, ws)
    f.boff_s = put_run(f.slab, f.sq.b)
    f.lwoff = put_run(f.slab, f.norm.w)
    f.lboff = put_run(f.slab, f.norm.b)
    f.aoff = put_run(f.slab, f.scale)
    f.slab |> push_from(poison_plane(SLAB_GAP))
    var cos_tab : array<float>
    var sin_tab : array<float>
    let no_freqs : array<float>
    build_rope_tabs(cos_tab, sin_tab, PO_THETA, 1.0, 1.0, no_freqs, false, 0l, int64(PF_CAP), int64(PF_DH))
    f.coff = put_run(f.tabs, cos_tab)
    f.soff = put_run(f.tabs, sin_tab)
    return <- f
}

[test]
def test_vkt_pk_gemv_fused(t0 : T?) {
    t0 |> run("the frame loop's fused stamps == the CPU chains: the normed qkv row roped into y and the caches, the residual adds, the normed GELU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_gemv_ln_qkv_cls() || !ensure_tts_pk_gemv_add_scale_cls() || !ensure_tts_pk_gemv_add_cls()
                    || !ensure_tts_pk_gemv_ln_gelu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let d = PF_D
            var inscope f <- pk_fused_fixture()
            var xn := f.x
            var qkv_row : array<float>
            var dp : array<float>
            var dn : array<float>
            with_dasllama_jobque_() {
                layernorm_rows(xn, 1l, int64(d), f.norm, PG_EPS)
                linear_vec(f.qkv, xn, qkv_row)
                linear_vec(f.sq, f.x, dp)
                linear_vec(f.sq, xn, dn)
            }
            var want_q <- copy_span(qkv_row, 0, d)
            rope_rows(want_q, 1l, int64(d), int64(PF_DH), int64(PF_P), PO_THETA)
            var want_k <- copy_span(qkv_row, d, d)
            rope_rows(want_k, 1l, int64(d), int64(PF_DH), int64(PF_P), PO_THETA)
            let want_v <- copy_span(qkv_row, 2 * d, d)
            let want_as <- [for (o in range(d)); f.y0[o] + dp[o] * f.scale[o]]
            let want_a <- [for (o in range(d)); f.y0[o] + dp[o]]
            var want_g := dn
            gelu(want_g, int64(d))
            let nc = PF_CAP * d
            with_cell_planes() $(var cp : CellPlanes) {
                let wp = cp |> in_(f.slab)
                let xp = cp |> in_(f.x, PF_XOFF, SLACK)
                let tp = cp |> in_(f.tabs)
                let kcp = cp |> io_(poison_plane(nc))
                let vcp = cp |> io_(poison_plane(nc))
                let yasp = cp |> io_(f.y0, PF_YOFF, SLACK)
                let yap = cp |> io_(f.y0, PF_YOFF, SLACK)
                let yp = cp |> out_(3 * d, PF_YOFF, SLACK)
                let ygp = cp |> out_(d, PF_YOFF, SLACK)
                var s_qkv = cp |> cls_set(fixed_array(wp, xp, yp, tp, kcp, vcp), @@set_tts_pk_gemv_ln_qkv_cls)
                var s_as = cp |> cls_set(fixed_array(wp, xp, yasp), @@set_tts_pk_gemv_add_scale_cls)
                var s_a = cp |> cls_set(fixed_array(wp, xp, yap), @@set_tts_pk_gemv_add_cls)
                var s_g = cp |> cls_set(fixed_array(wp, xp, ygp), @@set_tts_pk_gemv_ln_gelu_cls)
                var pq = TtsPkGemvArgs(nin = uint(d), nout = uint(3 * d), stride = uint(PF_STRIDE), eps = PG_EPS, woff = uint(f.woff_q), boff = uint(f.boff_q),
                    xoff = uint(PF_XOFF), yoff = uint(PF_YOFF), moff = 0u, lwoff = uint(f.lwoff), lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = 0u, loff = 0u,
                    span = uint(d), dh = uint(PF_DH), p = uint(PF_P), coff = uint(f.coff), soff = uint(f.soff))
                var ps = pq
                ps.nout = uint(d)
                ps.woff = uint(f.woff_s)
                ps.boff = uint(f.boff_s)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_tts_pk_gemv_ln_qkv_cls(raw, h, s_qkv, pq, int64(3 * d))
                    enc_tts_pk_gemv_add_scale_cls(raw, h, s_as, ps, int64(d))
                    enc_tts_pk_gemv_add_cls(raw, h, s_a, ps, int64(d))
                    enc_tts_pk_gemv_ln_gelu_cls(raw, h, s_g, ps, int64(d))
                }
                let y_run <- cp |> plane_run(yp)
                let kc <- cp |> plane_run(kcp)
                let vc <- cp |> plane_run(vcp)
                let yas <- cp |> plane_run(yasp)
                let ya <- cp |> plane_run(yap)
                check_rows(t, "tts_pk_gemv_ln_qkv q vs the CPU chain", copy_span(y_run, 0, d), want_q, BAR_APPROX, FLAT_ROWS)
                check_rows(t, "tts_pk_gemv_ln_qkv k cache vs the CPU chain", copy_span(kc, PF_P * d, d), want_k, BAR_APPROX, FLAT_ROWS)
                check_rows(t, "tts_pk_gemv_ln_qkv v cache vs the CPU chain", copy_span(vc, PF_P * d, d), want_v, BAR_APPROX, FLAT_ROWS)
                check_rows(t, "tts_pk_gemv_add_scale vs the CPU chain", yas, want_as, BAR_APPROX, FLAT_ROWS)
                check_rows(t, "tts_pk_gemv_add vs the CPU chain", ya, want_a, BAR_APPROX, FLAT_ROWS)
                check_rows(t, "tts_pk_gemv_ln_gelu vs the CPU chain", cp |> plane_run(ygp), want_g, BAR_APPROX, FLAT_ROWS)
                t |> success(outside_run(cp |> plane_whole(yp), PF_YOFF, d, NAN_SENTINEL), "tts_pk_gemv_ln_qkv: only the q span lands in y - the k and v spans and the slots outside keep their sentinel")
                let other_rows <- [for (i in range(nc)); kc[i] + vc[i]; where i / d != PF_P]
                t |> success(elem_all(other_rows, 2.0 * POISON), "tts_pk_gemv_ln_qkv: the caches' other rows are untouched")
                t |> success(outside_run(cp |> plane_whole(ygp), PF_YOFF, d, NAN_SENTINEL), "tts_pk_gemv_ln_gelu: the slots outside the row keep their sentinel")
                t |> success(yas[0] != f.y0[0] && ya[0] != f.y0[0], "the adds' in place control: the residual row is moved off its input")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
