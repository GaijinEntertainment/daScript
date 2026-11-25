options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

//! The case bodies the TTS kernel cells of both discrete homes run - `test_vulkan_tts_kernels.das` over `_vkd_oracles`'
//! `CellPlanes`, `test_cuda_tts_kernels.das` over `_cuda_cell`'s `CuPlanes` - each written once, generic over the rig
//! (`_discrete_cells.das` carries the form): the f32 tile GEMM and its stored members, the recurrence over its stored Q8_0
//! plane, and the PL-BERT, predictor and decoder row classes. The conv, Pocket and source cases sit in
//! `_discrete_tts_conv_cells.das`, `_discrete_tts_pocket_cells.das` and `_discrete_tts_source_cells.das`.
module _discrete_tts_cells shared public

require dastest/testing_boost public
require math
require daslib/math_boost   // round_up
require daslib/f16_cvt
require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_math   // f32_gemm_wgs, tts_tanh, sigmoid_f32
require dasllama/dasllama_gpu_kernels_discrete
require dasllama/dasllama_gpu_rail   // RailBuf, TTS_PK_ST_HALVES
require dasllama/dasllama_tower_tts   // tts_mm_stored_enc: the driver's own stored dispatch
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // the device layout's block strides
require dasllama/dasllama_layout   // q8_gather_rows, kq_gather_rows_dev: the discrete homes' device layout
require dasllama/dasllama_convert   // the quantizers and the CPU reader's rows
require dasllama/dasllama_tts_blocks   // q8_matrix_row and the CPU oracles
require dasllama/dasllama_math   // gelu, with_dasllama_jobque_
require daslib/jobque_boost
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require ?cuda dasllama/dasllama_cuda_classes
require ?cuda dasllama/dasllama_cuda_classes_tts
require ?cuda _cuda_cell
require ?das_metal dasllama/dasllama_metal_classes_tts
require ?das_metal _metal_cell
require _compares
require _kernel_oracles   // cell_role
require _tts_kernel_oracles   // gather_ref, concat_ref, sigsum_ref
require _discrete_cells   // seats

//! the exact tile's stored member stamps built on `rl`'s home
def tts_stored_gemm_ready(rl : auto(R)) : bool => tts_mm_stored_ensure(rl)

//! the column statistics' four stamps built on `rl`'s home
def tts_colstats_ready(rl : auto(R)) : bool => rail_ensure_tts_colpart_sum_cls(rl) && rail_ensure_tts_colreduce_mean_cls(rl) && rail_ensure_tts_colpart_sq_cls(rl) && rail_ensure_tts_colreduce_sq_cls(rl)

//! ... and the fused AdaIN pair over them
def tts_adain_ready(rl : auto(R)) : bool => tts_colstats_ready(rl) && rail_ensure_tts_adain_leaky_cls(rl) && rail_ensure_tts_adain_snake_cls(rl)

let private SM_ROWS = 37      //! positions: off the 64-position tile
let private SM_D = 70         //! weight rows: off the 64-row tile, so the last tile masks 58 rows
let private SM_K = 512        //! a row's elements: two superblocks, sixteen Q8_0 blocks
let private SM_N = 96         //! the columns multiplied where the x rows are narrower than a weight row
let private SM_LIFT_TAPS = 3  //! the transposed conv of the lift case ...
let private SM_LIFT_CIN = 64  //! ... its inputs, two 32-wide steps a tap ...
let private SM_LIFT_COUT = 10 //! ... and its outputs
let private LQ_T = 9          //! the recurrence's steps
let private LQ_H = 64         //! ... and its hidden units, on the 32 lattice

//! A [d][k] matrix minted in a stored member: its planes in the discrete homes' device layout and the rows the CPU reader widens them to
struct StoredMat {
    wq : array<uint8>
    ws : array<uint8>
    rows : array<float>
}

//! a quant member's format: the member words are `KqFmt`'s names
def stored_fmt(member : string) : KqFmt {
    var f = KqFmt.q8
    if (!kq_fmt_of_name(member, f)) {
        panic("stored member '{member}' names no KqFmt")
    }
    return f
}

