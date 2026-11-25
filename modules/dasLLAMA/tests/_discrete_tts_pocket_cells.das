options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

//! The case bodies the Pocket TTS family's kernel cells of both discrete homes run - `test_vulkan_tts_pocket_kernels.das`
//! over `_vkd_oracles`' `CellPlanes`, `test_cuda_tts_kernels.das` over `_cuda_cell`'s `CuPlanes` - each written once, generic
//! over the rig (`_discrete_cells.das` carries the form): the codec stream's row copies with and without the ELU, the layer
//! scale, the causal cached attention row over the caches' rows, the row GEMV's five stamps and the frame loop's four fused
//! stamps, and the rows rope; every written-only output under a NaN fill, every compare with its poisoned element.
module _discrete_tts_pocket_cells shared public

require dastest/testing_boost public
require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_pocket   // modulate: the flow head's adaLN, the GEMV cell's CPU chain
require dasllama/dasllama_rope
require dasllama/dasllama_math
require dasllama/dasllama_tower_tts   // tts_pkst_enc: the driver's dispatch of a stored row GEMV to its member's stamp
require daslib/jobque_boost
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require ?cuda dasllama/dasllama_cuda_classes_tts
require ?cuda _cuda_cell
require ?das_metal dasllama/dasllama_metal_classes_tts
require ?das_metal _metal_cell
require _compares
require _kernel_oracles   // cell_role
require _discrete_cells   // seats
require _discrete_tts_cells   // stored_mat: a matrix minted in a stored member, in the device layout

//! the frame loop's four fused f32 stamps built on `rl`'s home
def tts_pk_fused_ready(rl : auto(R)) : bool => rail_ensure_tts_pk_gemv_ln_qkv_cls(rl) && rail_ensure_tts_pk_gemv_add_scale_cls(rl) && rail_ensure_tts_pk_gemv_add_cls(rl) && rail_ensure_tts_pk_gemv_ln_gelu_cls(rl)

//! the row GEMV's five f32 stamps built on `rl`'s home
def tts_pk_gemv_ready(rl : auto(R)) : bool => rail_ensure_tts_pk_gemv_dot_cls(rl) && rail_ensure_tts_pk_gemv_ln_silu_cls(rl) && rail_ensure_tts_pk_gemv_gate_cls(rl) && rail_ensure_tts_pk_gemv_add_silu_cls(rl) && rail_ensure_tts_pk_gemv_tail_cls(rl)

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

//! the ELU twin's bar: one exp a negative element, the rest copied (read: 1.2e-7 on every home)
let private PK_ROWS_ELU_BARS = home_bars(Bar(rel = 1e-6, abs_floor = 1e-7, poison_scaled = true))

//! the window's elements out of a whole dst plane, row by row
def private pk_window(dst : array<float>) : array<float> =>
    <- [for (e in range(PR_ROWS * PR_WIDTH)); dst[(PR_DR0 + e / PR_WIDTH) * PR_DS + PR_DC + e % PR_WIDTH]]

//! the codec stream's row copies, plain and ELU, over a 7 x 13 strided window against the CPU window copy and `elu_rows`
def tts_pk_rows_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_pk_rows"
    let nwin = PR_ROWS * PR_WIDTH
    let ndst = PR_DROWS * PR_DS
    var inscope src <- row_fill(PR_SROWS * PR_SS, 41, 7, 0.05, 2.1)
    var inscope want <- [for (e in range(nwin)); src[(PR_SR0 + e / PR_WIDTH) * PR_SS + PR_SC + e % PR_WIDTH]]
    var inscope want_e := want
    want_e |> push(0.0)
    with_dasllama_jobque_() {
        elu_rows(want_e, int64(nwin + 1))
    }
    want_e |> resize(nwin)
    let sp = cp |> in_(src)
    let dp = cp |> out_(ndst)
    let ep = cp |> out_(ndst)
    var pa = GkPkRowsArgs(rows = uint(PR_ROWS), width = uint(PR_WIDTH), ss = uint(PR_SS), sc = uint(PR_SC), sr0 = uint(PR_SR0),
        ds = uint(PR_DS), dc = uint(PR_DC), dr0 = uint(PR_DR0))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_pk_rows_cls(rl, seats(fixed_array(sp, dp)), pa, int64(nwin))
        enc_tts_pk_rows_elu_cls(rl, seats(fixed_array(sp, ep)), pa, int64(nwin))
    }
    var inscope dst_p <- cp |> plane_run(dp)
    var inscope dst_e <- cp |> plane_run(ep)
    var inscope got <- pk_window(dst_p)
    var inscope got_e <- pk_window(dst_e)
    check_rows(t, "tts_pk_rows window vs the source rows", got, want, BAR_EXACT, PR_WIDTH)
    t |> equal(nan_count(dst_p), ndst - nwin, "tts_pk_rows: every element outside the window keeps its sentinel")
    check_rows(t, "tts_pk_rows_elu window vs the CPU elu_rows", got_e, want_e,
        cp |> home_bar(PK_ROWS_ELU_BARS), PR_WIDTH)
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

let private RS_T = 37
let private RS_C = 72
let private RS_SOFF = 9

//! the layer scale in place over 37 x 72 rows, the scale row at an offset, against the CPU `layer_scale_rows`, exact
def tts_pk_row_scale_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_pk_row_scale"
    let n = RS_T * RS_C
    var inscope x <- row_fill(n, 41, 7, 0.05, 2.1)
    var inscope scale <- row_fill(RS_C, 29, 3, 0.03, 1.2)
    var inscope want := x
    with_dasllama_jobque_() {
        layer_scale_rows(want, int64(RS_T), int64(RS_C), scale)
    }
    let xp = cp |> io_(x, 0, SLACK)
    let wp = cp |> in_(scale, RS_SOFF, SLACK)
    var pa = GkBiasActArgs(d = uint(RS_C), nelem = uint(n), boff = uint(RS_SOFF))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_pk_row_scale_cls(rl, seats(fixed_array(xp, wp)), pa, int64(n))
    }
    var inscope got <- cp |> plane_run(xp)
    check_rows(t, "tts_pk_row_scale vs the CPU layer_scale_rows", got, want, BAR_EXACT, RS_C)
    t |> success(got[0] != x[0], "tts_pk_row_scale in place control: the input row is overwritten")
    t |> success(outside_run(cp |> plane_whole(xp), 0, n, POISON), "tts_pk_row_scale: the slack past the rows is untouched")
}

