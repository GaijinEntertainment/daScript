options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_par
require daslib/jobque_boost
require math
require _compares   // Sweep: the length sweeps' tally and verdict; opaque: the seeds' taint seam

//! The activation, norm and fp32 GEMM kernels - softmax_sink, swiglu_oai(4), softcap(4),
//! sigmoid_gate(4), silu_mul4, silu4_batch, l2_norm_rows and gemm_f32 / _jo / _uk_4x16 - against
//! closed forms and in-test fp64 references over shapes that reach every tail; and the scalar
//! kernels - rmsnorm, silu / gelu, the swiglu / geglu gates, softmax, parallel_argmax and hlse -
//! against hand-computed values.

let ACT_SWEEP <- [ 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65,
                   100, 127, 128, 129, 1023, 1024, 1025 ]
let L2_SHAPES <- [ 1, 1, 1, 7, 3, 8, 4, 64, 2, 127, 5, 33, 3, 128, 7, 4 ]
let GEMM_SHAPES <- [ 1, 1, 1, 2, 3, 5, 3, 7, 16, 4, 5, 16, 4, 16, 16, 5, 9, 17,
                     7, 4, 33, 8, 32, 48, 9, 13, 19, 4, 1, 16, 6, 2, 31, 16, 40, 64 ]

let ACT_POISON = 0.01
let XMAG = 4.0
let ZMAG = 8.0
let OAI_MAG = 56.0
let SENTINEL = 1.0e9
let L2_EPS = 1.0e-6

def private lcg_pm(n : int; seed : uint; lo, hi : float) : array<float> {
    var st = opaque(seed)
    var a : array<float>
    a |> resize(n)
    for (v in a) {
        st = st * 1664525u + 1013904223u
        let u01 = float(st >> 8u) * (1.0 / 16777216.0)
        let mag = lo + (hi - lo) * u01
        v = ((st >> 4u) & 1u) == 0u ? -mag : mag
    }
    return <- a
}

def private filled(n : int; v : float) : array<float> {
    var a : array<float>
    a |> resize(n)
    for (x in a) {
        x = v
    }
    return <- a
}

//! one swept length at `bar`, the poison leg `ACT_POISON` off the expectation
def private act_check(var r : Sweep; n : int; bar : float; got, want : array<float>) => sweep_check(r, n, bar, got, want, ACT_POISON)

//! `report_sweep`, and every element of the in-place run moved
def private report_moved(t : T?; name : string; r : Sweep) => report_sweep(t, name, r, 0, true)

def private softmax_bar(n : int) : float => 1.0e-6 + 4.0e-7 * float(n)

def private softmax_sink_ref(x : array<float>; sink : float) : tuple<array<float>; float> {
    var mx = double(sink)
    for (v in x) {
        mx = max(mx, double(v))
    }
    var terms : array<double>
    terms |> resize(length(x))
    let sk = exp(max(double(sink) - mx, -80.0lf))
    var sum = sk
    for (e, v in terms, x) {
        e = exp(max(double(v) - mx, -80.0lf))
        sum += e
    }
    var out : array<float>
    out |> resize(length(x))
    for (o, e in out, terms) {
        o = float(e / sum)
    }
    return out => float(sk / sum)
}

def private sweep_softmax_sink(sink : float; seed : uint) : Sweep {
    var r = Sweep()
    for (n in ACT_SWEEP) {
        var inscope x0 <- lcg_pm(n, opaque(seed) + uint(n), 0.25, 6.0)
        let want <- softmax_sink_ref(x0, sink)
        var inscope x := x0
        softmax_sink(x, int64(n), sink)
        act_check(r, n, softmax_bar(n), x, want._0)
        var total = 0.0lf
        for (v in x) {
            total += double(v)
        }
        if (abs(total + double(want._1) - 1.0lf) > double(softmax_bar(n))) {
            r.bad++
        }
    }
    return r
}

def private sweep_softmax_sink_shift() : Sweep {
    var r = Sweep()
    for (n in ACT_SWEEP) {
        var inscope x0 <- lcg_pm(n, opaque(0x71a5u) + uint(n), 0.25, 6.0)
        let want <- softmax_sink_ref(x0, 1.5)
        var inscope plain := x0
        softmax_sink(plain, int64(n), 1.5)
        act_check(r, n, softmax_bar(n), plain, want._0)
        var inscope shifted : array<float>
        shifted |> resize(n)
        for (s, v in shifted, x0) {
            s = v + 5.0
        }
        softmax_sink(shifted, int64(n), 6.5)
        act_check(r, n, softmax_bar(n), shifted, plain)
    }
    return r
}