//! `w` [d][k] minted in `member` by the engine's quantizers and gathered as the driver stages
//! a leaf; under `own` (`q8`, `k4`) in the Metal home's own layout
def stored_mat(member : string; w : array<float>; d, k : int; own : bool = false) : StoredMat {
    var m : StoredMat
    let n = int64(d) * int64(k)
    m.rows |> resize(d * k)
    if (member == "f16") {
        m.wq |> resize(d * k * 2)
        unsafe {
            var hp = addr<uint16?>(m.wq[0])
            for (i in range(d * k)) {
                hp[i] = uint16(f32_to_f16(w[i]))
                m.rows[i] = f16_to_f32(uint(hp[i]))
            }
        }
    } elif (member == "q8") {
        var inscope q : array<int8>
        var inscope qs : array<float>
        q |> resize(d * k)
        qs |> resize(d * k / 32)
        quantize_q8_0_into(w, n, q, qs, 0l, 0l)
        for (sc in qs) {   // a Q8_0 file's scale is a half, and so is either layout's: the reference reads the same value
            sc = f16_to_f32(f32_to_f16(sc))
        }
        m.wq |> resize(own ? d * k / 32 * 34 : d * k)
        m.ws |> resize(own ? 0 : d * k / 32 * 2)
        unsafe {
            if (own) {
                q8_region_to_metal_blob(addr<int8 const?>(q[0]), addr<float const?>(qs[0]), int64(k), int64(d), false, 1l, 1l, 0l, addr(m.wq[0]))
            } else {
                q8_gather_rows(addr<int8 const?>(q[0]), null, addr<float const?>(qs[0]), int64(k), int64(d), int64(d), 1l, 4l, 0l, m.wq, m.ws)
            }
        }
        for (r in range(d)) {
            q8_matrix_row(q, qs, false, int64(k), int64(d), int64(r), m.rows, int64(r * k))
        }
    } else {
        let f = stored_fmt(member)
        let nsb = n / KQ_SB_ELEMS
        var inscope kq, ks : array<uint8>
        var inscope no_site_words : array<uint>
        kq |> resize(nsb * kq_qsb(f))
        ks |> resize(nsb * kq_ssb(f))
        quantize_kq_plane(f, w, n, kq, ks, 0l)
        for (r in range(d)) {
            kq_plane_row_f32(f, kq, ks, int64(k), int64(d), 1l, int64(r), m.rows, int64(r * k))
        }
        if (own) {
            metal_kq_scale_form(f, ks)
            m.wq := kq
            m.ws := ks
        } else {
            m.wq |> resize(nsb * kq_qsb(f))
            m.ws |> resize(nsb * kq_dev_ssb(f))
            unsafe {
                kq_gather_rows_dev(f, addr<uint8 const?>(kq[0]), addr<uint8 const?>(ks[0]), int64(k), int64(d), int64(d), false, 1l, m.wq, m.ws, 0l, 0l, no_site_words)
            }
        }
    }
    m.wq |> resize(round_up(length(m.wq), 16))
    m.ws |> resize(round_up(max(length(m.ws), 16), 16))
    return <- m
}

//! y[rows x d] = x[rows x n] w^T (+ b) summed in k order, the tile's own order: w's row o, column k at `at(o, k)`
def stored_mm_ref(x, b : array<float>; rows, d, n, xstride : int; bias : bool; at : block<(o, k : int) : float>) : array<float> {
    var y : array<float>
    y |> resize(rows * d)
    for (p in range(rows)) {
        for (o in range(d)) {
            var acc = 0.0
            for (k in range(n)) {
                acc += x[p * xstride + k] * invoke(at, o, k)
            }
            y[p * d + o] = bias ? acc + b[o] : acc
        }
    }
    return <- y
}

//! a stored member's format as the driver's sites name it: its `KqFmt` slot, the halves' code for `f16`
def stored_site_fmt(member : string) : int => member == "f16" ? TTS_PK_ST_HALVES : int(stored_fmt(member))

//! the launch of member `member`'s stamp through the driver's own dispatch, over the planes at the roles: the quants, x, y,
//! the bias rows, the scales
def private stored_launch(var rl : auto(R); member : string; wq, x, y, b, ws : RailBuf; var pa : F32GemmArgs; wgs : int64) {
    tts_mm_stored_enc(rl, stored_site_fmt(member), wq, x, y, b, ws, pa, wgs)
}

//! the exact tile's stored member `member` vs the CPU reader's rows: a leaf off every tile
//! multiple, biased and not, then under x rows narrower than a weight row
def f32_stored_case(t : T?; var cp : auto(C); member : string) {
    cp |> cell_clear()
    let tag = "f32_gemm_{member}"
    let rows = SM_ROWS
    let d = SM_D
    let k = SM_K
    var inscope w <- row_fill(d * k, 17, 3, 0.04, 1.9)
    var inscope m <- stored_mat(member, w, d, k)
    var inscope x <- row_fill(rows * k, 41, 7, 0.05, 2.1)
    var inscope b <- row_fill(d, 29, 11, 0.06, 2.8)
    let wp = cp |> in_plain(m.wq)
    let sp = cp |> in_plain(m.ws)
    let xp = cp |> in_(x)
    let bp = cp |> in_(b)
    let ybp = cp |> out_(rows * d)
    let ynp = cp |> out_(rows * d)
    let ywp = cp |> out_(rows * d)
    let wgs = f32_gemm_wgs(int64(rows), int64(d))
    var pb = F32GemmArgs(n = uint(k), d = uint(d), rows = uint(rows), woff = 0u, wstride = uint(k), boff = 0u, xoff = 0u, xstride = uint(k), yoff = 0u, ystride = uint(d))
    var pn = pb
    pn.boff = F32G_NO_BIAS
    var pw = pb
    pw.n = uint(SM_N)
    cp |> cell_launch(t, tag) $(var rl) {
        let at = $(pl : int) => rail_buf(rl, cell_role(pl))   // read before the first dispatch re-seats the rail's first roles
        let pl = fixed_array(at(wp), at(xp), at(bp), at(sp), at(ybp), at(ynp), at(ywp))
        stored_launch(rl, member, pl[0], pl[1], pl[4], pl[2], pl[3], pb, wgs)
        stored_launch(rl, member, pl[0], pl[1], pl[5], pl[2], pl[3], pn, wgs)
        stored_launch(rl, member, pl[0], pl[1], pl[6], pl[2], pl[3], pw, wgs)
    }
    var inscope want_b <- stored_mm_ref(x, b, rows, d, k, k, true) $(o, kk) => m.rows[o * k + kk]
    var inscope want_n <- stored_mm_ref(x, b, rows, d, k, k, false) $(o, kk) => m.rows[o * k + kk]
    var inscope want_w <- stored_mm_ref(x, b, rows, d, SM_N, k, true) $(o, kk) => m.rows[o * k + kk]
    var inscope y_b <- cp |> plane_run(ybp)
    check_rows(t, "{tag}: the biased tile over the stored leaf vs the CPU reader's rows", y_b, want_b, BAR_APPROX, d)
    check_rows(t, "{tag}: no bias row named", cp |> plane_run(ynp), want_n, BAR_APPROX, d)
    check_rows(t, "{tag}: x rows narrower than a weight row", cp |> plane_run(ywp), want_w, BAR_APPROX, d)
    t |> equal(nan_count(y_b), 0, "{tag}: every output element is stored (no sentinel survives)")
}

