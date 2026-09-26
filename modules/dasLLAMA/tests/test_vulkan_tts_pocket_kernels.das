options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module test_vulkan_tts_pocket_kernels

require dastest/testing_boost public
require math
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_rope
require dasllama/dasllama_math   // setup_dasllama_jobque_, silu
require daslib/jobque_boost

//! The Pocket TTS family's Vulkan classes against the CPU chain (a Vulkan device, else the cells skip): the codec
//! stream's row copies with and without the ELU, the layer scale, the causal cached attention row over the caches' rows,
//! the row GEMV's five stamps and the rows rope; every written-only output under a NaN fill, every compare with its
//! poisoned element.

let private POISON = 7.5   //! the slots a kernel must never read or write: a large finite value
let private SLACK = 5      //! the elements past a run, left at their fill
let private GAP = 3        //! the poisoned slots between two runs of one slab plane

def private row_fill(n : int; mul, add : int; scale, off : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (i in range(n)) {
        a[i] = float((i * mul + add) % 97) * scale - off
    }
    return <- a
}

//! `n` slots of `POISON`
def private poison_plane(n : int) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (v in a) {
        v = POISON
    }
    return <- a
}

//! `a` behind `base` slots of `POISON` and followed by `SLACK` more: a device plane whose run starts at an element base
def private with_base(a : array<float>; base : int) : array<float> {
    var out <- poison_plane(base + length(a) + SLACK)
    for (i in range(length(a))) {
        out[base + i] = a[i]
    }
    return <- out
}

//! `a` appended to `plane` behind `GAP` poisoned slots; its element base in the plane
def private put(var plane : array<float>; a : array<float>) : int {
    let off = length(plane) + GAP
    plane |> resize(off + length(a))
    for (i in range(GAP)) {
        plane[off - GAP + i] = POISON
    }
    for (i in range(length(a))) {
        plane[off + i] = a[i]
    }
    return off
}

//! the index of the largest |got - want| (0 when every element agrees)
def private maxdiff_at(got, want : array<float>) : int {
    var m = 0.0
    var at = 0
    for (i in range(length(got))) {
        let dd = abs(got[i] - want[i])
        if (dd > m) {
            m = dd
            at = i
        }
    }
    return at
}

//! the NaN elements of `a`: the sentinel words a dispatch left
def private nan_count(a : array<float>) : int {
    var n = 0
    for (v in a) {
        if (is_nan(v)) {
            n++
        }
    }
    return n
}

//! true when every element of `a` is bit-equal to `v`
def private elem_all(a : array<float>; v : float) : bool {
    for (e in a) {
        if (e != v) {
            return false
        }
    }
    return true
}

//! a device buffer holding `a` whole
def private dev_upload(a : array<auto(TT)>) : uint64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let bytes = long_length(a) * int64(typeinfo sizeof(type<TT>))
        let dev = make_device_buf(bytes)
        upload_region_at(dev, 0l, unsafe(addr<uint8 const?>(a[0])), bytes)
        return dev
    } else {
        return 0ul
    }
}