def private softcap_ref(x : array<float>; cap : float) : array<float> {
    var out : array<float>
    out |> resize(length(x))
    for (o, v in out, x) {
        o = float(double(cap) * tanh(double(v) / double(cap)))
    }
    return <- out
}

def private softcap_bar(cap : float) : float => 1.0e-6 + 1.0e-6 * cap

def private sweep_softcap(cap : float; vec : bool) : Sweep {
    var r = Sweep()
    for (n in ACT_SWEEP) {
        var inscope x0 <- lcg_pm(n, opaque(vec ? 0x2c81u : 0x9b44u) + uint(n), 0.5, 200.0)
        var inscope want <- softcap_ref(x0, cap)
        var inscope x := x0
        if (vec) {
            softcap4(x, int64(n), cap)
        } else {
            softcap(x, int64(n), cap)
        }
        act_check(r, n, softcap_bar(cap), x, want)
        sweep_inert(r, x, x0)
        for (v in x) {
            if (abs(v) > cap) {
                r.bad++
            }
        }
    }
    return r
}

def private sigmoid_gate_ref(x, g : array<float>) : array<float> {
    var out : array<float>
    out |> resize(length(x))
    for (o, xv, gv in out, x, g) {
        o = float(double(xv) / (1.0lf + exp(-double(gv))))
    }
    return <- out
}

def private sweep_sigmoid_gate(vec : bool) : Sweep {
    var r = Sweep()
    for (n in ACT_SWEEP) {
        var inscope x0 <- lcg_pm(n, opaque(0x5ab3u) + uint(n), 0.25, XMAG)
        var inscope g <- lcg_pm(n, opaque(0x9d10u) + uint(n), 0.25, ZMAG)
        var inscope want <- sigmoid_gate_ref(x0, g)
        var inscope x := x0
        if (vec) {
            sigmoid_gate4(unsafe(addr(x[0])), unsafe(addr(g[0])), int64(n))
        } else {
            sigmoid_gate(unsafe(addr(x[0])), unsafe(addr(g[0])), int64(n))
        }
        act_check(r, n, 1.0e-6 + 1.0e-6 * XMAG, x, want)
        sweep_inert(r, x, x0)
    }
    return r
}

def private swiglu_oai_ref(g, u : array<float>) : array<float> {
    var out : array<float>
    out |> resize(length(g))
    for (o, gv, uv in out, g, u) {
        let xv = min(double(gv), 7.0lf)
        let yv = clamp(double(uv), -7.0lf, 7.0lf)
        o = float((xv / (1.0lf + exp(-1.702lf * xv))) * (yv + 1.0lf))
    }
    return <- out
}

def private sweep_swiglu_oai(vec : bool) : Sweep {
    var r = Sweep()
    for (n in ACT_SWEEP) {
        var inscope g0 <- lcg_pm(n, opaque(0x4411u) + uint(n), 0.25, 12.0)
        var inscope u <- lcg_pm(n, opaque(0x77c2u) + uint(n), 0.25, 12.0)
        var inscope want <- swiglu_oai_ref(g0, u)
        var inscope g := g0
        if (vec) {
            swiglu_oai4(unsafe(addr(g[0])), unsafe(addr(u[0])), int64(n))
        } else {
            swiglu_oai(unsafe(addr(g[0])), unsafe(addr(u[0])), int64(n))
        }
        act_check(r, n, 1.0e-6 + 1.0e-6 * OAI_MAG, g, want)
    }
    return r
}

struct ClampCase {
    clamp_diffs : int
    control_same : int
}

def private oai_pair_diffs(ga, gb, ua, ub : array<float>; vec : bool) : int {
    var a := ga
    var b := gb
    let n = long_length(ga)
    if (vec) {
        swiglu_oai4(unsafe(addr(a[0])), unsafe(addr(ua[0])), n)
        swiglu_oai4(unsafe(addr(b[0])), unsafe(addr(ub[0])), n)
    } else {
        swiglu_oai(unsafe(addr(a[0])), unsafe(addr(ua[0])), n)
        swiglu_oai(unsafe(addr(b[0])), unsafe(addr(ub[0])), n)
    }
    var d = 0
    for (x, y in a, b) {
        if (x != y) {
            d++
        }
    }
    return d
}