//! the exact tile's stored member `member` over a transposed conv's lift [tap * cout + co][cin]
def f32_stored_lift_case(t : T?; var cp : auto(C); member : string) {
    cp |> cell_clear()
    let tag = "f32_gemm_{member} lift"
    let rows = SM_ROWS
    let taps = SM_LIFT_TAPS
    let cin = SM_LIFT_CIN
    let cout = SM_LIFT_COUT
    let k = member == "q8" || member == "f16" ? cin : round_up(cin, 256)
    let n = taps * cin
    var inscope w <- row_fill(taps * cout * k, 19, 5, 0.04, 1.7)
    for (r in range(taps * cout)) {   // a K-quant lift row's pad past cin holds zeros, as the mint pads it
        for (c in range(cin, k)) {
            w[r * k + c] = 0.0
        }
    }
    var inscope m <- stored_mat(member, w, taps * cout, k)
    var inscope x <- row_fill(rows * n, 43, 9, 0.05, 2.3)
    var inscope b <- row_fill(cout, 31, 13, 0.06, 2.6)
    let wp = cp |> in_plain(m.wq)
    let sp = cp |> in_plain(m.ws)
    let xp = cp |> in_(x)
    let bp = cp |> in_(b)
    let yp = cp |> out_(rows * cout)
    var pa = F32GemmArgs(n = uint(n), d = uint(cout), rows = uint(rows), woff = 0u, wstride = uint(k), boff = 0u, xoff = 0u, xstride = uint(n), yoff = 0u,
        ystride = uint(cout), tcin = uint(cin))
    cp |> cell_launch(t, tag) $(var rl) {
        let at = $(pl : int) => rail_buf(rl, cell_role(pl))
        stored_launch(rl, member, at(wp), at(xp), at(yp), at(bp), at(sp), pa, f32_gemm_wgs(int64(rows), int64(cout)))
    }
    var inscope want <- stored_mm_ref(x, b, rows, cout, n, n, true) $(o, kk) => m.rows[((kk / cin) * cout + o) * k + kk % cin]
    check_rows(t, "{tag}: the tile over the lift vs the CPU reader's lift rows", cp |> plane_run(yp), want, BAR_APPROX, cout)
}

//! one direction of the recurrence over its stored Q8_0 plane [4H][H] against the CPU loop over the rows the reader widens, forward and backward
def lstm_q8_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_lstm_dir_q8"
    let hd = LQ_H
    let steps = LQ_T
    var inscope w <- row_fill(4 * hd * hd, 23, 7, 0.01, 0.45)
    var inscope m <- stored_mat("q8", w, 4 * hd, hd)
    var inscope gates <- row_fill(steps * 4 * hd, 37, 5, 0.03, 1.4)
    let gp = cp |> in_(gates)
    let wp = cp |> in_plain(m.wq)
    let sp = cp |> in_plain(m.ws)
    let hfp = cp |> out_(steps * hd)
    let hbp = cp |> out_(steps * hd)
    var pf = TtsLstmArgs(t = uint(steps), hidden = uint(hd), goff = 0u, gstride = uint(4 * hd), hhoff = 0u, hoff = 0u, hstride = uint(hd), backward = 0u)
    var pb = pf
    pb.backward = 1u
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_lstm_dir_q8_cls(rl, seats(fixed_array(gp, wp, hfp, sp)), pf, 1l)
        enc_tts_lstm_dir_q8_cls(rl, seats(fixed_array(gp, wp, hbp, sp)), pb, 1l)
    }
    for (backward in range(2)) {
        var inscope want : array<float>
        want |> resize(steps * hd)
        var inscope hs, cs : array<float>
        hs |> resize(hd)
        cs |> resize(hd)
        var inscope hn : array<float>
        hn |> resize(hd)
        for (step in range(steps)) {
            let ti = backward != 0 ? steps - 1 - step : step
            for (j in range(hd)) {
                var g4 : float[4]
                for (g in range(4)) {
                    var a : float[4]
                    for (i in range(hd)) {
                        a[i & 3] += m.rows[(g * hd + j) * hd + i] * hs[i]
                    }
                    g4[g] = gates[ti * 4 * hd + g * hd + j] + ((a[0] + a[1]) + (a[2] + a[3]))
                }
                cs[j] = sigmoid_f32(g4[1]) * cs[j] + sigmoid_f32(g4[0]) * tts_tanh(g4[2])
                hn[j] = sigmoid_f32(g4[3]) * tts_tanh(cs[j])
                want[ti * hd + j] = hn[j]
            }
            for (j in range(hd)) {
                hs[j] = hn[j]
            }
        }
        check_rows(t, "{tag}: the {backward != 0 ? "backward" : "forward"} walk vs the CPU loop over the reader's rows", cp |> plane_run(backward != 0 ? hbp : hfp), want,
            BAR_APPROX, hd)
    }
}