// ===== the row copies =====

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
            let B_P = 1u
            let B_E = 2u
            let nwin = PR_ROWS * PR_WIDTH
            let ndst = PR_DROWS * PR_DS
            let src <- row_fill(PR_SROWS * PR_SS, 41, 7, 0.05, 2.1)
            let srcd = dev_upload(src)
            let bsrc = long_length(src) * 4l
            let bdst = int64(ndst) * 4l
            let dpd = make_device_buf(bdst)
            let ded = make_device_buf(bdst)
            let sizes = fixed_array(bdst, bdst)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            nan_sentinel_upload(dpd, bdst)
            nan_sentinel_upload(ded, bdst)
            var s_p = set_tts_pk_rows_cls(fixed_array(srcd, dpd), fixed_array(bsrc, bdst), fixed_array(0u, B_P))
            var s_e = set_tts_pk_rows_elu_cls(fixed_array(srcd, ded), fixed_array(bsrc, bdst), fixed_array(0u, B_E))
            var pa = TtsPkRowsArgs(rows = uint(PR_ROWS), width = uint(PR_WIDTH), ss = uint(PR_SS), sc = uint(PR_SC), sr0 = uint(PR_SR0),
                ds = uint(PR_DS), dc = uint(PR_DC), dr0 = uint(PR_DR0))
            let wgs = tts_pk_rows_wgs(int64(nwin))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pk_rows_cls(raw, h, s_p, pa, wgs)
                enc_tts_pk_rows_elu_cls(raw, h, s_e, pa, wgs)
                cmd_copy_planes(raw, h, fixed_array(dpd, ded), host, sizes, B_P | B_E)
            }
            let dst_p <- staged(host.mapped, at[0], ndst, type<float>)
            let dst_e <- staged(host.mapped, at[1], ndst, type<float>)
            let got <- pk_window(dst_p)
            let got_e <- pk_window(dst_e)
            let want <- [for (e in range(nwin)); src[(PR_SR0 + e / PR_WIDTH) * PR_SS + PR_SC + e % PR_WIDTH]]
            var want_e := want
            want_e |> push(0.0)
            with_job_que() {
                setup_dasllama_jobque_()
                elu_rows(want_e, int64(nwin + 1))
            }
            want_e |> resize(nwin)
            let bad = mismatch_exact(got, want)
            t |> success(bad == 0, "tts_pk_rows: the window == the source rows, exact ({bad} of {nwin} off)")
            t |> success(got[nwin - 1] + 0.5 != want[nwin - 1], "tts_pk_rows: the compare reds a poisoned element")
            t |> equal(nan_count(dst_p), ndst - nwin, "tts_pk_rows: every element outside the window keeps its sentinel")
            let bad_e = mismatch_approx(got_e, want_e)
            let mi = maxdiff_at(got_e, want_e)
            to_log(LOG_INFO, "tts_pk_rows_elu: max |gpu - cpu| {abs(got_e[mi] - want_e[mi])} at row {mi / PR_WIDTH} col {mi % PR_WIDTH}\n")
            t |> success(bad_e == 0, "tts_pk_rows_elu: the window == the CPU elu_rows ({bad_e} of {nwin} off)")
            t |> success(!approx(got_e[0] + 0.5, want_e[0]), "tts_pk_rows_elu: the bar reds a poisoned element")
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
            destroy_device_buf(srcd)
            destroy_device_buf(dpd)
            destroy_device_buf(ded)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the layer scale =====

let private RS_T = 37
let private RS_C = 72
let private RS_XOFF = 5
let private RS_SOFF = 9