def private oai_clamp_case(vec : bool) : ClampCase {
    let n = 18
    let u <- lcg_pm(n, opaque(0x3f00u), 1.5, 5.0)
    var res = ClampCase()
    res.clamp_diffs += oai_pair_diffs(filled(n, 20.0), filled(n, 7.0), u, u, vec)
    res.clamp_diffs += oai_pair_diffs(filled(n, 2.0), filled(n, 2.0), filled(n, 20.0), filled(n, 7.0), vec)
    res.clamp_diffs += oai_pair_diffs(filled(n, 2.0), filled(n, 2.0), filled(n, -20.0), filled(n, -7.0), vec)
    res.control_same = n - oai_pair_diffs(filled(n, 3.0), filled(n, 7.0), u, u, vec)
    return res
}

def private silu_mul_ref(a, z : array<float>) : array<float> {
    var out : array<float>
    out |> resize(length(a))
    for (o, av, zv in out, a, z) {
        let d = double(zv)
        o = float(double(av) * (d / (1.0lf + exp(-d))))
    }
    return <- out
}

def private sweep_silu_mul4() : Sweep {
    var r = Sweep()
    for (n in ACT_SWEEP) {
        var inscope a <- lcg_pm(n, opaque(0x6e21u) + uint(n), 0.25, XMAG)
        var inscope z <- lcg_pm(n, opaque(0xf13cu) + uint(n), 0.25, ZMAG)
        var inscope want <- silu_mul_ref(a, z)
        var inscope before <- filled(n, SENTINEL)
        var inscope o := before
        silu_mul4(unsafe(addr(o[0])), unsafe(addr(a[0])), unsafe(addr(z[0])), int64(n))
        act_check(r, n, 1.0e-6 + 1.0e-6 * XMAG * ZMAG, o, want)
        sweep_inert(r, o, before)
    }
    return r
}

def private silu_ref(x : array<float>) : array<float> {
    var out : array<float>
    out |> resize(length(x))
    for (o, v in out, x) {
        let d = double(v)
        o = float(d / (1.0lf + exp(-d)))
    }
    return <- out
}

struct BatchCase {
    split_diffs : int
    dispatches : int64
    elems : int
}

def private silu4_batch_case(var r : Sweep; d, npos : int) : BatchCase {
    var res = BatchCase(elems = d * npos)
    let x0 <- lcg_pm(d * npos, opaque(0x84c1u) + uint(d * 131 + npos), 0.25, ZMAG)
    var whole := x0
    silu4(whole, int64(d) * int64(npos))
    var batched := x0
    let before = get_disp_count()
    silu4_batch(batched, int64(d), int64(npos))
    res.dispatches = get_disp_count() - before
    let want <- silu_ref(x0)
    act_check(r, d * npos, 1.0e-6 + 1.0e-6 * ZMAG, whole, want)
    sweep_inert(r, batched, x0)
    for (bv, wv in batched, whole) {
        if (bv != wv) {
            res.split_diffs++
        }
    }
    return res
}

def private l2_ref(v : array<float>; rows, row_size : int; eps : float) : array<float> {
    var out : array<float>
    out |> resize(rows * row_size)
    for (r in range(rows)) {
        let base = r * row_size
        var sum = 0.0lf
        for (i in range(row_size)) {
            let x = v[base + i]
            sum += double(x * x)
        }
        let scale = 1.0 / max(sqrt(float(sum)), eps)
        for (i in range(row_size)) {
            out[base + i] = v[base + i] * scale
        }
    }
    return <- out
}

def private l2_bar(row_size : int) : float => 1.0e-6 + 2.0e-7 * float(row_size)

def private sweep_l2_norm_rows() : Sweep {
    var r = Sweep()
    for (s in range(length(L2_SHAPES) / 2)) {
        let rows = L2_SHAPES[s * 2]
        let row_size = L2_SHAPES[s * 2 + 1]
        var inscope v0 <- lcg_pm(rows * row_size, opaque(0xa1b2u) + uint(s), 1.25, 3.0)
        var inscope want <- l2_ref(v0, rows, row_size, L2_EPS)
        var inscope v := v0
        l2_norm_rows(unsafe(addr(v[0])), int64(rows), int64(row_size), L2_EPS)
        act_check(r, row_size, l2_bar(row_size), v, want)
        sweep_inert(r, v, v0)
        for (rw in range(rows)) {
            var sum = 0.0lf
            for (i in range(row_size)) {
                sum += double(v[rw * row_size + i]) * double(v[rw * row_size + i])
            }
            if (abs(sqrt(sum) - 1.0lf) > double(l2_bar(row_size))) {
                r.bad++
            }
        }
    }
    return r
}