let private G_ROWS = 37     //! positions: off the 64-position tile
let private G_D = 100       //! weight rows: off the 64-row tile, so the last tile masks 28 rows
let private G_N = 96        //! K: three 32-wide steps, the slab's lattice
let private G_WSTRIDE = 128 //! the slab's padded row: the pad past n never reads
let private G_XSTRIDE = 96

//! the f32 tile GEMM's two stamps, plain and biased, at 37 x 100 x 96 against the k-ordered sum, the slab pad past n poisoned
def f32_gemm_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "f32_gemm"
    let rows = G_ROWS
    let d = G_D
    let n = G_N
    var inscope x <- row_fill(rows * G_XSTRIDE, 41, 7, 0.05, 2.1)
    var inscope w <- row_fill(d * G_WSTRIDE, 17, 3, 0.04, 1.9)
    var inscope b <- row_fill(d, 29, 11, 0.06, 2.8)
    for (o in range(d)) {   //! the pad past n carries garbage: a read past n lands it in the sum
        for (k in range(n, G_WSTRIDE)) {
            w[o * G_WSTRIDE + k] = POISON
        }
    }
    let xp = cp |> in_(x)
    let wp = cp |> in_(w)
    let bp = cp |> in_(b)
    let yp = cp |> out_(rows * d)
    let ybp = cp |> out_(rows * d)
    var pa = F32GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, wstride = uint(G_WSTRIDE), boff = 0u,
        xoff = 0u, xstride = uint(G_XSTRIDE), yoff = 0u, ystride = uint(d))
    let wgs = f32_gemm_wgs(int64(rows), int64(d))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_f32_gemm_cls(rl, seats(fixed_array(wp, xp, yp)), pa, wgs)
        enc_f32_gemm_b_cls(rl, seats(fixed_array(wp, xp, ybp, bp)), pa, wgs)
    }
    var inscope y_dev <- cp |> plane_run(yp)
    var inscope want <- stored_mm_ref(x, b, rows, d, n, G_XSTRIDE, false) $(o, k) => w[o * G_WSTRIDE + k]
    var inscope want_b <- stored_mm_ref(x, b, rows, d, n, G_XSTRIDE, true) $(o, k) => w[o * G_WSTRIDE + k]
    check_rows(t, "f32_gemm plain stamp vs the k-ordered sum", y_dev, want, BAR_APPROX, d)
    check_rows(t, "f32_gemm_b biased stamp vs the sum plus the bias row", cp |> plane_run(ybp), want_b, BAR_APPROX, d)
    t |> equal(nan_count(y_dev), 0, "every output element is stored (no sentinel survives)")
}

let private TTS_REL = 1e-5      //! the f32 row classes' bar: the device's reassociation and transcendentals sit well inside it, the f16 LUT GELU does not

let private GA_C = 72       //! the row width: a row straddles the 256-element workgroups
let private GA_ROWS = 23
let private GA_VOCAB = 40
let private GA_WORD = 5     //! the word rows' element base in the slab
let private GA_OUT = 9      //! the output rows' element base: the prefix before it is never written