[test]
def test_vkt_pk_row_scale(t0 : T?) {
    t0 |> run("the layer scale == the CPU layer_scale_rows, exact, at t 37 x 72 with x and the scale row at offsets") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_row_scale_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
            let n = RS_T * RS_C
            let x <- row_fill(n, 41, 7, 0.05, 2.1)
            let scale <- row_fill(RS_C, 29, 3, 0.03, 1.2)
            let xp <- with_base(x, RS_XOFF)
            let wn <- with_base(scale, RS_SOFF)
            let xd = dev_upload(xp)
            let wnd = dev_upload(wn)
            let bx = long_length(xp) * 4l
            let sizes = fixed_array(bx)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            var s = set_tts_pk_row_scale_cls(fixed_array(xd, wnd), fixed_array(bx, long_length(wn) * 4l), fixed_array(B_X, 0u))
            var pa = TtsPkScaleArgs(rows = uint(RS_T), c = uint(RS_C), xoff = uint(RS_XOFF), soff = uint(RS_SOFF))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pk_row_scale_cls(raw, h, s, pa, tts_pk_row_scale_wgs(int64(n)))
                cmd_copy_planes(raw, h, fixed_array(xd), host, sizes, B_X)
            }
            let whole <- staged(host.mapped, at[0], length(xp), type<float>)
            let got <- [for (i in range(n)); whole[RS_XOFF + i]]
            var want := x
            with_job_que() {
                setup_dasllama_jobque_()
                layer_scale_rows(want, int64(RS_T), int64(RS_C), scale)
            }
            let bad = mismatch_exact(got, want)
            t |> success(bad == 0, "tts_pk_row_scale: every element == the CPU layer_scale_rows ({bad} of {n} off)")
            t |> success(got[n - 1] + 0.5 != want[n - 1], "tts_pk_row_scale: the compare reds a poisoned element")
            t |> success(got[0] != x[0], "tts_pk_row_scale in place control: the input row is overwritten")
            let outside <- [for (i in range(length(whole))); whole[i]; where i < RS_XOFF || i >= RS_XOFF + n]
            t |> success(elem_all(outside, POISON), "tts_pk_row_scale: the prefix before xoff and the slack past the rows are untouched")
            destroy_device_buf(xd)
            destroy_device_buf(wnd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the causal cached attention row =====

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
//! before its window poisoned, the prompt over the 16-key window, and the prompt output's prefix before ooff
struct PkAttnOut {
    a : array<float>
    b : array<float>
    x : array<float>
    c : array<float>
    pre : array<float>
}

//! the cache's keys and values as the device's rows [pos][d], `PA_PROWS` of them - the rows past the cache's poisoned,
//! and the row `blank` poisoned too
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

//! the CPU outputs over the cache the keys and values fill: the prompt over every key (a), the decode step over its
//! window (b), the prompt over the 16-key window (c)
def private pk_attn_wants(var kc : TtsKvCache; q : array<float>) : PkAttnOut {
    let d = PA_D
    let kr <- row_fill(PA_T * d, 17, 3, 0.02, 0.97)
    let vr <- row_fill(PA_T * d, 53, 11, 0.04, 1.9)
    let qdec <- [for (e in range(d)); q[PA_DEC_POS * d + e]]
    var out : PkAttnOut
    with_job_que() {
        setup_dasllama_jobque_()
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
        let B_OA = 1u
        let B_OB = 2u
        let B_OX = 4u
        let B_OC = 8u
        let d = PA_D
        var inscope rows <- pk_cache_rows(kc, -1l)
        var inscope rows_x <- pk_cache_rows(kc, int64(PA_DEC_POS - PA_DEC_CTX))
        let qd = dev_upload(with_base(qfull, PA_QOFF))
        let kd = dev_upload(with_base(rows.k, PA_KOFF))
        let vd = dev_upload(with_base(rows.v, PA_VOFF))
        let kxd = dev_upload(with_base(rows_x.k, PA_KOFF))
        let vxd = dev_upload(with_base(rows_x.v, PA_VOFF))
        let bq = int64(PA_QOFF + length(qfull) + SLACK) * 4l
        let bk = int64(PA_KOFF + length(rows.k) + SLACK) * 4l
        let bv = int64(PA_VOFF + length(rows.v) + SLACK) * 4l
        let bo = int64(PA_OOFF + PA_T * d) * 4l
        let bo1 = int64(PA_OOFF + d) * 4l
        let outs = fixed_array(make_device_buf(bo), make_device_buf(bo1), make_device_buf(bo1), make_device_buf(bo))
        let sizes = fixed_array(bo, bo1, bo1, bo)
        let at <- staged_offsets(sizes)
        let host = make_host_buf(at[length(sizes)], true, [cached = true])
        for (o, bytes in outs, sizes) {
            nan_sentinel_upload(o, bytes)
        }
        var s_a = set_tts_pk_attn_cls(fixed_array(qd, kd, vd, outs[0]), fixed_array(bq, bk, bv, bo), fixed_array(0u, 0u, 0u, B_OA))
        var s_b = set_tts_pk_attn_cls(fixed_array(qd, kd, vd, outs[1]), fixed_array(bq, bk, bv, bo1), fixed_array(0u, 0u, 0u, B_OB))
        var s_x = set_tts_pk_attn_cls(fixed_array(qd, kxd, vxd, outs[2]), fixed_array(bq, bk, bv, bo1), fixed_array(0u, 0u, 0u, B_OX))
        var s_c = set_tts_pk_attn_cls(fixed_array(qd, kd, vd, outs[3]), fixed_array(bq, bk, bv, bo), fixed_array(0u, 0u, 0u, B_OC))
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
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_pk_attn_cls(raw, h, s_a, pa, tts_pk_attn_wgs(heads, int64(PA_T)))
            enc_tts_pk_attn_cls(raw, h, s_b, pb, tts_pk_attn_wgs(heads, 1l))
            enc_tts_pk_attn_cls(raw, h, s_x, pb, tts_pk_attn_wgs(heads, 1l))
            enc_tts_pk_attn_cls(raw, h, s_c, pc, tts_pk_attn_wgs(heads, int64(PA_T)))
            cmd_copy_planes(raw, h, outs, host, sizes, B_OA | B_OB | B_OX | B_OC)
        }
        out.a <- staged(host.mapped, at[0] + int64(PA_OOFF) * 4l, PA_T * d, type<float>)
        out.b <- staged(host.mapped, at[1] + int64(PA_OOFF) * 4l, d, type<float>)
        out.x <- staged(host.mapped, at[2] + int64(PA_OOFF) * 4l, d, type<float>)
        out.c <- staged(host.mapped, at[3] + int64(PA_OOFF) * 4l, PA_T * d, type<float>)
        out.pre <- staged(host.mapped, at[0], PA_OOFF, type<float>)
        for (dev in fixed_array(qd, kd, vd, kxd, vxd)) {
            destroy_device_buf(dev)
        }
        for (o in outs) {
            destroy_device_buf(o)
        }
        destroy_host_buf(host)
    }
    return <- out
}