let private PA_HEADS = 2
let private PA_DH = 64
let private PA_D = 128
let private PA_T = 45
let private PA_CAP = 300
let private PA_QS = 264      //! q's row stride: the query span sits at PA_QCOL among other columns
let private PA_QCOL = 8
let private PA_PROWS = 48    //! the device caches' rows: the 45 appended, then three poisoned rows no query may see
let private PA_DEC_POS = 44  //! the decode step's position: the last appended key
let private PA_DEC_CTX = 8
let private PA_WIN_CTX = 16
let private PA_LONG = 300    //! the long cache's keys: past one 256-key stride of the scores a row stages
let private PA_STRIDE = 256  //! that stride, the keys the long leg's control keeps

//! the attention row's bar: a softmax over at most 300 scores and its weighted sum of the values, each an f32 dot of 32 or 64
//! terms (read: 3.3e-7 on every home)
let private PK_ATTN_BARS = home_bars(Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true))

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

//! the cache as the device's `prows` rows [pos][d], the rows past the cache's and the row `blank` poisoned
def private pk_cache_rows(kc : TtsKvCache; blank : int64; prows : int = PA_PROWS) : PkCacheRows {
    var k <- poison_plane(prows * PA_D)
    var v <- poison_plane(prows * PA_D)
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

//! the four attention dispatches over the cache's rows and the q plane `qfull`, in one launch
def private pk_attn_device(t : T?; var cp : auto(C); kc : TtsKvCache; qfull : array<float>) : PkAttnOut {
    cp |> cell_clear()
    var out : PkAttnOut
    let d = PA_D
    var inscope rows <- pk_cache_rows(kc, -1l)
    var inscope rows_x <- pk_cache_rows(kc, int64(PA_DEC_POS - PA_DEC_CTX))
    let qp = cp |> in_(qfull, 0, SLACK)
    var inscope qdec <- copy_span(qfull, PA_DEC_POS * PA_QS, PA_QS)
    let qdp = cp |> in_(qdec, 0, SLACK)
    let kp = cp |> in_(rows.k, 0, SLACK)
    let vp = cp |> in_(rows.v, 0, SLACK)
    let kxp = cp |> in_(rows_x.k, 0, SLACK)
    let vxp = cp |> in_(rows_x.v, 0, SLACK)
    let oa = cp |> out_(PA_T * d)
    let ob = cp |> out_(d)
    let ox = cp |> out_(d)
    let oc = cp |> out_(PA_T * d)
    var pa = GkPkAttnArgs(t = uint(PA_T), qs = uint(PA_QS), qcol = uint(PA_QCOL), d = uint(d), pos0 = 0u, ctx = 0u, hs = uint(PA_DH))
    var pb = pa
    pb.t = 1u
    pb.pos0 = uint(PA_DEC_POS)
    pb.ctx = uint(PA_DEC_CTX)
    var pc = pa
    pc.ctx = uint(PA_WIN_CTX)
    let heads = int64(PA_HEADS)
    cp |> cell_launch(t, "tts_pk_attn") $(var rl) {
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qp, kp, vp, oa)), pa, tts_attn_wgs(int64(PA_T), heads))
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qdp, kp, vp, ob)), pb, tts_attn_wgs(1l, heads))
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qdp, kxp, vxp, ox)), pb, tts_attn_wgs(1l, heads))
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qp, kp, vp, oc)), pc, tts_attn_wgs(int64(PA_T), heads))
    }
    out.a <- cp |> plane_run(oa)
    out.b <- cp |> plane_run(ob)
    out.x <- cp |> plane_run(ox)
    out.c <- cp |> plane_run(oc)
    out.a_whole <- cp |> plane_whole(oa)
    return <- out
}

//! the causal cached attention row vs `attention_causal_rows` over every key, windows and 300 keys
def tts_pk_attn_case(t : T?; var cp : auto(C)) {
    let bar = cp |> home_bar(PK_ATTN_BARS)
    let d = PA_D
    var inscope qfull <- row_fill(PA_T * PA_QS, 41, 7, 0.02, 0.95)
    var inscope q <- [for (e in range(PA_T * d)); qfull[(e / d) * PA_QS + PA_QCOL + e % d]]
    var inscope kc : TtsKvCache
    var inscope want <- pk_attn_wants(kc, q)
    var inscope got <- pk_attn_device(t, cp, kc, qfull)
    check_rows(t, "tts_pk_attn prompt t{PA_T} every key vs the CPU attention_causal_rows", got.a,
        want.a, bar, PA_D, PA_DH)
    check_rows(t, "tts_pk_attn decode pos {PA_DEC_POS} ctx {PA_DEC_CTX} vs the CPU attention_causal_rows",
        got.b, want.b, bar, PA_D, PA_DH)
    check_rows(t, "tts_pk_attn prompt t{PA_T} ctx {PA_WIN_CTX} vs the CPU attention_causal_rows",
        got.c, want.c, bar, PA_D, PA_DH)
    t |> equal(mismatch_exact(got.x, got.b), 0, "tts_pk_attn control: a poisoned key before the decode step's window stays silent, bit for bit")
    var moved = 0
    for (e in range(PA_T * d)) {
        if (!approx(got.a[e], got.c[e])) {
            moved++
        }
    }
    t |> success(moved > 0, "tts_pk_attn control: the 16-key window moves the prompt's late rows off the every-key rows ({moved} apart)")
    t |> success(outside_run(got.a_whole, 0, PA_T * d, NAN_SENTINEL), "tts_pk_attn: the elements past the rows keep their sentinel")
    pk_attn_long_leg(t, cp)
}

