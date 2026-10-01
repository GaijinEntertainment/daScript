options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_convert
require math
require _compares   // Sweep: the length sweeps' tally and verdict; opaque: the seeds' taint seam; bf16_value / bf16_bits_truncated

//! The [tuned] elementwise kernels - dot, dot_bf16, axpy, add_inplace, add_scale_inplace,
//! mul_inplace, scale_inplace, copy_floats - swept over tail lengths against fp64 references.

let SWEEP <- [ 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 23, 24, 25, 31, 32, 33,
               63, 64, 65, 100, 127, 128, 129, 1023, 1024, 1025 ]

let TEST_SCALE = -0.75
let SENTINEL = 1.0e9
let DOT_POISON = 0.5
let ROUND_U = 1.0e-7
let BAR_FLOOR = 1.0e-6

def private dot_bar(n : int) : float {
    return ROUND_U * float(n) * float(n) + BAR_FLOOR
}

def private lcg_grid(n : int; seed : uint; step : float; signed_values : bool) : array<float> {
    var st = opaque(seed)
    var a : array<float>
    a |> resize(n)
    for (v in a) {
        st = st * 1664525u + 1013904223u
        let k = int((st >> 9u) & 31u) + 1
        let neg = signed_values && ((st >> 20u) & 1u) == 0u
        v = float(neg ? -k : k) * step
    }
    return <- a
}

def private lcg_unit(n : int; seed : uint) : array<float> {
    var st = opaque(seed)
    var a : array<float>
    a |> resize(n)
    for (v in a) {
        st = st * 1664525u + 1013904223u
        v = float(st >> 8u) * (2.0 / 16777216.0) - 1.0
    }
    return <- a
}

def private to_bf16(src : array<float>) : array<uint16> {
    return <- [for (v in src); bf16_bits_truncated(v)]
}

//! one swept length of an in-place kernel: bit-exact, and the elements left where they were counted
def private tally(var r : Sweep; n : int; got, want, before : array<float>) {
    sweep_check(r, n, 0.0, got, want)
    sweep_inert(r, got, before)
}

//! the verdict on a sweep over every length of `SWEEP`
def private report(t : T?; name : string; r : Sweep) => report_sweep(t, name, r, length(SWEEP))

//! `report`, and every element of the in-place run moved
def private report_inplace(t : T?; name : string; r : Sweep) => report_sweep(t, name, r, length(SWEEP), true)

def private sweep_dot_exact() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let a <- lcg_grid(n, opaque(0x11a3u) + uint(n), 0.125, true)
        let b <- lcg_grid(n, opaque(0x9e37u) + uint(n), 0.375, true)
        var want = 0.0lf
        for (av, bv in a, b) {
            want += double(av) * double(bv)
        }
        let got = dot(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        sweep_check(r, n, 0.0, [got], [float(want)])
    }
    return r
}

def private sweep_dot_bar() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let a <- lcg_unit(n, opaque(0x4d2bu) + uint(n))
        let b <- lcg_unit(n, opaque(0xa51fu) + uint(n))
        var want = 0.0lf
        for (av, bv in a, b) {
            want += double(av) * double(bv)
        }
        let got = dot(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        sweep_check(r, n, dot_bar(n), [got], [float(want)], DOT_POISON)
    }
    return r
}

def private sweep_dot_bf16_exact() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let a <- lcg_grid(n, opaque(0x3c07u) + uint(n), 0.125, true)
        let bf <- lcg_grid(n, opaque(0x7b91u) + uint(n), 0.375, true)
        let b <- to_bf16(bf)
        var want = 0.0lf
        for (av, bw in a, b) {
            want += double(av) * double(bf16_value(bw))
        }
        let got = dot_bf16(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        sweep_check(r, n, 0.0, [got], [float(want)])
    }
    return r
}

def private sweep_dot_bf16_bar() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let a <- lcg_unit(n, opaque(0x2f6eu) + uint(n))
        let bf <- lcg_unit(n, opaque(0xc0deu) + uint(n))
        let b <- to_bf16(bf)
        var want = 0.0lf
        for (av, bw in a, b) {
            want += double(av) * double(bf16_value(bw))
        }
        let got = dot_bf16(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        sweep_check(r, n, dot_bar(n), [got], [float(want)], DOT_POISON)
    }
    return r
}

def private sweep_copy_floats() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let s <- lcg_unit(n, opaque(0x51c0u) + uint(n))
        var before : array<float>
        before |> resize(n)
        for (v in before) {
            v = SENTINEL
        }
        var d := before
        copy_floats(unsafe(addr(d[0])), unsafe(addr(s[0])), int64(n))
        tally(r, n, d, s, before)
    }
    return r
}