//! one attention compare with its maxdiff logged and its poisoned element
def private pk_attn_check(t : T?; tag : string; got, want : array<float>) {
    let n = length(want)
    let bad = mismatch_approx(got, want)
    let mi = maxdiff_at(got, want)
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi / PA_D} head {(mi % PA_D) / PA_DH} col {mi % PA_DH}\n")
    t |> success(bad == 0, "{tag}: the device == the CPU attention_causal_rows ({bad} of {n} off)")
    t |> success(!approx(got[n - 1] + 0.5, want[n - 1]), "{tag}: the bar reds a poisoned element")
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
            pk_attn_check(t, "tts_pk_attn prompt t{PA_T} every key", got.a, want.a)
            pk_attn_check(t, "tts_pk_attn decode pos {PA_DEC_POS} ctx {PA_DEC_CTX}", got.b, want.b)
            pk_attn_check(t, "tts_pk_attn prompt t{PA_T} ctx {PA_WIN_CTX}", got.c, want.c)
            t |> equal(mismatch_exact(got.x, got.b), 0, "tts_pk_attn control: a poisoned key before the decode step's window stays silent, bit for bit")
            var moved = 0
            for (e in range(PA_T * d)) {
                if (!approx(got.a[e], got.c[e])) {
                    moved++
                }
            }
            t |> success(moved > 0, "tts_pk_attn control: the 16-key window moves the prompt's late rows off the every-key rows ({moved} apart)")
            t |> equal(nan_count(got.pre), PA_OOFF, "tts_pk_attn: the prefix before ooff keeps its sentinel")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the row GEMV =====

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

def private pk_gemv_fixture() : PkGemvFix {
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
    f.woff = put(f.slab, wst)
    f.boff = put(f.slab, f.lin.b)
    f.lwoff = put(f.slab, f.norm.w)
    f.lboff = put(f.slab, f.norm.b)
    f.aoff = put(f.slab, f.aux)
    f.slab |> push_from(poison_plane(GAP))
    return <- f
}

//! the CPU `modulate`: x * (1 + scale) + shift, the shift row first in mod
def private pk_modulate(var x : array<float>; mods : array<float>; n : int) {
    for (j in range(n)) {
        x[j] = x[j] * (1.0 + mods[n + j]) + mods[j]
    }
}