//! the CPU row of one query at the last of `n` keys of the long fixture, every key in reach; `kc` left holding those keys
def private pk_attn_long_want(var kc : TtsKvCache; q : array<float>; n : int) : array<float> {
    let d = PA_D
    var inscope kr <- row_fill(n * d, 19, 5, 0.002, 0.097)   // small keys: the softmax spreads over every key
    var inscope vr <- [for (e in range(n * d)); float(e / d) * 0.01 + float(e % 7) * 0.1]   // a ramp over the positions: a key set's mean tells it apart
    var want : array<float>
    with_dasllama_jobque_() {
        kv_cache_init(kc, int64(PA_HEADS), int64(PA_DH), int64(PA_CAP))
        kv_cache_append(kc, kr, vr, int64(n), int64(d))
        attention_causal_rows(q, 1l, int64(d), 0l, kc, want)
    }
    return <- want
}

//! one decode step over `PA_LONG` keys, every key in reach - the scores a row stages past one `PA_STRIDE`-key stride - against
//! `attention_causal_rows`; the row over the first stride's keys alone lands outside the bar
def private pk_attn_long_leg(t : T?; var cp : auto(C)) {
    let bar = cp |> home_bar(PK_ATTN_BARS)
    let d = PA_D
    var inscope q <- row_fill(d, 43, 7, 0.02, 0.95)
    var inscope kc, kc_short : TtsKvCache
    var inscope want <- pk_attn_long_want(kc, q, PA_LONG)
    var inscope short <- pk_attn_long_want(kc_short, q, PA_STRIDE)
    t |> equal(kc.n, int64(PA_LONG), "tts_pk_attn long: the cache holds {PA_LONG} keys, past one {PA_STRIDE}-key stride")
    cp |> cell_clear()
    var inscope rows <- pk_cache_rows(kc, -1l, PA_LONG + 3)
    let qp = cp |> in_(q, 0, SLACK)
    let kp = cp |> in_(rows.k, 0, SLACK)
    let vp = cp |> in_(rows.v, 0, SLACK)
    let op = cp |> out_(d)
    var pa = GkPkAttnArgs(t = 1u, qs = uint(d), qcol = 0u, d = uint(d), pos0 = uint(PA_LONG - 1), ctx = 0u, hs = uint(PA_DH))
    cp |> cell_launch(t, "tts_pk_attn long") $(var rl) {
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qp, kp, vp, op)), pa, tts_attn_wgs(1l, int64(PA_HEADS)))
    }
    var inscope got <- cp |> plane_run(op)
    check_rows(t, "tts_pk_attn decode pos {PA_LONG - 1} over {PA_LONG} keys vs the CPU attention_causal_rows",
        got, want, bar, PA_D, PA_DH)
    check_exceeds(t, "tts_pk_attn control: the row over the first {PA_STRIDE} keys alone", got,
        short, bar)
}

//! the grouped-query form of the row: four query heads of 32 over two key-value heads (a small backbone's grouping in small), the
//! queries' span among other columns, the caches' rows the key-value width; the CPU `attention_causal_rows` reads a query head's keys
//! at its key-value head the same way
let private PGQ_HEADS = 4
let private PGQ_KV_HEADS = 2
let private PGQ_DH = 32
let private PGQ_D = 128        //! the queries' width, PGQ_HEADS x PGQ_DH
let private PGQ_KVD = 64       //! the caches' row width, PGQ_KV_HEADS x PGQ_DH
let private PGQ_T = 21
let private PGQ_CAP = 40
let private PGQ_QS = 200
let private PGQ_QCOL = 24

