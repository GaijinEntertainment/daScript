options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require math

//! The [tuned] elementwise kernels - dot, dot_bf16, axpy, add_inplace, add_scale_inplace,
//! mul_inplace, scale_inplace, copy_floats - swept over tail lengths against fp64 references.

var g_taint = 0u

[init]
def init_elementwise_taint() {
    g_taint = 0u
}

[sideeffects]
def private opaque_seed(s : uint) : uint {
    return s ^ g_taint
}

let SWEEP <- [ 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 23, 24, 25, 31, 32, 33,
               63, 64, 65, 100, 127, 128, 129, 1023, 1024, 1025 ]

let TEST_SCALE = -0.75
let SENTINEL = 1.0e9
let POISON = 0.5
let ROUND_U = 1.0e-7
let BAR_FLOOR = 1.0e-6

struct KernelSweep {
    bad : int
    inert : int
    cases : int
    worst : float
    worst_bar : float
    worst_n : int
    poison_inside : int
}

def private note(var r : KernelSweep; n : int; diff, bar : float) {
    if (diff >= r.worst) {
        r.worst = diff
        r.worst_bar = bar
        r.worst_n = n
    }
}

def private dot_bar(n : int) : float {
    return ROUND_U * float(n) * float(n) + BAR_FLOOR
}

def private lcg_grid(n : int; seed : uint; step : float; signed_values : bool) : array<float> {
    var st = opaque_seed(seed)
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
    var st = opaque_seed(seed)
    var a : array<float>
    a |> resize(n)
    for (v in a) {
        st = st * 1664525u + 1013904223u
        v = float(st >> 8u) * (2.0 / 16777216.0) - 1.0
    }
    return <- a
}

def private to_bf16(src : array<float>) : array<uint16> {
    var b : array<uint16>
    b |> resize(length(src))
    for (w, v in b, src) {
        w = uint16(unsafe(reinterpret<uint>(v)) >> 16u)
    }
    return <- b
}

def private bf16_value(w : uint16) : float {
    return unsafe(reinterpret<float>(uint(w) << 16u))
}

def private tally(var r : KernelSweep; n : int; got, want, before : array<float>) {
    r.cases++
    for (g, w, b in got, want, before) {
        if (g != w) {
            r.bad++
            note(r, n, abs(g - w), 0.0)
        }
        if (g == b) {
            r.inert++
        }
    }
}

def private report_cases(t : T?; name : string; r : KernelSweep) {
    t |> equal(r.cases, length(SWEEP), "{name}: every swept length must run")
}

def private report_exact(t : T?; name : string; r : KernelSweep) {
    report_cases(t, name, r)
    t |> equal(r.bad, 0, "{name}: values off the in-test fp64 reference, worst |diff| {r.worst} at n={r.worst_n}")
}

def private report_inplace(t : T?; name : string; r : KernelSweep) {
    report_exact(t, name, r)
    t |> equal(r.inert, 0, "{name}: elements the kernel left exactly where it found them")
}

def private report_bar(t : T?; name : string; r : KernelSweep) {
    report_cases(t, name, r)
    t |> equal(r.bad, 0, "{name}: lengths past the bar, worst |diff| {r.worst} against bar {r.worst_bar} at n={r.worst_n}")
    t |> equal(r.poison_inside, 0, "{name}: poison legs inside the bar - a bar nothing exceeds discriminates nothing")
    feint("{name}: worst |diff| {r.worst} against bar {r.worst_bar} at n={r.worst_n}\n")
}

def private sweep_dot_exact() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let a <- lcg_grid(n, opaque_seed(0x11a3u) + uint(n), 0.125, true)
        let b <- lcg_grid(n, opaque_seed(0x9e37u) + uint(n), 0.375, true)
        var want = 0.0lf
        for (av, bv in a, b) {
            want += double(av) * double(bv)
        }
        let got = dot(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        r.cases++
        if (got != float(want)) {
            r.bad++
            note(r, n, abs(got - float(want)), 0.0)
        }
    }
    return r
}

def private sweep_dot_bar() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let a <- lcg_unit(n, opaque_seed(0x4d2bu) + uint(n))
        let b <- lcg_unit(n, opaque_seed(0xa51fu) + uint(n))
        var want = 0.0lf
        for (av, bv in a, b) {
            want += double(av) * double(bv)
        }
        let got = dot(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        let bar = dot_bar(n)
        let diff = abs(got - float(want))
        r.cases++
        if (diff > bar) {
            r.bad++
        }
        if (abs(got - (float(want) + POISON)) <= bar) {
            r.poison_inside++
        }
        note(r, n, diff, bar)
    }
    return r
}