def private sweep_add_inplace() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque(0x6a12u) + uint(n), 0.125, true)
        let s <- lcg_grid(n, opaque(0xb33fu) + uint(n), 0.375, true)
        var want : array<float>
        want |> resize(n)
        for (w, b, sv in want, before, s) {
            w = float(double(b) + double(sv))
        }
        var d := before
        add_inplace(unsafe(addr(d[0])), unsafe(addr(s[0])), int64(n))
        tally(r, n, d, want, before)
    }
    return r
}

def private sweep_add_scale_inplace() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque(0x1de5u) + uint(n), 0.125, false)
        let s <- lcg_grid(n, opaque(0x8c44u) + uint(n), 0.375, false)
        var want : array<float>
        want |> resize(n)
        for (w, b, sv in want, before, s) {
            w = float((double(b) + double(sv)) * double(TEST_SCALE))
        }
        var d := before
        add_scale_inplace(unsafe(addr(d[0])), unsafe(addr(s[0])), TEST_SCALE, int64(n))
        tally(r, n, d, want, before)
    }
    return r
}

def private sweep_mul_inplace() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque(0x9077u) + uint(n), 0.125, true)
        let s <- lcg_grid(n, opaque(0x2bb8u) + uint(n), 0.375, true)
        var want : array<float>
        want |> resize(n)
        for (w, b, sv in want, before, s) {
            w = float(double(b) * double(sv))
        }
        var d := before
        mul_inplace(unsafe(addr(d[0])), unsafe(addr(s[0])), int64(n))
        tally(r, n, d, want, before)
    }
    return r
}

def private sweep_scale_inplace() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque(0x4e19u) + uint(n), 0.125, true)
        var want : array<float>
        want |> resize(n)
        for (w, b in want, before) {
            w = float(double(b) * double(TEST_SCALE))
        }
        var d := before
        scale_inplace(unsafe(addr(d[0])), TEST_SCALE, int64(n))
        tally(r, n, d, want, before)
    }
    return r
}

def private sweep_axpy() : Sweep {
    var r = Sweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque(0x7231u) + uint(n), 0.125, true)
        let s <- lcg_grid(n, opaque(0xd4a0u) + uint(n), 0.375, true)
        var want : array<float>
        want |> resize(n)
        for (w, b, sv in want, before, s) {
            w = float(double(b) + double(TEST_SCALE) * double(sv))
        }
        var d := before
        axpy(unsafe(addr(d[0])), unsafe(addr(s[0])), TEST_SCALE, int64(n))
        tally(r, n, d, want, before)
    }
    return r
}

def private zero_length_moves() : int {
    let before <- lcg_grid(4, opaque(0x0e11u), 0.125, true)
    let s <- lcg_grid(4, opaque(0x0e12u), 0.375, true)
    var d := before
    unsafe {
        copy_floats(addr(d[0]), addr(s[0]), 0l)
        add_inplace(addr(d[0]), addr(s[0]), 0l)
        add_scale_inplace(addr(d[0]), addr(s[0]), TEST_SCALE, 0l)
        mul_inplace(addr(d[0]), addr(s[0]), 0l)
        scale_inplace(addr(d[0]), TEST_SCALE, 0l)
        axpy(addr(d[0]), addr(s[0]), TEST_SCALE, 0l)
    }
    var moved = 0
    for (x, b in d, before) {
        if (x != b) {
            moved++
        }
    }
    return moved
}

def private zero_length_dots() : float {
    let a <- lcg_grid(4, opaque(0x0e13u), 0.125, true)
    let bf <- lcg_grid(4, opaque(0x0e14u), 0.375, true)
    let b <- to_bf16(bf)
    unsafe {
        return abs(dot(addr(a[0]), addr(a[0]), 0l)) + abs(dot_bf16(addr(a[0]), addr(b[0]), 0l))
    }
}

[test]
def test_dot(t : T?) {
    t |> run("dot bit-matches an fp64 accumulation on inputs fp32 cannot round") @(t : T?) {
        report(t, "dot exact", sweep_dot_exact())
    }
    t |> run("dot against an fp64 accumulation on unit-range inputs, with the bar's control") @(t : T?) {
        report(t, "dot bar", sweep_dot_bar())
    }
}

[test]
def test_dot_bf16(t : T?) {
    t |> run("dot_bf16 bit-matches an fp64 accumulation over exactly-bf16 operands") @(t : T?) {
        report(t, "dot_bf16 exact", sweep_dot_bf16_exact())
    }
    t |> run("dot_bf16 against an fp64 accumulation over the widened operands, with the bar's control") @(t : T?) {
        report(t, "dot_bf16 bar", sweep_dot_bf16_bar())
    }
}