def private l2_eps_floor() : Sweep {
    var r = Sweep()
    let rows = 3
    let row_size = 8
    var v0 <- filled(rows * row_size, 0.0)
    for (i in range(row_size)) {
        v0[row_size + i] = 1.0e-9
    }
    let want <- l2_ref(v0, rows, row_size, L2_EPS)
    var v := v0
    l2_norm_rows(unsafe(addr(v[0])), int64(rows), int64(row_size), L2_EPS)
    act_check(r, row_size, l2_bar(row_size), v, want)
    return r
}

struct GemmCase {
    bad : int
    jo_diffs : int
    cases : int
    elems : int
    poison_inside : int
    worst : float
    worst_bar : float
    worst_k : int
}

def private gemm_ref(c0, a, b : array<float>; M, K, N : int) : array<float> {
    var out : array<float>
    out |> resize(M * N)
    for (i in range(M)) {
        for (j in range(N)) {
            var acc = double(c0[i * N + j])
            for (k in range(K)) {
                acc += double(a[i * K + k]) * double(b[k * N + j])
            }
            out[i * N + j] = float(acc)
        }
    }
    return <- out
}

def private gemm_bar(K : int) : float => 6.0e-8 * float(K) * (float(K) + 2.0) + 1.0e-6

def private sweep_gemm() : GemmCase {
    var r = GemmCase()
    for (s in range(length(GEMM_SHAPES) / 3)) {
        let M = GEMM_SHAPES[s * 3]
        let K = GEMM_SHAPES[s * 3 + 1]
        let N = GEMM_SHAPES[s * 3 + 2]
        var inscope a <- lcg_pm(M * K, opaque(0x1717u) + uint(s), 0.25, 1.0)
        var inscope b <- lcg_pm(K * N, opaque(0x2828u) + uint(s), 0.25, 1.0)
        var inscope c0 <- lcg_pm(M * N, opaque(0x3939u) + uint(s), 0.25, 1.0)
        var inscope want <- gemm_ref(c0, a, b, M, K, N)
        var inscope c1 := c0
        gemm_f32(unsafe(addr(c1[0])), unsafe(addr(a[0])), unsafe(addr(b[0])), int64(M), int64(K), int64(N))
        var inscope c2 := c0
        gemm_f32_jo(unsafe(addr(c2[0])), unsafe(addr(a[0])), unsafe(addr(b[0])), int64(M), int64(K), int64(N))
        r.cases++
        tally_gemm(r, K, c1, c2, want)
    }
    return r
}

def private tally_gemm(var r : GemmCase; K : int; c1, c2, want : array<float>) {
    let bar = gemm_bar(K)
    for (g, j, w in c1, c2, want) {
        r.elems++
        let diff = abs(g - w)
        if (diff > bar || abs(j - w) > bar) {
            r.bad++
        }
        if (g != j) {
            r.jo_diffs++
        }
        if (abs(g - (w + ACT_POISON)) <= bar) {
            r.poison_inside++
        }
        if (diff >= r.worst) {
            r.worst = diff
            r.worst_bar = bar
            r.worst_k = K
        }
    }
}

def private zero_size_moves() : int {
    let before <- lcg_pm(8, opaque(0x0f01u), 0.25, 2.0)
    let g <- lcg_pm(8, opaque(0x0f02u), 0.25, 2.0)
    var x := before
    softmax_sink(x, 0l, 1.0)
    softcap(x, 0l, 30.0)
    softcap4(x, 0l, 30.0)
    silu4_batch(x, 8l, 0l)
    sigmoid_gate(unsafe(addr(x[0])), unsafe(addr(g[0])), 0l)
    sigmoid_gate4(unsafe(addr(x[0])), unsafe(addr(g[0])), 0l)
    swiglu_oai(unsafe(addr(x[0])), unsafe(addr(g[0])), 0l)
    swiglu_oai4(unsafe(addr(x[0])), unsafe(addr(g[0])), 0l)
    silu_mul4(unsafe(addr(x[0])), unsafe(addr(g[0])), unsafe(addr(g[0])), 0l)
    l2_norm_rows(unsafe(addr(x[0])), 0l, 8l, L2_EPS)
    var moved = 0
    for (v, b in x, before) {
        if (v != b) {
            moved++
        }
    }
    return moved
}

[test(description = "Over lengths 1 to 1025, `softmax_sink` matches an fp64 reference with negligible and dominant sinks, the row plus sink mass sums to one, and shifting scores and sink together leaves the row unchanged")]
def test_softmax_sink(t : T?) {
    t |> run("softmax_sink against an fp64 reference, the row plus the sink mass summing to one") @(t : T?) {
        report_sweep(t, "softmax_sink negligible sink", sweep_softmax_sink(-1000.0, 0x1001u))
        report_sweep(t, "softmax_sink dominant sink", sweep_softmax_sink(8.0, 0x1002u))
    }
    t |> run("softmax_sink is invariant to a shift of the scores and the sink together") @(t : T?) {
        report_sweep(t, "softmax_sink shift", sweep_softmax_sink_shift())
    }
}