//! the embedding gather at element bases off zero against the CPU word + position + type sum, with both adds, neither and each alone
def tts_row_gather_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_row_gather"
    let c = GA_C
    let rows = GA_ROWS
    let total = rows * c
    let pos_off = GA_WORD + GA_VOCAB * c
    let type_off = pos_off + rows * c
    var inscope slab <- row_fill(type_off + c, 37, 5, 0.03, 1.4)
    var inscope ids <- [for (r in range(rows)); (r * 7 + 3) % GA_VOCAB]
    ids[1] = ids[0]   //! two positions on one word: only the position rows part them
    let sp = cp |> in_(slab)
    let ip = cp |> in_plain(ids)
    let op = cp |> out_(total, GA_OUT)
    var pa = GkRowGatherArgs(c = uint(c), rows = uint(rows), aoff = uint(GA_WORD), boff = uint(pos_off), coff = uint(type_off), out_off = uint(GA_OUT), add_b = 1u, add_c = 1u, scale = 1.0)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_row_gather_cls(rl, seats(fixed_array(sp, ip, sp, sp, op)), pa, int64(total))
    }
    var inscope got <- cp |> plane_run(op)
    var inscope want <- gather_ref(slab, ids, c, GA_WORD, pos_off, type_off)
    check_rows(t, "tts_row_gather vs the CPU word + position + type sum", got, want, BAR_EXACT, c)
    to_log(LOG_INFO, "tts_row_gather: the widest miss's row reads id {ids[maxdiff_at(got, want) / c]}\n")
    var inscope zero_ids : array<int>
    zero_ids |> resize(rows)
    t |> success(mismatch_exact(got, gather_ref(slab, zero_ids, c, GA_WORD, pos_off, type_off)) > 0, "tts_row_gather control: the rows follow their ids (the id-0 sum misses the device)")
    t |> success(mismatch_exact(subarray(got, range(c)), subarray(got, range(c, 2 * c))) > 0, "tts_row_gather control: two positions on one word differ by their position rows")
    t |> success(outside_run(cp |> plane_whole(op), GA_OUT, total, NAN_SENTINEL), "tts_row_gather: the prefix before out_off keeps its sentinel")
    cp |> plane_refill(op)
    pa.add_b = 0u
    pa.add_c = 0u
    cp |> cell_launch(t, "{tag} adds = 0") $(var rl) {
        enc_tts_row_gather_cls(rl, seats(fixed_array(sp, ip, sp, sp, op)), pa, int64(total))
    }
    var inscope got_plain <- cp |> plane_run(op)
    var inscope want_plain : array<float>
    want_plain |> resize(total)
    gather_rows(want_plain, subarray(slab, range(GA_WORD, pos_off)), ids, int64(c))
    check_rows(t, "tts_row_gather adds = 0 vs the word row alone", got_plain, want_plain, BAR_EXACT, c)
    t |> success(mismatch_exact(got_plain, want) > 0, "tts_row_gather adds = 0 control: the position rows are out of the sum")
    for (arm in range(2)) {   //! each add alone: the [rows x c] plane, then the [c] row
        cp |> plane_refill(op)
        pa.add_b = arm == 0 ? 1u : 0u
        pa.add_c = arm == 0 ? 0u : 1u
        cp |> cell_launch(t, "{tag} one add") $(var rl) {
            enc_tts_row_gather_cls(rl, seats(fixed_array(sp, ip, sp, sp, op)), pa, int64(total))
        }
        var inscope got_one <- cp |> plane_run(op)
        var inscope want_one <- [for (i in range(total)); want_plain[i] + (arm == 0 ? slab[pos_off + i] : slab[type_off + i % c])]
        check_rows(t, "tts_row_gather {arm == 0 ? "add_b" : "add_c"} alone vs the word row plus that add", got_one, want_one, BAR_EXACT, c)
        t |> success(mismatch_exact(got_one, want) > 0 && mismatch_exact(got_one, want_plain) > 0, "tts_row_gather control: one add alone lands between the plain and the full sums")
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

//! the tanh GELU in place over a run at an offset against the CPU `gelu` at the f32 bar, the outside untouched, the LUT form told apart
def tts_gelu_tanh_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_gelu_tanh"
    var inscope x <- elem_fixture()
    let xp = cp |> io_(x)
    var pa = TtsElemArgs(nelem = uint(EL_N), off = uint(EL_OFF), slope = 0.0)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_gelu_tanh_cls(rl, seats(fixed_array(xp)), pa, int64(EL_N))
    }
    var inscope got <- cp |> plane_run(xp)
    var inscope want := x
    gelu(unsafe(addr(want[EL_OFF])), int64(EL_N))
    check_rows(t, "tts_gelu_tanh vs the CPU gelu", copy_span(got, EL_OFF, EL_N), copy_span(want, EL_OFF, EL_N), Bar(rel = TTS_REL, poison = 1e-4), FLAT_ROWS)
    t |> success(outside_run(got, EL_OFF, EL_N, x), "tts_gelu_tanh: the elements outside the run are untouched")
    var lut_off = 0
    for (i in range(EL_OFF, EL_OFF + EL_N)) {
        if (!bar_ok(gelu_tanh_lut_f32(x[i]), want[i], TTS_REL)) {
            lut_off++
        }
    }
    t |> success(lut_off > 0, "tts_gelu_tanh control: the bar tells the f32 tanh form from the f16 LUT form ({lut_off} of {EL_N} apart)")
}

//! the leaky ReLU in place over a run at an offset against the CPU `leaky_relu`, exact, the outside untouched
def tts_leaky_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_leaky"
    var inscope x <- elem_fixture()
    let xp = cp |> io_(x)
    var pa = TtsElemArgs(nelem = uint(EL_N), off = uint(EL_OFF), slope = EL_SLOPE)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_leaky_cls(rl, seats(fixed_array(xp)), pa, int64(EL_N))
    }
    var inscope got <- cp |> plane_run(xp)
    var inscope run <- [for (i in range(EL_N)); x[EL_OFF + i]]
    leaky_relu(run, int64(EL_N), EL_SLOPE)
    check_rows(t, "tts_leaky vs the CPU leaky_relu", copy_span(got, EL_OFF, EL_N), run, BAR_EXACT, FLAT_ROWS)
    t |> success(outside_run(got, EL_OFF, EL_N, x), "tts_leaky: the elements outside the run are untouched")
    let neg = EL_OFF + 1
    t |> success(got[neg] == x[neg] * EL_SLOPE && got[neg] != x[neg], "tts_leaky control: a negative element is scaled by the slope")
    t |> success(got[EL_OFF] == x[EL_OFF], "tts_leaky control: a positive element passes unchanged")
}

let private BA_D = 12        //! the bias row's width
let private BA_ROWS = 434    //! 5208 elements: five whole 1024-element workgroups and 88 more
let private BA_BOFF = 3      //! the bias row's element base in its plane
let private BA_SLACK = 5     //! elements past the rows, left as they were

//! one `tower_bias_act` arm in place: the bias row at an element base added to every row, then `act`; exact against `want`
def private bias_act_arm(t : T?; var cp : auto(C); tag : string; x, wn, want : array<float>; act : uint) : array<float> {
    cp |> cell_clear()
    let nelem = BA_D * BA_ROWS
    let xp = cp |> io_(x)
    let wp = cp |> in_(wn)
    var pa = GkBiasActArgs(d = uint(BA_D), nelem = uint(nelem), boff = uint(BA_BOFF), act = act)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tower_bias_act_cls(rl, seats(fixed_array(xp, wp)), pa, (int64(nelem) / 4l + 255l) / 256l)
    }
    var inscope got <- cp |> plane_run(xp)
    check_rows(t, tag, copy_span(got, 0, nelem), want, BAR_EXACT, BA_D)
    t |> success(outside_run(got, 0, nelem, x), "{tag}: the elements past the rows are untouched")
    return <- got
}