def private sweep_dot_bf16_exact() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let a <- lcg_grid(n, opaque_seed(0x3c07u) + uint(n), 0.125, true)
        let bf <- lcg_grid(n, opaque_seed(0x7b91u) + uint(n), 0.375, true)
        let b <- to_bf16(bf)
        var want = 0.0lf
        for (av, bw in a, b) {
            want += double(av) * double(bf16_value(bw))
        }
        let got = dot_bf16(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        r.cases++
        if (got != float(want)) {
            r.bad++
            note(r, n, abs(got - float(want)), 0.0)
        }
    }
    return r
}

def private sweep_dot_bf16_bar() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let a <- lcg_unit(n, opaque_seed(0x2f6eu) + uint(n))
        let bf <- lcg_unit(n, opaque_seed(0xc0deu) + uint(n))
        let b <- to_bf16(bf)
        var want = 0.0lf
        for (av, bw in a, b) {
            want += double(av) * double(bf16_value(bw))
        }
        let got = dot_bf16(unsafe(addr(a[0])), unsafe(addr(b[0])), int64(n))
        let bar = dot_bar(n)
        let diff = abs(got - float(want))
        r.cases++
        if (diff > bar) {
            r.bad++
        }
        if (abs(got - (float(want) + POISON)) <= bar) {
            r.poison_inside++
        }
        note(r, n, diff, bar)
    }
    return r
}

def private sweep_copy_floats() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let s <- lcg_unit(n, opaque_seed(0x51c0u) + uint(n))
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

def private sweep_add_inplace() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque_seed(0x6a12u) + uint(n), 0.125, true)
        let s <- lcg_grid(n, opaque_seed(0xb33fu) + uint(n), 0.375, true)
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

def private sweep_add_scale_inplace() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque_seed(0x1de5u) + uint(n), 0.125, false)
        let s <- lcg_grid(n, opaque_seed(0x8c44u) + uint(n), 0.375, false)
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

def private sweep_mul_inplace() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque_seed(0x9077u) + uint(n), 0.125, true)
        let s <- lcg_grid(n, opaque_seed(0x2bb8u) + uint(n), 0.375, true)
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

def private sweep_scale_inplace() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque_seed(0x4e19u) + uint(n), 0.125, true)
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

def private sweep_axpy() : KernelSweep {
    var r = KernelSweep()
    for (n in SWEEP) {
        let before <- lcg_grid(n, opaque_seed(0x7231u) + uint(n), 0.125, true)
        let s <- lcg_grid(n, opaque_seed(0xd4a0u) + uint(n), 0.375, true)
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
    let before <- lcg_grid(4, opaque_seed(0x0e11u), 0.125, true)
    let s <- lcg_grid(4, opaque_seed(0x0e12u), 0.375, true)
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
    let a <- lcg_grid(4, opaque_seed(0x0e13u), 0.125, true)
    let bf <- lcg_grid(4, opaque_seed(0x0e14u), 0.375, true)
    let b <- to_bf16(bf)
    unsafe {
        return abs(dot(addr(a[0]), addr(a[0]), 0l)) + abs(dot_bf16(addr(a[0]), addr(b[0]), 0l))
    }
}

[test]
def test_dot(t : T?) {
    t |> run("dot bit-matches an fp64 accumulation on inputs fp32 cannot round") @(t : T?) {
        report_exact(t, "dot exact", sweep_dot_exact())
    }
    t |> run("dot against an fp64 accumulation on unit-range inputs, with the bar's control") @(t : T?) {
        report_bar(t, "dot bar", sweep_dot_bar())
    }
}

[test]
def test_dot_bf16(t : T?) {
    t |> run("dot_bf16 bit-matches an fp64 accumulation over exactly-bf16 operands") @(t : T?) {
        report_exact(t, "dot_bf16 exact", sweep_dot_bf16_exact())
    }
    t |> run("dot_bf16 against an fp64 accumulation over the widened operands, with the bar's control") @(t : T?) {
        report_bar(t, "dot_bf16 bar", sweep_dot_bf16_bar())
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