[test(description = "`softcap` and `softcap4` equal cap x tanh(x / cap) within a cap-scaled bar at caps 30 and 50 over lengths 1 to 1025, move every element, and never leave the band")]
def test_softcap(t : T?) {
    t |> run("softcap is cap*tanh(x/cap) and never leaves the band") @(t : T?) {
        report_moved(t, "softcap cap=30", sweep_softcap(30.0, false))
        report_moved(t, "softcap cap=50", sweep_softcap(50.0, false))
    }
    t |> run("softcap4 holds the same closed form through its tail") @(t : T?) {
        report_moved(t, "softcap4 cap=30", sweep_softcap(30.0, true))
        report_moved(t, "softcap4 cap=50", sweep_softcap(50.0, true))
    }
}

[test(description = "`sigmoid_gate` and `sigmoid_gate4` scale each element by the logistic of its gate, matching an fp64 reference over lengths 1 to 1025 including every vector tail")]
def test_sigmoid_gate(t : T?) {
    t |> run("sigmoid_gate scales by the logistic of the gate row") @(t : T?) {
        report_moved(t, "sigmoid_gate", sweep_sigmoid_gate(false))
    }
    t |> run("sigmoid_gate4 holds the same form through its tail") @(t : T?) {
        report_moved(t, "sigmoid_gate4", sweep_sigmoid_gate(true))
    }
}

[test(description = "`swiglu_oai` and `swiglu_oai4` match an fp64 reference carrying the gate limit 7 and up band +-7, and a gate or up value past the limit reads exactly as the limit while gates inside it stay distinct")]
def test_swiglu_oai(t : T?) {
    t |> run("swiglu_oai against an fp64 reference carrying the trained clamps") @(t : T?) {
        report_moved(t, "swiglu_oai", sweep_swiglu_oai(false))
        report_sweep(t, "swiglu_oai4", sweep_swiglu_oai(true))
    }
    t |> run("the gate limit and the up-branch band are the activation, not an optimization") @(t : T?) {
        let scalar = oai_clamp_case(false)
        t |> equal(scalar.clamp_diffs, 0, "swiglu_oai: a value past the limit must read as the limit")
        t |> equal(scalar.control_same, 0, "swiglu_oai: gates inside the limit must not collapse together")
        let vec = oai_clamp_case(true)
        t |> equal(vec.clamp_diffs, 0, "swiglu_oai4: a value past the limit must read as the limit")
        t |> equal(vec.control_same, 0, "swiglu_oai4: gates inside the limit must not collapse together")
    }
}

[test(description = "`silu_mul4` overwrites every element of a sentinel destination with a x silu(z), matching an fp64 reference over lengths 1 to 1025")]
def test_silu_mul4(t : T?) {
    t |> run("silu_mul4 writes a*silu(z) over a sentinel destination") @(t : T?) {
        report_moved(t, "silu_mul4", sweep_silu_mul4())
    }
}

[test(description = "`silu4_batch` lands the same bits as `silu4` over the whole buffer, splitting rows across lanes only with a job queue, and runs whole where an odd row length or a single row forbids the split")]
def test_silu4_batch(t : T?) {
    t |> run("silu4_batch splits rows without moving a bit, threaded and inline") @(t : T?) {
        var r = Sweep()
        let inline_wide = silu4_batch_case(r, 256, 512)
        var threaded = BatchCase()
        with_dasllama_jobque_() {
            threaded = silu4_batch_case(r, 256, 512)
        }
        report_moved(t, "silu4_batch", r)
        t |> equal(inline_wide.split_diffs, 0, "silu4_batch inline: elements differing from silu4 over the whole buffer")
        t |> equal(threaded.split_diffs, 0, "silu4_batch threaded: elements differing from silu4 over the whole buffer")
        t |> success(!is_job_que_available(), "the inline leg needs a que-less process to be one lane")
        t |> equal(inline_wide.dispatches, 0l, "a que-less process must run silu4_batch inline")
        t |> success(threaded.dispatches > 0l, "the threaded leg must dispatch, else both legs are the same one lane")
    }
    t |> run("silu4_batch falls back whole where the row length or the row count forbids a split") @(t : T?) {
        var r = Sweep()
        with_dasllama_jobque_() {
            let odd = silu4_batch_case(r, 250, 64)
            let single = silu4_batch_case(r, 64, 1)
            let tail = silu4_batch_case(r, 4, 1025)
            t |> equal(odd.split_diffs, 0, "a row length off the float4 grouping must run silu4 whole")
            t |> equal(single.split_diffs, 0, "a single row must run silu4 whole")
            t |> equal(tail.split_diffs, 0, "a narrow many-row image must still land the same bits")
            t |> equal(odd.dispatches, 0l, "a row length off the float4 grouping must not dispatch")
            t |> equal(single.dispatches, 0l, "a single row must not dispatch")
            t |> success(tail.dispatches > 0l, "a narrow many-row image must still split across lanes")
        }
        report_moved(t, "silu4_batch fallbacks", r)
    }
}

