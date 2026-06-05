options gen2
options persistent_heap

// Small integer-keyed table micro-benchmarks.
// Sweeps table size N across the small regime to locate the linear-vs-hashed
// crossover (maxLinearCapacity). Three decisive operations: build (insert+alloc),
// positive lookup (hit), negative lookup (miss, worst case for a linear scan).
//
// hit/miss repeat the N-key sweep REPS times inside the measured block (chunk =
// N*REPS) so fixed per-run harness overhead amortizes away and ns/op reflects a
// single lookup at table-size N — the number the crossover decision turns on.

require dastest/testing_boost

let SIZES = [1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64]
let REPS = 64

[benchmark]
def small_int_build(b : B?) {
    for (n in SIZES) {
        b |> run("build/{n}", n) {
            var tab : table<int; int>
            for (i in range(n)) {
                unsafe(tab[i]) = i * 2
            }
            unsafe { delete tab; }
        }
    }
}

[benchmark]
def small_int_hit(b : B?) {
    for (n in SIZES) {
        var tab : table<int; int>
        for (i in range(n)) {
            unsafe(tab[i]) = i * 2
        }
        let expected = REPS * (n - 1) * n     // REPS * sum(i*2, i in 0..n-1)
        b |> run("hit/{n}", n * REPS) {
            var s = 0
            for (r in range(REPS)) {
                for (i in range(n)) {
                    s += tab?[i] ?? 0
                }
            }
            if (s != expected) {
                b->failNow()
            }
        }
        unsafe { delete tab; }
    }
}

[benchmark]
def small_int_miss(b : B?) {
    for (n in SIZES) {
        var tab : table<int; int>
        for (i in range(n)) {
            unsafe(tab[i]) = i * 2
        }
        b |> run("miss/{n}", n * REPS) {
            var s = 0
            for (r in range(REPS)) {
                for (i in range(n)) {
                    s += tab?[i + 1000000] ?? 0
                }
            }
            if (s != 0) {
                b->failNow()
            }
        }
        unsafe { delete tab; }
    }
}