//! the attention row over grouped keys, four query heads of 32 over two key-value heads
def tts_pk_attn_grouped_case(t : T?; var cp : auto(C)) {
    let bar = cp |> home_bar(PK_ATTN_BARS)
    cp |> cell_clear()
    let d = PGQ_D
    let kvd = PGQ_KVD
    var inscope qfull <- row_fill(PGQ_T * PGQ_QS, 43, 5, 0.02, 0.93)
    var inscope q <- [for (e in range(PGQ_T * d)); qfull[(e / d) * PGQ_QS + PGQ_QCOL + e % d]]
    var inscope kr <- row_fill(PGQ_T * kvd, 19, 7, 0.02, 0.91)
    var inscope vr <- row_fill(PGQ_T * kvd, 59, 13, 0.04, 1.7)
    var inscope kc : TtsKvCache
    var inscope want_a, want_w : array<float>
    with_dasllama_jobque_() {
        kv_cache_init(kc, int64(PGQ_KV_HEADS), int64(PGQ_DH), int64(PGQ_CAP))
        kv_cache_append(kc, kr, vr, int64(PGQ_T), int64(kvd))
        attention_causal_rows(q, int64(PGQ_T), int64(d), 0l, kc, want_a)
        attention_causal_rows(q, int64(PGQ_T), int64(d), 8l, kc, want_w)
    }
    // the caches' rows as the device holds them: [pos][kv_heads dh], the rows past the appended ones poisoned
    var inscope krows <- poison_plane(PGQ_CAP * kvd)
    var inscope vrows <- poison_plane(PGQ_CAP * kvd)
    for (p in range(PGQ_T)) {
        for (h in range(PGQ_KV_HEADS)) {
            for (j in range(PGQ_DH)) {
                krows[p * kvd + h * PGQ_DH + j] = kc.k[int64(h * PGQ_DH + j) * int64(PGQ_CAP) + int64(p)]
                vrows[p * kvd + h * PGQ_DH + j] = kc.v[(int64(h * PGQ_CAP) + int64(p)) * int64(PGQ_DH) + int64(j)]
            }
        }
    }
    let qp = cp |> in_(qfull, 0, SLACK)
    let kp = cp |> in_(krows, 0, SLACK)
    let vp = cp |> in_(vrows, 0, SLACK)
    let oa = cp |> out_(PGQ_T * d)
    let ow = cp |> out_(PGQ_T * d)
    var pa = GkPkAttnArgs(t = uint(PGQ_T), qs = uint(PGQ_QS), qcol = uint(PGQ_QCOL), d = uint(d), pos0 = 0u, ctx = 0u, hs = uint(PGQ_DH),
        kvs = uint(kvd))
    var pw = pa
    pw.ctx = 8u
    cp |> cell_launch(t, "tts_pk_attn grouped") $(var rl) {
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qp, kp, vp, oa)), pa, tts_attn_wgs(int64(PGQ_T), int64(PGQ_HEADS)))
        enc_tts_pk_attn_cls(rl, seats(fixed_array(qp, kp, vp, ow)), pw, tts_attn_wgs(int64(PGQ_T), int64(PGQ_HEADS)))
    }
    var inscope got_a <- cp |> plane_run(oa)
    var inscope got_w <- cp |> plane_run(ow)
    check_rows(t, "tts_pk_attn grouped t{PGQ_T} every key vs the CPU attention_causal_rows", got_a,
        want_a, bar, d, PGQ_DH)
    check_rows(t, "tts_pk_attn grouped t{PGQ_T} ctx 8 vs the CPU attention_causal_rows", got_w,
        want_w, bar, d, PGQ_DH)
    check_exceeds(t, "tts_pk_attn grouped control: the 8-key window moves the late rows off the every-key rows",
        got_w, got_a, bar)
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
    for (o in range64(f.lin.nout)) {
        linear_weight_row(f.lin, o, wst, o * int64(PG_STRIDE))
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
    let nin = int(f.lin.nin)
    let nout = PG_NOUT
    var d_plain, d_norm : array<float>
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

//! the five stamps in one launch, the y planes read back whole: the gated residual's over its input row, the rest under a NaN fill
def private pk_gemv_device(t : T?; var cp : auto(C); f : PkGemvFix) : PkGemvRows {
    cp |> cell_clear()
    var out : PkGemvRows
    let nout = PG_NOUT
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
    var pa = GkPkGemvArgs(nin = uint(PG_NIN), nout = uint(nout), stride = uint(PG_STRIDE), eps = PG_EPS, woff = uint(f.woff),
        boff = uint(f.boff), xoff = uint(PG_XOFF), yoff = uint(PG_YOFF), moff = uint(PG_MOFF), lwoff = uint(f.lwoff),
        lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = uint(PG_X0OFF), loff = uint(PG_LOFF), x0mul = 1.0)
    let rows = int64(nout)
    cp |> cell_launch(t, "tts_pk_gemv") $(var rl) {
        enc_tts_pk_gemv_dot_cls(rl, seats(fixed_array(wp, xp, dotp)), pa, rows)
        enc_tts_pk_gemv_ln_silu_cls(rl, seats(fixed_array(wp, xp, lnp, mp)), pa, rows)
        enc_tts_pk_gemv_gate_cls(rl, seats(fixed_array(wp, xp, gatep, mp)), pa, rows)
        enc_tts_pk_gemv_add_silu_cls(rl, seats(fixed_array(wp, xp, addp)), pa, rows)
        enc_tts_pk_gemv_tail_cls(rl, seats(fixed_array(wp, xp, tailp, mp, x0p, latp)), pa, rows)
    }
    out.dot <- cp |> plane_whole(dotp)
    out.ln <- cp |> plane_whole(lnp)
    out.gate <- cp |> plane_whole(gatep)
    out.add <- cp |> plane_whole(addp)
    out.tail <- cp |> plane_whole(tailp)
    out.lat <- cp |> plane_whole(latp)
    return <- out
}

//! the row GEMV's bar at `nin` inputs: nin products under 0.25 summed at f32 on both sides in different orders, each sum within
//! nin x 1.5e-8 of the exact one, the bar ten times that (read at nin 200: 2.4e-6 on every home; at 256 over stored rows:
//! Metal 5.1e-6, Vulkan and CUDA 4.1e-6)
def private pk_gemv_bars(nin : int) : HomeBars =>
    home_bars(Bar(rel = 2e-7 * float(nin), abs_floor = 1e-6, poison_scaled = true))

//! the five stamps over one fixture against their CPU chains, `arm` naming the dot's path
def private pk_gemv_arm(t : T?; var cp : auto(C); arm : string; quad : bool) {
    var inscope f <- pk_gemv_fixture(quad)
    t |> equal(f.woff % 4 == 0, quad, "{arm}: the weight rows sit on the lattice the arm names")
    var inscope want <- pk_gemv_wants(f)
    var inscope got <- pk_gemv_device(t, cp, f)
    pk_gemv_checks(t, arm, f, want, got, cp |> home_bar(pk_gemv_bars(PG_NIN)))
}

//! the five stamps' rows against their CPU chains, the gated residual moved off its input, every slot outside a row kept
def private pk_gemv_checks(t : T?; arm : string; f : PkGemvFix; want, got : PkGemvRows; bar : Bar) {
    check_rows(t, "{arm} tts_pk_gemv_dot vs the CPU chain", copy_span(got.dot, PG_YOFF, PG_NOUT),
        want.dot, bar, 1)
    check_rows(t, "{arm} tts_pk_gemv_ln_silu vs the CPU chain", copy_span(got.ln, PG_YOFF, PG_NOUT),
        want.ln, bar, 1)
    check_rows(t, "{arm} tts_pk_gemv_gate vs the CPU chain", copy_span(got.gate, PG_YOFF, PG_NOUT),
        want.gate, bar, 1)
    check_rows(t, "{arm} tts_pk_gemv_add_silu vs the CPU chain",
        copy_span(got.add, PG_YOFF, PG_NOUT), want.add, bar, 1)
    check_rows(t, "{arm} tts_pk_gemv_tail vs the CPU chain", copy_span(got.tail, PG_YOFF, PG_NOUT),
        want.tail, bar, 1)
    check_rows(t, "{arm} tts_pk_gemv_tail latent vs the CPU chain",
        copy_span(got.lat, PG_LOFF, PG_NOUT), want.lat, bar, 1)
    t |> success(got.gate[PG_YOFF] != f.y0[0], "{arm} tts_pk_gemv_gate in place control: the residual row is moved off its input")
    let ny = length(got.dot)
    t |> equal(nan_count(got.dot) + nan_count(got.ln) + nan_count(got.add) + nan_count(got.tail), 4 * (ny - PG_NOUT),
        "{arm} tts_pk_gemv: the slots before yoff and the masked rows past nout keep their sentinel")
    t |> equal(nan_count(got.lat), PG_LOFF + SLACK, "{arm} tts_pk_gemv_tail: the latent plane outside its row keeps its sentinel")
    t |> success(outside_run(got.gate, PG_YOFF, PG_NOUT, POISON), "{arm} tts_pk_gemv_gate: the residual plane outside its row is untouched")
}

//! the row GEMV's five stamps at nin 200 / nout 45 against the CPU `linear_vec` chains, the weight rows off and on the four lattice
def tts_pk_gemv_case(t : T?; var cp : auto(C)) {
    pk_gemv_arm(t, cp, "float rows", false)
    pk_gemv_arm(t, cp, "float4 rows", true)
}

let private PGS_NIN = 256   //! a stored row: one K-quant superblock, eight Q8_0 blocks

//! the stored GEMV's operands over the minted weight rows `m`: the CPU chain's linear over the rows the reader widens them to,
//! the f32 slab holding the bias, the norm rows and the slab vector alone
def private pk_gemv_stored_fixture(m : StoredMat) : PkGemvFix {
    let nin = PGS_NIN
    let nout = PG_NOUT
    var f <- PkGemvFix(lin <- TtsLinear(w := m.rows, b <- row_fill(nout, 29, 11, 0.03, 1.4), nin = int64(nin), nout = int64(nout)),
        norm <- TtsNorm(w <- row_fill(nin, 29, 5, 0.01, -0.5), b <- row_fill(nin, 31, 2, 0.02, 0.9)),
        aux <- row_fill(2 * nout, 37, 5, 0.03, 1.3), x <- row_fill(nin, 41, 7, 0.05, 2.1),
        mods <- row_fill(2 * nin + nout, 31, 7, 0.01, 0.5), x0 <- row_fill(nout, 43, 13, 0.04, 1.8),
        y0 <- row_fill(nout, 47, 17, 0.03, 1.1))
    f.boff = put_run(f.slab, f.lin.b)
    f.lwoff = put_run(f.slab, f.norm.w)
    f.lboff = put_run(f.slab, f.norm.b)
    f.aoff = put_run(f.slab, f.aux)
    f.slab |> push_from(poison_plane(SLAB_GAP))
    return <- f
}

//! the stored row GEMV's stamps built on `rl`'s home: every kind of every member, and the home's own-layout ones
def tts_pkst_ready(rl : auto(R)) : bool => tts_pkst_ensure(rl) && rail_tts_sites_ensure(rl)

//! one stored row GEMV of `kind` into `y` through the driver's dispatch - the member's stamp, or under `own` the home's own-layout one
def private pkst_cell_enc(var rl : auto(R); base : TtsPkStSite; kind : int; y : RailBuf; pa : GkPkGemvArgs; rows : int64; own : bool) {
    var s = base
    s.kind = kind
    s.y = y
    if (own) {
        if (!rail_tts_pk_own(rl, s, pa, rows)) {
            panic("this home has no own-layout row GEMV")
        }
    } else {
        tts_pkst_enc(rl, s, pa, rows)
    }
}

//! the five stored stamps in one launch over the member's quant and scale planes, the y planes read back whole
def private pk_gemv_stored_device(t : T?; var cp : auto(C); f : PkGemvFix; m : StoredMat; fmt : int; own : bool) : PkGemvRows {
    cp |> cell_clear()
    var out : PkGemvRows
    let nout = PG_NOUT
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
    let qp = cp |> in_plain(m.wq)
    let sp = cp |> in_plain(m.ws)
    let pa = GkPkGemvArgs(nin = uint(PGS_NIN), nout = uint(nout), stride = uint(PGS_NIN), eps = PG_EPS, woff = 0u,
        boff = uint(f.boff), xoff = uint(PG_XOFF), yoff = uint(PG_YOFF), moff = uint(PG_MOFF), lwoff = uint(f.lwoff),
        lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = uint(PG_X0OFF), loff = uint(PG_LOFF), x0mul = 1.0)
    let rows = int64(nout)
    cp |> cell_launch(t, "tts_pkst") $(var rl) {
        let at = $(pl : int) => rail_buf(rl, cell_role(pl))   // read before the first dispatch re-seats the rail's first roles
        let ys = fixed_array(at(dotp), at(lnp), at(gatep), at(addp), at(tailp))
        let base = TtsPkStSite(fmt = fmt, rows = at(wp), x = at(xp), mod = at(mp), x0 = at(x0p), lat = at(latp), wq = at(qp), ws = at(sp))
        pkst_cell_enc(rl, base, TTS_PK_GEMV_DOT, ys[0], pa, rows, own)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_LN_SILU, ys[1], pa, rows, own)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_GATE, ys[2], pa, rows, own)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_ADD_SILU, ys[3], pa, rows, own)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_TAIL, ys[4], pa, rows, own)
    }
    out.dot <- cp |> plane_whole(dotp)
    out.ln <- cp |> plane_whole(lnp)
    out.gate <- cp |> plane_whole(gatep)
    out.add <- cp |> plane_whole(addp)
    out.tail <- cp |> plane_whole(tailp)
    out.lat <- cp |> plane_whole(latp)
    return <- out
}