[test(description = "`l2_norm_rows` matches an fp64 reference and drives every row to unit length across row shapes, and an all-zero or tiny row divides by the eps floor instead")]
def test_l2_norm_rows(t : T?) {
    t |> run("l2_norm_rows drives every row to unit length") @(t : T?) {
        report_moved(t, "l2_norm_rows", sweep_l2_norm_rows())
    }
    t |> run("l2_norm_rows floors the divisor at eps") @(t : T?) {
        report_sweep(t, "l2_norm_rows eps floor", l2_eps_floor())
    }
}

[test(description = "`gemm_f32` matches an fp64 C + A x B within a K-scaled bar on every declared shape, a 0.01 poison leg falls outside that bar, and `gemm_f32_jo` is bit-identical to it")]
def test_gemm_f32(t : T?) {
    t |> run("gemm_f32 and gemm_f32_jo accumulate the same fp64 product over tiles and both strips") @(t : T?) {
        let r = sweep_gemm()
        t |> equal(r.cases, length(GEMM_SHAPES) / 3, "every declared shape must run")
        t |> success(r.elems > 0, "the sweep must compare at least one element")
        t |> equal(r.bad, 0, "gemm_f32: elements past the bar, worst |diff| {r.worst} against bar {r.worst_bar} at K={r.worst_k}")
        t |> equal(r.jo_diffs, 0, "gemm_f32_jo must be bit-identical to gemm_f32")
        t |> equal(r.poison_inside, 0, "gemm_f32: poison legs inside the bar - a bar nothing exceeds discriminates nothing")
        feint("gemm_f32: {r.elems} elements, worst |diff| {r.worst} against bar {r.worst_bar} at K={r.worst_k}\n")
    }
}

[test(description = "A zero element or row count leaves the buffer untouched in every activation and norm kernel: softmax_sink, softcap, silu, sigmoid gate, swiglu_oai, silu_mul4 and l2_norm_rows")]
def test_zero_size(t : T?) {
    t |> run("a zero count is a no-op on every activation and norm kernel") @(t : T?) {
        t |> equal(zero_size_moves(), 0, "a zero count must leave the buffer untouched")
    }
}

[test(description = "`rmsnorm` maps a constant vector to ones, gives unit mean square under unit weights, and passes the weight through for an rms-1 input, each within a small tolerance")]
def test_rmsnorm(t : T?) {
    // A constant vector normalizes to ~1, whatever its magnitude — RMSNorm removes scale.
    t |> run("constant vector -> ones") @(t : T?) {
        var o <- [0.0, 0.0, 0.0]
        let x <- [7.0, 7.0, 7.0]
        let w <- [1.0, 1.0, 1.0]
        rmsnorm(o, x, w, 3l)
        for (j in range(3)) {
            t |> success(abs(o[j] - 1.0) < 1e-4)
        }
    }
    // Defining property: with unit weights, mean(o^2) == 1, so sum(o^2) == size.
    t |> run("unit mean-square") @(t : T?) {
        var o <- [0.0, 0.0, 0.0, 0.0]
        let x <- [1.0, 2.0, 3.0, 4.0]
        let w <- [1.0, 1.0, 1.0, 1.0]
        rmsnorm(o, x, w, 4l)
        var ss = 0.0
        for (j in range(4)) {
            ss += o[j] * o[j]
        }
        t |> success(abs(ss - 4.0) < 1e-3)
    }
    // A normalized input (rms ~ 1) passes the learned weight straight through.
    t |> run("weight passthrough") @(t : T?) {
        var o <- [0.0, 0.0, 0.0, 0.0]
        let x <- [1.0, 1.0, 1.0, 1.0]
        let w <- [2.0, 3.0, 4.0, 5.0]
        rmsnorm(o, x, w, 4l)
        for (j in range(4)) {
            t |> success(abs(o[j] - w[j]) < 1e-3)
        }
    }
}