//! the row-broadcast bias in place, bare and under relu, exact; the controls: the rows without
//! the bias, and the two arms against each other
def tower_bias_act_case(t : T?; var cp : auto(C)) {
    let nelem = BA_D * BA_ROWS
    var inscope x <- row_fill(nelem + BA_SLACK, 29, 5, 0.07, 3.1)   // sums of both signs: the relu arm has rows to floor
    var inscope wn <- row_fill(BA_BOFF + BA_D, 11, 3, 0.13, 0.4)
    var inscope want_none <- [for (i in range(nelem)); x[i] + wn[BA_BOFF + i % BA_D]]
    var inscope want_relu <- [for (v in want_none); max(v, 0.0)]
    var inscope got_none <- bias_act_arm(t, cp, "tower_bias_act, the bias row alone vs x + b", x, wn, want_none, BIAS_ACT_NONE)
    var inscope got_relu <- bias_act_arm(t, cp, "tower_bias_act, relu vs max(x + b, 0)", x, wn, want_relu, BIAS_ACT_RELU)
    t |> success(mismatch_exact(copy_span(got_none, 0, nelem), copy_span(x, 0, nelem)) > 0, "tower_bias_act control: the biased rows are off the input (the bias row is live)")
    t |> success(mismatch_exact(got_relu, got_none) > 0, "tower_bias_act control: relu moves the negative sums off the bias-alone rows")
}

//! one attention shape at element bases off zero, over the keys as they are and with the last key row zeroed, against `attention_rows`
def private tts_attn_arm(t : T?; var cp : auto(C); rows, heads, hs : int) {
    cp |> cell_clear()
    let tag = "tts_attn t{rows} {heads}x{hs}"
    let d = heads * hs
    let n = rows * d
    let qoff = 5
    let koff = 7
    let voff = 11
    let ooff = 13
    var inscope q <- row_fill(n, 41, 7, 0.02, 0.95)
    var inscope k <- row_fill(n, 17, 3, 0.02, 0.97)
    var inscope v <- row_fill(n, 53, 11, 0.04, 1.9)
    var inscope kz := k
    for (e in range(d)) {
        kz[(rows - 1) * d + e] = 0.0
    }
    let qp = cp |> in_(q, qoff)
    let kp = cp |> in_(k, koff)
    let kzp = cp |> in_(kz, koff)
    let vp = cp |> in_(v, voff)
    let op = cp |> out_(n, ooff)
    let ozp = cp |> out_(n, ooff)
    var pa = TtsAttnArgs(t = uint(rows), d = uint(d), hs = uint(hs), heads = uint(heads), scale = 1.0 / sqrt(float(hs)),
        qoff = uint(qoff), koff = uint(koff), voff = uint(voff), ooff = uint(ooff))
    let wgs = tts_attn_wgs(int64(rows), int64(heads))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_attn_cls(rl, seats(fixed_array(qp, kp, vp, op)), pa, wgs)
        enc_tts_attn_cls(rl, seats(fixed_array(qp, kzp, vp, ozp)), pa, wgs)
    }
    var inscope got <- cp |> plane_run(op)
    var inscope gotz <- cp |> plane_run(ozp)
    var inscope want, wantz : array<float>
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

//! the bidirectional attention at hs 64 and 128, one and two keys an invocation, against `attention_rows`
def tts_attn_case(t : T?; var cp : auto(C)) {
    with_dasllama_jobque_() {   //! attention_rows threads its heads
        tts_attn_arm(t, cp, 37, 4, 64)
        tts_attn_arm(t, cp, 130, 2, 128)
        tts_attn_arm(t, cp, 512, 2, 64)   //! the staged score row full: every invocation scores two keys
    }
}

let private CC_ROWS = 37
let private CC_C = 40
let private CC_RC = 13      //! the style row's columns: the duration encoder appends one row to every position
let private CC_F0S = 3      //! the F0 column's row stride
let private CC_NS = 2       //! the noise column's row stride
let private CC_WIDTH = 48   //! the padded arm's width: six zero columns past x, f0 and n

//! the concat rows against the CPU concat, exact: one style row broadcast, then f0 and n at strides with a zero pad
def tts_concat_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_concat"
    let rows = CC_ROWS
    let c = CC_C
    let wr = c + CC_RC
    var inscope x <- row_fill(rows * c, 41, 7, 0.05, 2.1)
    var inscope style <- row_fill(CC_RC, 29, 11, 0.06, 2.8)
    var inscope f0 <- row_fill(rows * CC_F0S, 17, 3, 0.04, 1.9)
    var inscope nd <- row_fill(rows * CC_NS, 53, 1, 0.11, 5.3)
    let xp = cp |> in_(x)
    let sp = cp |> in_(style)
    let fp = cp |> in_(f0)
    let np = cp |> in_(nd)
    let orp = cp |> out_(rows * wr)
    let opp = cp |> out_(rows * CC_WIDTH)
    var pr = GkConcatArgs(c = uint(c), rc = uint(CC_RC), rs = 0u, f0s = 0u, ns = 0u, with_res = 1u, width = uint(wr), rows = uint(rows))
    var pp = GkConcatArgs(c = uint(c), rc = uint(CC_RC), rs = uint(CC_RC), f0s = uint(CC_F0S), ns = uint(CC_NS), with_res = 0u,
        width = uint(CC_WIDTH), rows = uint(rows))
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_concat_cls(rl, seats(fixed_array(xp, sp, fp, np, orp)), pr, int64(rows * wr))
        enc_tts_concat_cls(rl, seats(fixed_array(xp, xp, fp, np, opp)), pp, int64(rows * CC_WIDTH))
    }
    var inscope got_p <- cp |> plane_run(opp)
    check_rows(t, "tts_concat with_res rs 0 vs the CPU concat", cp |> plane_run(orp), concat_ref(x, style, f0, nd, rows, c, CC_RC, 0, 0, 0, wr, true), BAR_EXACT, wr)
    check_rows(t, "tts_concat f0/n at strides, zero pad vs the CPU concat", got_p, concat_ref(x, style, f0, nd, rows, c, CC_RC, CC_RC, CC_F0S, CC_NS, CC_WIDTH, false), BAR_EXACT, CC_WIDTH)
    var inscope want_s1 <- concat_ref(x, style, f0, nd, rows, c, CC_RC, CC_RC, 1, 1, CC_WIDTH, false)
    t |> success(mismatch_exact(got_p, want_s1) > 0, "tts_concat control: the f0 and n columns follow their strides")
}