//! the row GEMV's five stored stamps over `member` rows vs the CPU chains; moved rows the control
def tts_pk_gemv_stored_case(t : T?; var cp : auto(C); member : string; own : bool = false) {
    let arm = "stored {member} rows{own ? " in the home's own layout" : ""}"
    var inscope w <- row_fill(PG_NOUT * PGS_NIN, 17, 3, 0.02, 0.97)
    var inscope m <- stored_mat(member, w, PG_NOUT, PGS_NIN, own)
    var inscope f <- pk_gemv_stored_fixture(m)
    var inscope want <- pk_gemv_wants(f)
    var inscope got <- pk_gemv_stored_device(t, cp, f, m, stored_site_fmt(member), own)
    let bar = cp |> home_bar(pk_gemv_bars(PGS_NIN))
    pk_gemv_checks(t, arm, f, want, got, bar)
    var inscope dot_off <- [for (o in range(PG_NOUT)); want.dot[(o + 1) % PG_NOUT] - f.lin.b[(o + 1) % PG_NOUT] + f.lin.b[o]]
    check_exceeds(t, "{arm} tts_pkst dot control: the chain with its weight rows moved down one lands off the device rows", copy_span(got.dot, PG_YOFF, PG_NOUT),
        dot_off, bar)
}

let private PO_T = 23
let private PO_D = 128
let private PO_DH = 64
let private PO_QS = 384        //! q, k and v side by side: the rope turns the k span alone
let private PO_POS0 = 37
let private PO_CAP = 64        //! the tables' positions, past the last row's
let private PO_THETA = 10000.0

