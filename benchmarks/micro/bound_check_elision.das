options gen2
options persistent_heap

require dastest/testing_boost

// Array-index-heavy loops that the interpreter bound-check elision pass can prove in range.
// The hot loops live in plain functions (not inside the `run` block) because the pass analyses
// a function's own CFG - accesses nested in a block/lambda argument are left checked.
//
// Neutral workload: this file does NOT enable `options bound_check_elision`, so it measures the
// baseline (checked) path by default. Re-run with the option on to measure the elided path; the
// delta is the per-access bounds-check cost the pass removes.

let N = 100000

// read+write over a dynamic array indexed by range(length(a)) -> symbolic length fact
def array_rw(var a : array<int>) {
    for (i in range(length(a))) {
        a[i] = a[i] + 1
    }
}

// read-only reduction, index bounded by the same length
def array_sum(a : array<int>) : int {
    var s = 0
    for (i in range(length(a))) {
        s += a[i]
    }
    return s
}

// constant-range loop over a fixed-size array -> constant-interval fact
def fixed_rw(var fx : int[256]&) {
    for (i in range(256)) {
        fx[i] = fx[i] + 1
    }
}

[benchmark]
def bench_dynamic_array(b : B?) {
    var a : array<int>
    a |> resize(N)
    for (i in range(N)) {
        a[i] = i
    }
    b |> run("array_rw/{N}", N) {
        array_rw(a)
    }
    b |> run("array_sum/{N}", N) {
        a[0] = array_sum(a)   // observe the result so the reduction is not dead-code eliminated
    }
}

[benchmark]
def bench_fixed_array(b : B?) {
    var fx : int[256]
    for (i in range(256)) {
        fx[i] = i
    }
    b |> run("fixed_rw/256", 256) {
        fixed_rw(fx)
    }
}