[test(description = "`silu` matches hand-computed values to 1e-4 including the negative dip, saturates to x and 0 at plus and minus 20, and `silu` / `gelu` with size zero leave the array untouched without an index fault")]
def test_silu(t : T?) {
    // Exact + hand-computed shape: silu(0)=0, silu(1)=0.7310586, silu(2)=1.7615942,
    // silu(-1)=-0.2689414 (negative — SiLU is non-monotonic, dips below zero).
    t |> run("known values") @(t : T?) {
        var x <- [0.0, 1.0, 2.0, -1.0]
        silu(x, 4l)
        t |> success(abs(x[0]) < 1e-6)              // 0 * sigmoid(0) = 0 exactly
        t |> success(abs(x[1] - 0.7310586) < 1e-4)
        t |> success(abs(x[2] - 1.7615942) < 1e-4)
        t |> success(abs(x[3] + 0.2689414) < 1e-4)
    }
    // Saturation: sigmoid -> 1 so silu(x) -> x for large +x; sigmoid -> 0 so silu(x) -> 0 for large -x.
    t |> run("saturation") @(t : T?) {
        var x <- [20.0, -20.0]
        silu(x, 2l)
        t |> success(abs(x[0] - 20.0) < 1e-4)
        t |> success(abs(x[1]) < 1e-4)
    }
    // Regression: the array overload forwards via addr(x[0]), which index-faults on an empty
    // array. size <= 0 must be a safe no-op (both silu and gelu).
    t |> run("empty / zero size is a no-op") @(t : T?) {
        var e : array<float>
        silu(e, 0l)
        gelu(e, 0l)
        var x <- [1.0, 2.0, 3.0]
        silu(x, 0l)
        gelu(x, 0l)
        t |> success(abs(x[0] - 1.0) < 1e-6)   // untouched by the zero-count calls
        t |> success(true)                      // reached here ⇒ no index-fault
    }
}

// Realistic gate/up pre-activations for the fused-kernel tests (n=12, mix of signs/magnitudes).
def private gate_inputs() : tuple<array<float>; array<float>> {
    var g <- [0.0, 1.0, 2.0, -1.0, 3.5, -4.0, 0.25, -0.1, 5.0, -2.5, 0.75, -0.5]
    var u <- [1.0, 2.0, -1.0, 0.5, 2.0, -0.5, 3.0, 1.0, -1.5, 0.25, -2.0, 4.0]
    return g => u
}

[test(description = "Scalar `swiglu` / `geglu` equal the activation-then-multiply reference bit for bit on 12 mixed-sign inputs, and the exp4 fused `swiglu4` / `geglu4` stay within 1e-3 max abs diff of them")]
def test_gate(t : T?) {
    let n = 12l
    // The scalar fused gate is the bit-exact reference: swiglu(g,u) == silu(g) then *u, same order.
    t |> run("swiglu scalar == silu*u (bit-exact)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        silu(ref, n)
        for (i in range(int(n))) { ref[i] *= inp._1[i] }
        var got := inp._0
        swiglu(got, inp._1, n)
        t |> success(logits_maxdiff(ref, got) == 0.0, "swiglu bit-exact vs silu then *u")
    }
    t |> run("geglu scalar == gelu*u (bit-exact)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        gelu(ref, n)
        for (i in range(int(n))) { ref[i] *= inp._1[i] }
        var got := inp._0
        geglu(got, inp._1, n)
        t |> success(logits_maxdiff(ref, got) == 0.0, "geglu bit-exact vs gelu then *u")
    }
    // The exp4 fused gate (production default) is ~2 ulp — within tolerance of the scalar reference.
    t |> run("swiglu4 ~= swiglu (exp4 tolerance)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        swiglu(ref, inp._1, n)
        var got := inp._0
        swiglu4(unsafe(addr(got[0])), unsafe(addr(inp._1[0])), n)
        t |> success(logits_maxdiff(ref, got) < 1e-3, "swiglu4 within tolerance of scalar swiglu")
    }
    t |> run("geglu4 ~= geglu (exp4 tolerance)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        geglu(ref, inp._1, n)
        var got := inp._0
        geglu4(unsafe(addr(got[0])), unsafe(addr(inp._1[0])), n)
        t |> success(logits_maxdiff(ref, got) < 1e-3, "geglu4 within tolerance of scalar geglu")
    }
}