//! the CPU chain of each stamp: the linear over x or over the normed and modulated x, then the stamp's epilogue
def private pk_gemv_wants(f : PkGemvFix) : PkGemvRows {
    let nin = PG_NIN
    let nout = PG_NOUT
    var d_plain : array<float>
    var d_norm : array<float>
    with_job_que() {
        setup_dasllama_jobque_()
        linear_vec(f.lin, f.x, d_plain)
        var xn := f.x
        layernorm_rows(xn, 1l, int64(nin), f.norm, PG_EPS)
        pk_modulate(xn, f.mods, nin)
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

//! the five stamps over one command buffer: the y planes staged whole, the gated residual's over its input row, every
//! other under a NaN fill
def private pk_gemv_device(f : PkGemvFix) : PkGemvRows {
    var out : PkGemvRows
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let B_DOT = 1u
        let B_LN = 2u
        let B_GATE = 4u
        let B_ADD = 8u
        let B_TAIL = 16u
        let B_LAT = 32u
        let nout = PG_NOUT
        let ny = PG_YOFF + nout + SLACK
        let nlat = PG_LOFF + nout + SLACK
        let wnd = dev_upload(f.slab)
        let xd = dev_upload(with_base(f.x, PG_XOFF))
        let md = dev_upload(with_base(f.mods, PG_MOFF))
        let x0d = dev_upload(with_base(f.x0, PG_X0OFF))
        let ygd = dev_upload(with_base(f.y0, PG_YOFF))
        let bwn = long_length(f.slab) * 4l
        let bx = int64(PG_XOFF + length(f.x) + SLACK) * 4l
        let bm = int64(PG_MOFF + length(f.mods) + SLACK) * 4l
        let bx0 = int64(PG_X0OFF + length(f.x0) + SLACK) * 4l
        let by = int64(ny) * 4l
        let blat = int64(nlat) * 4l
        let outs = fixed_array(make_device_buf(by), make_device_buf(by), ygd, make_device_buf(by), make_device_buf(by), make_device_buf(blat))
        let sizes = fixed_array(by, by, by, by, by, blat)
        let at <- staged_offsets(sizes)
        let host = make_host_buf(at[length(sizes)], true, [cached = true])
        for (o, bytes in outs, sizes) {
            if (o != ygd) {
                nan_sentinel_upload(o, bytes)
            }
        }
        var s_dot = set_tts_pk_gemv_dot_cls(fixed_array(wnd, xd, outs[0]), fixed_array(bwn, bx, by), fixed_array(0u, 0u, B_DOT))
        var s_ln = set_tts_pk_gemv_ln_silu_cls(fixed_array(wnd, xd, outs[1], md), fixed_array(bwn, bx, by, bm), fixed_array(0u, 0u, B_LN, 0u))
        var s_gate = set_tts_pk_gemv_gate_cls(fixed_array(wnd, xd, ygd, md), fixed_array(bwn, bx, by, bm), fixed_array(0u, 0u, B_GATE, 0u))
        var s_add = set_tts_pk_gemv_add_silu_cls(fixed_array(wnd, xd, outs[3]), fixed_array(bwn, bx, by), fixed_array(0u, 0u, B_ADD))
        var s_tail = set_tts_pk_gemv_tail_cls(fixed_array(wnd, xd, outs[4], md, x0d, outs[5]), fixed_array(bwn, bx, by, bm, bx0, blat),
            fixed_array(0u, 0u, B_TAIL, 0u, 0u, B_LAT))
        var pa = TtsPkGemvArgs(nin = uint(PG_NIN), nout = uint(nout), stride = uint(PG_STRIDE), eps = PG_EPS, woff = uint(f.woff),
            boff = uint(f.boff), xoff = uint(PG_XOFF), yoff = uint(PG_YOFF), moff = uint(PG_MOFF), lwoff = uint(f.lwoff),
            lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = uint(PG_X0OFF), loff = uint(PG_LOFF))
        let wgs = tts_pk_gemv_wgs(int64(nout))
        cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_tts_pk_gemv_dot_cls(raw, h, s_dot, pa, wgs)
            enc_tts_pk_gemv_ln_silu_cls(raw, h, s_ln, pa, wgs)
            enc_tts_pk_gemv_gate_cls(raw, h, s_gate, pa, wgs)
            enc_tts_pk_gemv_add_silu_cls(raw, h, s_add, pa, wgs)
            enc_tts_pk_gemv_tail_cls(raw, h, s_tail, pa, wgs)
            cmd_copy_planes(raw, h, outs, host, sizes, B_DOT | B_LN | B_GATE | B_ADD | B_TAIL | B_LAT)
        }
        out.dot <- staged(host.mapped, at[0], ny, type<float>)
        out.ln <- staged(host.mapped, at[1], ny, type<float>)
        out.gate <- staged(host.mapped, at[2], ny, type<float>)
        out.add <- staged(host.mapped, at[3], ny, type<float>)
        out.tail <- staged(host.mapped, at[4], ny, type<float>)
        out.lat <- staged(host.mapped, at[5], nlat, type<float>)
        for (dev in fixed_array(wnd, xd, md, x0d)) {
            destroy_device_buf(dev)
        }
        for (o in outs) {
            destroy_device_buf(o)
        }
        destroy_host_buf(host)
    }
    return <- out
}