let private SG_T = 37
let private SG_NB = 50      //! the duration bins summed
let private SG_LS = 64      //! the logit rows' stride: the columns past SG_NB carry POISON
let private SG_SPEED = 1.3

//! the sums' bar: 50 sigmoids a row summed at f32 (read: 1.0e-7 on every home)
let private SIGSUM_BARS = home_bars(Bar(rel = 1e-5, abs_floor = 1e-6, poison_scaled = true))

//! the duration sigmoid sums over the first 50 of 64 logit columns against the CPU raw durations, the poisoned columns kept out
def tts_sigsum_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_sigsum"
    var inscope logits <- row_fill(SG_T * SG_LS, 41, 7, 0.1, 4.8)
    for (r in range(SG_T)) {
        for (j in range(SG_NB, SG_LS)) {
            logits[r * SG_LS + j] = POISON
        }
    }
    let lp = cp |> in_(logits)
    let rp = cp |> out_(SG_T)
    var pa = GkSigSumArgs(t = uint(SG_T), nb = uint(SG_NB), ls = uint(SG_LS), speed = SG_SPEED)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_sigsum_cls(rl, seats(fixed_array(lp, rp)), pa, int64(SG_T))
    }
    var inscope got <- cp |> plane_run(rp)
    check_rows(t, "tts_sigsum vs the CPU sum", got,
        sigsum_ref(logits, SG_T, SG_NB, SG_LS, SG_SPEED), cp |> home_bar(SIGSUM_BARS), 1)
    var inscope want_all <- sigsum_ref(logits, SG_T, SG_LS, SG_LS, SG_SPEED)
    t |> success(mismatch_approx(got, want_all) > 0, "tts_sigsum control: the columns past nb stay out of the sum")
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

//! the four column-stats passes over `xp` [rows][c] into `stp`'s stats rows, on the rail `rl`
def private colstats_launch(var rl : auto(R); xp, stp, rows, c : int) {
    let nblk = tts_stats_blocks(int64(rows))
    var pa = TtsStatsArgs(t = uint(rows), c = uint(c), nblk = uint(nblk), poff = uint(2 * c))
    enc_tts_colpart_sum_cls(rl, seats(fixed_array(xp, stp)), pa, nblk)
    enc_tts_colreduce_mean_cls(rl, seats(fixed_array(stp, stp)), pa, int64(c))
    enc_tts_colpart_sq_cls(rl, seats(fixed_array(xp, stp, stp)), pa, nblk)
    enc_tts_colreduce_sq_cls(rl, seats(fixed_array(stp, stp)), pa, int64(c))
}

//! one statistics shape: x [rows][c] off a mean, the stats plane under a NaN fill
def private tts_colstats_arm(t : T?; var cp : auto(C); rows, c : int) {
    cp |> cell_clear()
    let tag = "tts_colstats t{rows} c{c}"
    var inscope x <- row_fill(rows * c, 53, 11, 0.05, 1.3)
    let xp = cp |> in_(x)
    let stp = cp |> out_(int(tts_stats_floats(int64(rows), int64(c))))
    cp |> cell_launch(t, tag) $(var rl) {
        colstats_launch(rl, xp, stp, rows, c)
    }
    var inscope got <- subarray(cp |> plane_run(stp), range(2 * c))
    var inscope want <- colstats_ref(x, rows, c)
    // row 0 the means, row 1 the squared deviations; a sum of 300 of them sits past the relative bar's reach of 0.5, so the poison is a percent
    check_rows(t, "{tag} means and squared deviations vs the double-precision sums", got, want, Bar(rel = APPROX_REL, poison = 0.01, poison_scaled = true), c)
    t |> success(!approx(got[c - 1], want[c - 1] + 0.5), "{tag}: the bar reds a poisoned mean")
}

//! the four-pass column statistics at 45x72, 300x20 (a short last block) and 1000x300 (lanes taking two channels)
def tts_colstats_case(t : T?; var cp : auto(C)) {
    tts_colstats_arm(t, cp, 45, 72)
    tts_colstats_arm(t, cp, 300, 20)     //! three blocks, the last one short
    tts_colstats_arm(t, cp, 1000, 300)   //! eight blocks; lanes 0..43 take a second channel
}

let private AD_T = 45
let private AD_C = 72
let private AD_SD = 16      //! the style vector's width
let private AD_EPS = 1e-5
let private AD_HOFF = 7     //! the style rows' element base: the prefix before it is never read
let private AD_SLACK = 5    //! elements past the run, left at the sentinel
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