[test(description = "`softmax` yields an order-preserving distribution summing to 1, uniform output on equal scores, equal results for inputs shifted by 1000 (max-subtraction), and a no-op on size zero")]
def test_softmax(t : T?) {
    // Output is a probability distribution: positive, order-preserving, sums to 1.
    t |> run("distribution sums to 1") @(t : T?) {
        var x <- [1.0, 2.0, 3.0]
        softmax(x, 3l)
        var sum = 0.0
        for (i in range(3)) {
            sum += x[i]
        }
        t |> success(abs(sum - 1.0) < 1e-5)
        t |> success(x[0] < x[1] && x[1] < x[2])  // larger score -> larger probability
        t |> success(x[0] > 0.0 && x[2] < 1.0)
    }
    // Equal scores -> uniform distribution.
    t |> run("uniform input -> uniform output") @(t : T?) {
        var x <- [5.0, 5.0, 5.0, 5.0]
        softmax(x, 4l)
        for (i in range(4)) {
            t |> success(abs(x[i] - 0.25) < 1e-6)
        }
    }
    // Max-subtraction is overflow-safe and shift-invariant: exp(1000) would be +inf,
    // but softmax([1000,1001,1002]) must equal softmax([0,1,2]).
    t |> run("numerical stability") @(t : T?) {
        var big <- [1000.0, 1001.0, 1002.0]
        var small <- [0.0, 1.0, 2.0]
        softmax(big, 3l)
        softmax(small, 3l)
        var sum = 0.0
        for (i in range(3)) {
            sum += big[i]
            t |> success(abs(big[i] - small[i]) < 1e-6)
        }
        t |> success(abs(sum - 1.0) < 1e-5)  // would be NaN without the max-subtract
    }
    // Regression: the array overload forwards via addr(x[0]), which index-faults on an empty
    // array; size <= 0 must be a safe no-op.
    t |> run("empty / zero size is a no-op") @(t : T?) {
        var e : array<float>
        softmax(e, 0l)
        var x <- [1.0, 2.0, 3.0]
        softmax(x, 0l)
        t |> success(abs(x[0] - 1.0) < 1e-6)   // untouched
        t |> success(true)                      // reached here ⇒ no index-fault
    }
}

// The greedy pick's two halves: parallel_argmax locates the FIRST maximum, hlse sums exp(x - mx)
// the same way. Both take the empty row as a no-op, which is what the vectorized forms'
// addr(x[0]) would fault on. `argmax_of` is _compares' whole-row parallel_argmax.
[test(description = "`parallel_argmax` returns the first maximum on empty, single, ordinary and tied rows, and `hlse` matches the scalar exp(x - mx) sum to 1e-6 across a 4-wide group plus tail, zero on empty")]
def test_argmax_hlse(t : T?) {
    t |> run("argmax on empty / single / ordinary rows") @(t : T?) {
        var e : array<float>
        t |> equal(argmax_of(e), 0l, "empty row answers 0, no index fault")
        var one <- [ -7.5 ]
        t |> equal(argmax_of(one), 0l, "the single element is the max")
        var many <- [ 0.1, -2.0, 0.3, 0.25, -1.0, 0.9, 0.2, 0.15, -0.5, 0.05, 4.25, 0.0, 1.5 ]
        t |> equal(argmax_of(many), 10l, "the max at index 10")
        var first <- [ 3.0, 0.0, 1.0, 2.0, 0.5 ]
        t |> equal(argmax_of(first), 0l, "a max at index 0")
    }
    t |> run("argmax on ties takes the FIRST") @(t : T?) {
        var tied <- [ 1.0, 5.0, 2.0, 5.0, 5.0, 0.0 ]
        t |> equal(argmax_of(tied), 1l, "three equal maxima, the lowest index wins")
        var tied_group <- [ 5.0, 5.0, 5.0, 5.0 ]
        t |> equal(argmax_of(tied_group), 0l, "a whole row at the max still answers index 0")
    }
    t |> run("hlse sums exp(x - mx)") @(t : T?) {
        var e : array<float>
        t |> success(hlse(e, 0.0) == 0.0lf, "empty row sums to 0, no index fault")
        var one <- [ 2.5 ]
        t |> success(abs(hlse(one, 2.5) - 1.0lf) < 1e-6lf, "x == mx contributes exactly 1")
        // 6 entries: one full 4-wide group plus a 2-element tail, so both loops run
        var x <- [ 0.0, 1.0, 2.0, 3.0, -1.0, 0.5 ]
        var want = 0.0lf
        for (v in x) {
            want += double(exp(v - 3.0))
        }
        t |> success(abs(hlse(x, 3.0) - want) < 1e-6lf, "group + tail sum matches the scalar walk")
        var flat <- [ 1.0, 1.0, 1.0, 1.0, 1.0 ]
        t |> success(abs(hlse(flat, 1.0) - 5.0lf) < 1e-6lf, "five tied entries at the max sum to 5")
    }
}