//! the rope's bar: each element one f32 rotation by tabled angles, two products and a sum (read: 1.2e-7 on every home)
let private PK_ROPE_BARS = home_bars(Bar(rel = 1e-6, abs_floor = 1e-6, poison_scaled = true))

//! the rows rope on the k span of 23 fused qkv rows from position 37 against `rope_rows`, the q and v spans and the slack untouched
def tts_pk_rope_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_pk_rope"
    let n = PO_T * PO_QS
    let nk = PO_T * PO_D
    let half = PO_DH / 2
    var inscope x <- row_fill(n, 41, 7, 0.05, 2.1)
    var inscope cos_tab, sin_tab : array<float>
    let no_freqs : array<float>
    build_rope_tabs(cos_tab, sin_tab, PO_THETA, 1.0, 1.0, no_freqs, false, 0l, int64(PO_CAP), int64(PO_DH))
    t |> equal(length(cos_tab), PO_CAP * half, "tts_pk_rope: the tables carry {PO_CAP} positions of {half} angles")
    var inscope slab : array<float>
    let coff = put_run(slab, cos_tab) + PO_POS0 * half   // the tables bound at the first row's position: row r reads table row r
    let soff = put_run(slab, sin_tab) + PO_POS0 * half
    var inscope kspan <- [for (e in range(nk)); x[(e / PO_D) * PO_QS + PO_D + e % PO_D]]
    var inscope want := kspan
    rope_rows(want, int64(PO_T), int64(PO_D), int64(PO_DH), int64(PO_POS0), PO_THETA)
    let xp = cp |> io_(x, 0, SLACK)
    let wp = cp |> in_(slab)
    var pa = GkRopeTabArgs(rows = uint(PO_T), qs = uint(PO_QS), col = uint(PO_D), d = uint(PO_D), dh = uint(PO_DH), coff = uint(coff), soff = uint(soff))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_pk_rope_cls(rl, seats(fixed_array(xp, wp, wp)), pa, int64(nk / 2))
    }
    var inscope rows <- cp |> plane_run(xp)
    var inscope got <- [for (e in range(nk)); rows[(e / PO_D) * PO_QS + PO_D + e % PO_D]]
    check_rows(t, "tts_pk_rope k span vs the CPU rope_rows", got, want,
        cp |> home_bar(PK_ROPE_BARS), PO_D, PO_DH)
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
    for (o in range64(f.qkv.nout)) {
        linear_weight_row(f.qkv, o, wq, o * int64(PF_STRIDE))
    }
    var ws <- poison_plane(d * PF_STRIDE)
    for (o in range64(f.sq.nout)) {
        linear_weight_row(f.sq, o, ws, o * int64(PF_STRIDE))
    }
    f.woff_q = put_run(f.slab, wq)
    f.boff_q = put_run(f.slab, f.qkv.b)
    f.woff_s = put_run(f.slab, ws)
    f.boff_s = put_run(f.slab, f.sq.b)
    f.lwoff = put_run(f.slab, f.norm.w)
    f.lboff = put_run(f.slab, f.norm.b)
    f.aoff = put_run(f.slab, f.scale)
    f.slab |> push_from(poison_plane(SLAB_GAP))
    var cos_tab, sin_tab : array<float>
    let no_freqs : array<float>
    build_rope_tabs(cos_tab, sin_tab, PO_THETA, 1.0, 1.0, no_freqs, false, 0l, int64(PF_CAP), int64(PF_DH))
    f.coff = put_run(f.tabs, cos_tab)
    f.soff = put_run(f.tabs, sin_tab)
    return <- f
}

//! the CPU chains a fused fixture's stamps are held to: the normed qkv row with its q and k spans roped at the frame's position,
//! the residual row plus the square's row with and without the scale, the normed square's row through the GELU
struct private PkFusedWant {
    qkv_row : array<float>
    q : array<float>
    k : array<float>
    v : array<float>
    add_scale : array<float>
    add : array<float>
    gelu : array<float>
}

def private pk_fused_wants(f : PkFusedFix; d, dh : int) : PkFusedWant {
    var w : PkFusedWant
    var inscope xn := f.x
    var inscope dp : array<float>
    with_dasllama_jobque_() {
        layernorm_rows(xn, 1l, int64(d), f.norm, PG_EPS)
        linear_vec(f.qkv, xn, w.qkv_row)
        linear_vec(f.sq, f.x, dp)
        linear_vec(f.sq, xn, w.gelu)
    }
    w.q <- copy_span(w.qkv_row, 0, d)
    rope_rows(w.q, 1l, int64(d), int64(dh), int64(PF_P), PO_THETA)
    w.k <- copy_span(w.qkv_row, d, d)
    rope_rows(w.k, 1l, int64(d), int64(dh), int64(PF_P), PO_THETA)
    w.v <- copy_span(w.qkv_row, 2 * d, d)
    w.add_scale <- [for (o in range(d)); f.y0[o] + dp[o] * f.scale[o]]
    w.add <- [for (o in range(d)); f.y0[o] + dp[o]]
    gelu(w.gelu, int64(d))
    return <- w
}

//! what a fused launch read back: the q span, the caches, the two residual rows and the GELU row
struct private PkFusedGot {
    y : array<float>
    kc : array<float>
    vc : array<float>
    add_scale : array<float>
    add : array<float>
    gelu : array<float>
}