[test]
def test_copy_and_add(t : T?) {
    t |> run("copy_floats replaces every sentinel with the source bit for bit") @(t : T?) {
        report_inplace(t, "copy_floats", sweep_copy_floats())
    }
    t |> run("add_inplace is exact on dyadic operands") @(t : T?) {
        report_inplace(t, "add_inplace", sweep_add_inplace())
    }
    t |> run("add_scale_inplace is exact and its sign flip cannot return the input") @(t : T?) {
        report_inplace(t, "add_scale_inplace", sweep_add_scale_inplace())
    }
}

[test]
def test_mul_scale_axpy(t : T?) {
    t |> run("mul_inplace is exact on operands no multiplier of which is one") @(t : T?) {
        report_inplace(t, "mul_inplace", sweep_mul_inplace())
    }
    t |> run("scale_inplace is exact at a dyadic scale") @(t : T?) {
        report_inplace(t, "scale_inplace", sweep_scale_inplace())
    }
    t |> run("axpy is exact on dyadic operands") @(t : T?) {
        report_inplace(t, "axpy", sweep_axpy())
    }
}

[test]
def test_zero_length(t : T?) {
    t |> run("a zero count is a no-op on every elementwise kernel") @(t : T?) {
        t |> equal(zero_length_moves(), 0, "a zero count must leave the destination untouched")
        t |> equal(zero_length_dots(), 0.0, "a zero count must answer an empty sum")
    }
}

def private f16_cast_bits(v : float) : uint16 => unsafe(reinterpret<uint16>(float16(v)))

def private f16_form_mismatches(vals : array<float>) : int {
    var out : array<uint16>
    out |> resize(length(vals))
    cvt_f32_to_f16(unsafe(addr(out[0])), unsafe(addr(vals[0])), long_length(vals))
    var bad = 0
    for (w, v in out, vals) {
        if (w != f16_cast_bits(v)) {
            bad++
        }
    }
    return bad
}

//! the values a rounding difference shows on: the halfway ties, the subnormal band, the largest half, both zeros, a value under the smallest subnormal
def private f16_edge_values : array<float> {
    var edge : array<float>
    edge |> reserve(64 * 4 + 5)
    for (k in range(64)) {
        let tie = 1.0 + float(k) / 2048.0 + 1.0 / 4096.0
        edge |> push(tie)
        edge |> push(-tie)
        edge |> push(float(k) * 5.9604645e-8)
        edge |> push(float(k + 1) * 0.5e-5)
    }
    edge |> push(65504.0)
    edge |> push(-65504.0)
    edge |> push(-0.0)
    edge |> push(0.0)
    edge |> push(1.0e-9)
    return <- edge
}

[test]
def test_f32_to_f16_forms(t : T?) {
    t |> run("cvt_f32_to_f16 bit-matches the float16 cast over the tail sweep and the rounding edges") @(t : T?) {
        var total = 0
        for (n in SWEEP) {
            total += f16_form_mismatches(lcg_unit(n, 77u))
        }
        total += f16_form_mismatches(f16_edge_values())
        t |> equal(total, 0, "the row convert and the cast must round alike inside the half range")
    }
    t |> run("cvt_f32_to_f16 clamps past the half range where the cast overflows - the control") @(t : T?) {
        var big <- [70000.0, -70000.0, 1.0e9, -1.0e9]
        var out : array<uint16>
        out |> resize(length(big))
        cvt_f32_to_f16(unsafe(addr(out[0])), unsafe(addr(big[0])), long_length(big))
        for (w, v in out, big) {
            t |> equal(w, f16_cast_bits(v > 0.0 ? 65504.0 : -65504.0), "a value past the half range must land on the largest half")
        }
        t |> equal(f16_form_mismatches(big), 4, "the cast overflows where the convert clamps")
    }
}

[test]
def test_active_q8_repack_layout(t : T?) {
    t |> run("active_q8_repack_layout: the disk-order record off the repack, the backend's layout on it") @(t : T?) {
        let plain = active_q8_repack_layout(false)
        t |> equal(plain.mr, 1l, "disk order: one row a tile")
        t |> equal(plain.kgroup, 4l, "disk order: the k group of four")
        t |> equal(plain.wbias, 0l, "disk order: no weight bias")
        let packed = active_q8_repack_layout(true)
        t |> equal(packed.mr, active_q8_layout_mr(), "repacked: the backend's row tile")
        t |> equal(packed.kgroup, active_q8_kgroup(), "repacked: the backend's k group")
        t |> equal(packed.wbias, active_q8_wbias(), "repacked: the backend's weight bias")
    }
}