//! the fused AdaIN's bar: column statistics over 45 rows, an rsqrt and the style's affine, then the leaky ReLU or the Snake's
//! sin (read: Metal 1.4e-6, Vulkan and CUDA 9.5e-7)
let private ADAIN_BARS = home_bars(Bar(rel = 1e-4, abs_floor = 1e-5, poison_scaled = true))

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

//! the column stats then both fused AdaIN stamps against `adain_rows_into` plus leaky ReLU or Snake, and the leaky stamp in place
def tts_adain_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_adain"
    let c = AD_C
    let n = AD_T * c
    var inscope f <- adain_fixture()
    let x0p = cp |> in_(f.x)
    let xp = cp |> in_(f.x)
    let xip = cp |> io_(f.x, 0, AD_SLACK)
    let hp = cp |> in_(f.style_fc, AD_HOFF)
    let wp = cp |> in_(f.slab)
    let stp = cp |> out_(int(tts_stats_floats(int64(AD_T), int64(c))))
    let ylp = cp |> out_(n, 0, AD_SLACK)
    let ysp = cp |> out_(n, 0, AD_SLACK)
    static_assert(AD_EPS == GK_ADAIN_EPS, "the CPU oracle's eps is the stamps' literal")
    var pa = GkAdainArgs(c = uint(c), t = uint(AD_T), hoff = uint(AD_HOFF), gwoff = uint(AD_GWOFF), gboff = uint(f.gboff), aoff = uint(f.aoff))
    cp |> cell_launch(t, tag) $(var rl) {
        colstats_launch(rl, x0p, stp, AD_T, c)
        enc_tts_adain_leaky_cls(rl, seats(fixed_array(xp, ylp, stp, hp, wp)), pa, int64(n))
        enc_tts_adain_snake_cls(rl, seats(fixed_array(xp, ysp, stp, hp, wp)), pa, int64(n))
        enc_tts_adain_leaky_cls(rl, seats(fixed_array(xip, xip, stp, hp, wp)), pa, int64(n))
    }
    var inscope got_l <- cp |> plane_run(ylp)
    var inscope got_i <- cp |> plane_run(xip)
    let bar = cp |> home_bar(ADAIN_BARS)
    check_rows(t, "tts_adain_leaky vs the CPU adain_rows_into and leaky_relu", got_l, f.want_leaky,
        bar, AD_C)
    check_rows(t, "tts_adain_snake vs the CPU adain_rows_into and snake_rows", cp |> plane_run(ysp),
        f.want_snake, bar, AD_C)
    check_rows(t, "tts_adain_leaky in place vs the CPU adain_rows_into and leaky_relu", got_i,
        f.want_leaky, bar, AD_C)
    t |> success(got_i[0] != f.x[0], "tts_adain_leaky in place control: the input row is overwritten")
    t |> equal(mismatch_exact(got_i, got_l), 0, "tts_adain_leaky: in place == out of place, bit for bit")
    t |> success(outside_run(cp |> plane_whole(ylp), 0, n, NAN_SENTINEL) && outside_run(cp |> plane_whole(ysp), 0, n, NAN_SENTINEL),
        "tts_adain: the elements past the run keep their sentinel")
    t |> success(outside_run(cp |> plane_whole(xip), 0, n, POISON), "tts_adain in place: the elements past the run are untouched")
}

let private AS_N = 1030     //! four whole workgroups and six more
let private AS_SLACK = 5    //! elements past the run, left at the sentinel

//! the residual add-and-scale against (a + b) / sqrt(2) in f32, exact, out of place and in place
def tts_add_scale_case(t : T?; var cp : auto(C)) {
    cp |> cell_clear()
    let tag = "tts_add_scale"
    let n = AS_N
    let scale = 1.0 / sqrt(2.0)
    var inscope a <- row_fill(n, 41, 7, 0.05, 2.1)
    var inscope b <- row_fill(n, 17, 3, 0.04, 1.9)
    let ap = cp |> in_(a, 0)
    let aip = cp |> io_(a, 0, AS_SLACK)
    let bp = cp |> in_(b, 0)
    let op = cp |> out_(n, 0, AS_SLACK)
    var po = GkAddScaleArgs(nelem = uint(n), scale = scale)
    cp |> cell_launch(t, tag) $(var rl) {
        enc_tts_add_scale_cls(rl, seats(fixed_array(ap, bp, op)), po, int64(n))
        enc_tts_add_scale_cls(rl, seats(fixed_array(aip, bp, aip)), po, int64(n))
    }
    var inscope got_i <- cp |> plane_run(aip)
    var inscope want <- [for (i in range(n)); (a[i] + b[i]) * scale]
    check_rows(t, "tts_add_scale vs (a + b) * scale", cp |> plane_run(op), want, BAR_EXACT, FLAT_ROWS)
    check_rows(t, "tts_add_scale in place vs (a + b) * scale", got_i, want, BAR_EXACT, FLAT_ROWS)
    t |> success(got_i[0] != a[0], "tts_add_scale in place control: the input row is overwritten")
    t |> success(outside_run(cp |> plane_whole(op), 0, n, NAN_SENTINEL), "tts_add_scale: the elements outside the run keep their sentinel")
    t |> success(outside_run(cp |> plane_whole(aip), 0, n, POISON), "tts_add_scale in place: the elements past the run are untouched")
}