//! the six rows of a fused launch against the CPU chains, `stem` the stamps' name stem
def private pk_fused_checks(t : T?; stem : string; want : PkFusedWant; got : PkFusedGot; d : int) {
    check_rows(t, "{stem}_ln_qkv q vs the CPU chain", copy_span(got.y, 0, d), want.q, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "{stem}_ln_qkv k cache vs the CPU chain", copy_span(got.kc, PF_P * d, d), want.k, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "{stem}_ln_qkv v cache vs the CPU chain", copy_span(got.vc, PF_P * d, d), want.v, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "{stem}_add_scale vs the CPU chain", got.add_scale, want.add_scale, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "{stem}_add vs the CPU chain", got.add, want.add, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "{stem}_ln_gelu vs the CPU chain", got.gelu, want.gelu, BAR_APPROX, FLAT_ROWS)
}

//! the frame loop's fused stamps vs the CPU chains: the roped qkv row, both adds, the normed GELU
def tts_pk_gemv_fused_case(t : T?; var cp : auto(C)) {   // nolint:STYLE038 one fixture and one launch carry the wide and the grouped qkv forms together
    cp |> cell_clear()
    let d = PF_D
    var inscope f <- pk_fused_fixture()
    var inscope want <- pk_fused_wants(f, d, PF_DH)
    let nc = PF_CAP * d
    let wp = cp |> in_(f.slab)
    let xp = cp |> in_(f.x, PF_XOFF, SLACK)
    let tp = cp |> in_(f.tabs)
    let kcp = cp |> io_(poison_plane(nc))
    let vcp = cp |> io_(poison_plane(nc))
    let yasp = cp |> io_(f.y0, PF_YOFF, SLACK)
    let yap = cp |> io_(f.y0, PF_YOFF, SLACK)
    let yp = cp |> out_(3 * d, PF_YOFF, SLACK)
    let ygp = cp |> out_(d, PF_YOFF, SLACK)
    let kvd = PF_DH   //! the grouped-query form: one key-value head of PF_DH under the two query heads, nout = d + 2 kvd, the k and v spans read off the same weight rows
    let nout2 = d + 2 * kvd
    let nc2 = PF_CAP * kvd
    let kc2p = cp |> io_(poison_plane(nc2))
    let vc2p = cp |> io_(poison_plane(nc2))
    let y2p = cp |> out_(nout2, PF_YOFF, SLACK)
    var pq = GkPkGemvArgs(nin = uint(d), nout = uint(3 * d), stride = uint(PF_STRIDE), eps = PG_EPS, woff = uint(f.woff_q), boff = uint(f.boff_q),
        xoff = uint(PF_XOFF), yoff = uint(PF_YOFF), moff = 0u, lwoff = uint(f.lwoff), lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = 0u, loff = 0u,
        span = uint(d), dh = uint(PF_DH), p = uint(PF_P), coff = uint(f.coff), soff = uint(f.soff))
    var ps = pq
    ps.nout = uint(d)
    ps.woff = uint(f.woff_s)
    ps.boff = uint(f.boff_s)
    var pg = pq
    pg.nout = uint(nout2)
    cp |> cell_launch(t, "tts_pk_gemv fused") $(var rl) {
        enc_tts_pk_gemv_ln_qkv_cls(rl, seats(fixed_array(wp, xp, yp, tp, kcp, vcp)), pq, int64(3 * d))
        enc_tts_pk_gemv_add_scale_cls(rl, seats(fixed_array(wp, xp, yasp)), ps, int64(d))
        enc_tts_pk_gemv_add_cls(rl, seats(fixed_array(wp, xp, yap)), ps, int64(d))
        enc_tts_pk_gemv_ln_gelu_cls(rl, seats(fixed_array(wp, xp, ygp)), ps, int64(d))
        enc_tts_pk_gemv_ln_qkv_cls(rl, seats(fixed_array(wp, xp, y2p, tp, kc2p, vc2p)), pg, int64(nout2))
    }
    var inscope got <- PkFusedGot(y <- cp |> plane_run(yp), kc <- cp |> plane_run(kcp), vc <- cp |> plane_run(vcp), add_scale <- cp |> plane_run(yasp),
        add <- cp |> plane_run(yap), gelu <- cp |> plane_run(ygp))
    pk_fused_checks(t, "tts_pk_gemv", want, got, d)
    t |> success(outside_run(cp |> plane_whole(yp), PF_YOFF, d, NAN_SENTINEL), "tts_pk_gemv_ln_qkv: only the q span lands in y - the k and v spans and the slots outside keep their sentinel")
    var inscope other_rows <- [for (i in range(nc)); got.kc[i] + got.vc[i]; where i / d != PF_P]
    t |> success(elem_all(other_rows, 2.0 * POISON), "tts_pk_gemv_ln_qkv: the caches' other rows are untouched")
    t |> success(outside_run(cp |> plane_whole(ygp), PF_YOFF, d, NAN_SENTINEL), "tts_pk_gemv_ln_gelu: the slots outside the row keep their sentinel")
    t |> success(got.add_scale[0] != f.y0[0] && got.add[0] != f.y0[0], "the adds' in place control: the residual row is moved off its input")
    var inscope want_k2 <- copy_span(want.qkv_row, d, kvd)
    rope_rows(want_k2, 1l, int64(kvd), int64(PF_DH), int64(PF_P), PO_THETA)
    var inscope want_v2 <- copy_span(want.qkv_row, d + kvd, kvd)
    var inscope kc2 <- cp |> plane_run(kc2p)
    var inscope vc2 <- cp |> plane_run(vc2p)
    check_rows(t, "tts_pk_gemv_ln_qkv grouped q vs the CPU chain", copy_span(cp |> plane_run(y2p), 0, d), want.q, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "tts_pk_gemv_ln_qkv grouped k cache (one key-value head under two query heads) vs the CPU chain", copy_span(kc2, PF_P * kvd, kvd), want_k2, BAR_APPROX, FLAT_ROWS)
    check_rows(t, "tts_pk_gemv_ln_qkv grouped v cache vs the CPU chain", copy_span(vc2, PF_P * kvd, kvd), want_v2, BAR_APPROX, FLAT_ROWS)
    check_exceeds(t, "tts_pk_gemv_ln_qkv grouped control: the v row is the narrow span's rows, not the wide form's", copy_span(vc2, PF_P * kvd, kvd), copy_span(want.v, 0, kvd), BAR_APPROX)
    t |> success(outside_run(cp |> plane_whole(y2p), PF_YOFF, d, NAN_SENTINEL), "tts_pk_gemv_ln_qkv grouped: only the q span lands in y")
    var inscope other_rows2 <- [for (i in range(nc2)); kc2[i] + vc2[i]; where i / kvd != PF_P]
    t |> success(elem_all(other_rows2, 2.0 * POISON), "tts_pk_gemv_ln_qkv grouped: the caches' other rows are untouched")
}