//! the output row out of a staged plane
def private pk_row(whole : array<float>; off : int) : array<float> {
    return <- [for (i in range(PG_NOUT)); whole[off + i]]
}

//! one GEMV stamp's row out of its staged plane against its CPU chain, the maxdiff logged, with its poisoned element
def private pk_gemv_check(t : T?; tag : string; whole : array<float>; off : int; want : array<float>) {
    let got <- pk_row(whole, off)
    let bad = mismatch_approx(got, want)
    let mi = maxdiff_at(got, want)
    to_log(LOG_INFO, "{tag}: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi}\n")
    t |> success(bad == 0, "{tag}: the device == the CPU chain ({bad} of {PG_NOUT} off)")
    t |> success(!approx(got[PG_NOUT - 1] + 0.5, want[PG_NOUT - 1]), "{tag}: the bar reds a poisoned element")
}

[test]
def test_vkt_pk_gemv(t0 : T?) {
    t0 |> run("the row GEMV's five stamps == the CPU linear_vec chains at nin 200 / nout 45, the slab's runs at offsets") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_gemv_dot_cls() || !ensure_tts_pk_gemv_ln_silu_cls() || !ensure_tts_pk_gemv_gate_cls()
                    || !ensure_tts_pk_gemv_add_silu_cls() || !ensure_tts_pk_gemv_tail_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            var inscope f <- pk_gemv_fixture()
            var inscope want <- pk_gemv_wants(f)
            var inscope got <- pk_gemv_device(f)
            pk_gemv_check(t, "tts_pk_gemv_dot", got.dot, PG_YOFF, want.dot)
            pk_gemv_check(t, "tts_pk_gemv_ln_silu", got.ln, PG_YOFF, want.ln)
            pk_gemv_check(t, "tts_pk_gemv_gate", got.gate, PG_YOFF, want.gate)
            pk_gemv_check(t, "tts_pk_gemv_add_silu", got.add, PG_YOFF, want.add)
            pk_gemv_check(t, "tts_pk_gemv_tail", got.tail, PG_YOFF, want.tail)
            pk_gemv_check(t, "tts_pk_gemv_tail latent", got.lat, PG_LOFF, want.lat)
            t |> success(got.gate[PG_YOFF] != f.y0[0], "tts_pk_gemv_gate in place control: the residual row is moved off its input")
            let ny = length(got.dot)
            t |> equal(nan_count(got.dot) + nan_count(got.ln) + nan_count(got.add) + nan_count(got.tail), 4 * (ny - PG_NOUT),
                "tts_pk_gemv: the slots before yoff and the masked rows past nout keep their sentinel")
            t |> equal(nan_count(got.lat), PG_LOFF + SLACK, "tts_pk_gemv_tail: the latent plane outside its row keeps its sentinel")
            let gate_out <- [for (i in range(ny)); got.gate[i]; where i < PG_YOFF || i >= PG_YOFF + PG_NOUT]
            t |> success(elem_all(gate_out, POISON), "tts_pk_gemv_gate: the residual plane outside its row is untouched")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the rows rope =====

let private PO_T = 23
let private PO_D = 128
let private PO_DH = 64
let private PO_QS = 384        //! q, k and v side by side: the rope turns the k span alone
let private PO_POS0 = 37
let private PO_CAP = 64        //! the tables' positions, past the last row's
let private PO_XOFF = 5
let private PO_THETA = 10000.0

[test]
def test_vkt_pk_rope(t0 : T?) {
    t0 |> run("the rows rope == the CPU rope_rows on the k span of t 23 rows at pos0 37, the q and v spans untouched") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_tts_pk_rope_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let B_X = 1u
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
            let coff = put(slab, cos_tab)
            let soff = put(slab, sin_tab)
            let kspan <- [for (e in range(nk)); x[(e / PO_D) * PO_QS + PO_D + e % PO_D]]
            var want := kspan
            rope_rows(want, int64(PO_T), int64(PO_D), int64(PO_DH), int64(PO_POS0), PO_THETA)
            let xp <- with_base(x, PO_XOFF)
            let xd = dev_upload(xp)
            let wnd = dev_upload(slab)
            let bx = long_length(xp) * 4l
            let sizes = fixed_array(bx)
            let at <- staged_offsets(sizes)
            let host = make_host_buf(at[length(sizes)], true, [cached = true])
            var s = set_tts_pk_rope_cls(fixed_array(xd, wnd), fixed_array(bx, long_length(slab) * 4l), fixed_array(B_X, 0u))
            var pa = TtsPkRopeArgs(rows = uint(PO_T), qs = uint(PO_QS), col = uint(PO_D), d = uint(PO_D), dh = uint(PO_DH), pos0 = uint(PO_POS0),
                xoff = uint(PO_XOFF), coff = uint(coff), soff = uint(soff))
            cmd_one_shot() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_pk_rope_cls(raw, h, s, pa, tts_pk_rope_wgs(int64(nk / 2)))
                cmd_copy_planes(raw, h, fixed_array(xd), host, sizes, B_X)
            }
            let whole <- staged(host.mapped, at[0], length(xp), type<float>)
            let got <- [for (e in range(nk)); whole[PO_XOFF + (e / PO_D) * PO_QS + PO_D + e % PO_D]]
            let bad = mismatch_approx(got, want)
            let mi = maxdiff_at(got, want)
            to_log(LOG_INFO, "tts_pk_rope: max |gpu - cpu| {abs(got[mi] - want[mi])} at row {mi / PO_D} head {(mi % PO_D) / PO_DH} col {mi % PO_DH}\n")
            t |> success(bad == 0, "tts_pk_rope: the k span == the CPU rope_rows ({bad} of {nk} off)")
            t |> success(!approx(got[nk - 1] + 0.5, want[nk - 1]), "tts_pk_rope: the bar reds a poisoned element")
            t |> success(got[2] != kspan[2], "tts_pk_rope in place control: a pair at a nonzero angle is turned off its input")
            var kept = 0
            for (r in range(PO_T)) {
                for (j in range(PO_QS)) {
                    if ((j < PO_D || j >= 2 * PO_D) && whole[PO_XOFF + r * PO_QS + j] == x[r * PO_QS + j]) {
                        kept++
                    }
                }
            }
            t |> equal(kept, PO_T * 2 * PO_D, "tts_pk_rope: the q and v spans are untouched")
            let outside <- [for (i in range(length(whole))); whole[i]; where i < PO_XOFF || i >= PO_XOFF + n]
            t |> success(elem_all(outside, POISON), "tts_pk_rope: the prefix before xoff and the slack past the rows are untouched")
            destroy_device_buf(xd)
            destroy_device_buf(wnd)
            destroy_host_buf(host)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