let private PSF_D = 256    //! a stored row's width: one K-quant superblock, eight Q8_0 blocks
let private PSF_DH = 64    //! four heads a span

//! the four fused stored stamps over `member` rows (one leaf) vs the CPU chains; moved rows control
def tts_pk_gemv_stored_fused_case(t : T?; var cp : auto(C); member : string) {
    cp |> cell_clear()
    let d = PSF_D
    var inscope w <- row_fill(4 * d * d, 17, 3, 0.02, 0.97)
    var inscope m <- stored_mat(member, w, 4 * d, d)
    var inscope f <- PkFusedFix(qkv <- TtsLinear(w <- copy_span(m.rows, 0, 3 * d * d), b <- row_fill(3 * d, 29, 11, 0.03, 1.4), nin = int64(d), nout = int64(3 * d)),
        sq <- TtsLinear(w <- copy_span(m.rows, 3 * d * d, d * d), b <- row_fill(d, 23, 7, 0.03, 1.2), nin = int64(d), nout = int64(d)),
        norm <- TtsNorm(w <- row_fill(d, 29, 5, 0.01, -0.5), b <- row_fill(d, 31, 2, 0.02, 0.9)),
        x <- row_fill(d, 41, 7, 0.05, 2.1), y0 <- row_fill(d, 47, 17, 0.03, 1.1), scale <- row_fill(d, 37, 5, 0.03, 1.3))
    f.boff_q = put_run(f.slab, f.qkv.b)
    f.boff_s = put_run(f.slab, f.sq.b)
    f.lwoff = put_run(f.slab, f.norm.w)
    f.lboff = put_run(f.slab, f.norm.b)
    f.aoff = put_run(f.slab, f.scale)
    f.slab |> push_from(poison_plane(SLAB_GAP))
    var inscope cos_tab, sin_tab : array<float>
    let no_freqs : array<float>
    build_rope_tabs(cos_tab, sin_tab, PO_THETA, 1.0, 1.0, no_freqs, false, 0l, int64(PF_CAP), int64(PSF_DH))
    f.coff = put_run(f.tabs, cos_tab)
    f.soff = put_run(f.tabs, sin_tab)
    var inscope want <- pk_fused_wants(f, d, PSF_DH)
    let nc = PF_CAP * d
    let wp = cp |> in_(f.slab)
    let xp = cp |> in_(f.x, PF_XOFF, SLACK)
    let tp = cp |> in_(f.tabs)
    let kcp = cp |> io_(poison_plane(nc))
    let vcp = cp |> io_(poison_plane(nc))
    let yasp = cp |> io_(f.y0, PF_YOFF, SLACK)
    let yap = cp |> io_(f.y0, PF_YOFF, SLACK)
    let yp = cp |> out_(3 * d, PF_YOFF, SLACK)
    let ygp = cp |> out_(d, PF_YOFF, SLACK)
    let qp = cp |> in_plain(m.wq)
    let sp = cp |> in_plain(m.ws)
    let pq = GkPkGemvArgs(nin = uint(d), nout = uint(3 * d), stride = uint(d), eps = PG_EPS, woff = 0u, boff = uint(f.boff_q),
        xoff = uint(PF_XOFF), yoff = uint(PF_YOFF), moff = 0u, lwoff = uint(f.lwoff), lboff = uint(f.lboff), aoff = uint(f.aoff), x0off = 0u, loff = 0u,
        span = uint(d), dh = uint(PSF_DH), p = uint(PF_P), coff = uint(f.coff), soff = uint(f.soff))
    var ps = pq
    ps.nout = uint(d)
    ps.woff = uint(3 * d * d)   // the square's rows behind the qkv rows in the one leaf
    ps.boff = uint(f.boff_s)
    let fmt = stored_site_fmt(member)
    cp |> cell_launch(t, "tts_pkst fused") $(var rl) {
        let at = $(pl : int) => rail_buf(rl, cell_role(pl))   // read before the first dispatch re-seats the rail's first roles
        let ys = fixed_array(at(yp), at(yasp), at(yap), at(ygp))
        let base = TtsPkStSite(fmt = fmt, rows = at(wp), x = at(xp), tabs = at(tp), kc = at(kcp), vc = at(vcp), wq = at(qp), ws = at(sp))
        pkst_cell_enc(rl, base, TTS_PK_GEMV_LN_QKV, ys[0], pq, int64(3 * d), false)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_ADD_SCALE, ys[1], ps, int64(d), false)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_ADD, ys[2], ps, int64(d), false)
        pkst_cell_enc(rl, base, TTS_PK_GEMV_LN_GELU, ys[3], ps, int64(d), false)
    }
    var inscope got <- PkFusedGot(y <- cp |> plane_run(yp), kc <- cp |> plane_run(kcp), vc <- cp |> plane_run(vcp), add_scale <- cp |> plane_run(yasp),
        add <- cp |> plane_run(yap), gelu <- cp |> plane_run(ygp))
    let arm = "stored {member} rows"
    pk_fused_checks(t, "{arm} tts_pk_gemv_st", want, got, d)
    t |> success(outside_run(cp |> plane_whole(yp), PF_YOFF, d, NAN_SENTINEL), "{arm} tts_pkst ln_qkv: only the q span lands in y - the k and v spans and the slots outside keep their sentinel")
    var inscope other_rows <- [for (i in range(nc)); got.kc[i] + got.vc[i]; where i / d != PF_P]
    t |> success(elem_all(other_rows, 2.0 * POISON), "{arm} tts_pkst ln_qkv: the caches' other rows are untouched")
    var inscope as_off <- [for (o in range(d)); want.add_scale[(o + 1) % d]]
    check_exceeds(t, "{arm} tts_pkst add_scale control: the chain with the square's rows moved down one lands off the device row", got.add_scale, as_off, BAR_APPROX)
}
