options gen2
options persistent_heap

// Large (open-addressed) table benchmarks: N >> TABLE_MAX_LINEAR_CAPACITY (8), so these run on the
// hashed regime, not the packed small mode (that is core/small_table). They quantify the non-string
// control-byte change: a large non-string table stores a 1-byte control state per slot instead of a
// 32-bit hash and compares keys directly. The int lanes show the effect (memory via B/op + cache
// density on the probe walk); the string lanes guard against regression (strings keep their 32-bit
// hash). Sizes stay cache-resident so the number reflects the algorithm, not DRAM latency.
//
// hit/miss repeat the N-key sweep REPS times (chunk = N*REPS) to amortize fixed harness overhead.

require dastest/testing_boost

let SIZES = [64, 256, 1024]
let REPS = 16

// ---- int keys (the control-byte change) ----

[benchmark]
def large_int_build(b : B?) {
    for (n in SIZES) {
        b |> run("int_build/{n}", n) {
            var tab : table<int; int>  // nolint:STYLE027  measuring the build (watch B/op)
            for (i in range(n)) {
                unsafe(tab[i]) = i * 2
            }
            unsafe { delete tab; }
        }
    }
}

[benchmark]
def large_int_hit(b : B?) {
    for (n in SIZES) {
        var tab <- {for (i in range(n)); i => i * 2}
        let expected = REPS * (n - 1) * n     // REPS * sum(i*2, i in 0..n-1)
        b |> run("int_hit/{n}", n * REPS) {
            var s = 0
            for (_r in range(REPS)) {
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
def large_int_miss(b : B?) {
    for (n in SIZES) {
        var tab <- {for (i in range(n)); i => i * 2}
        b |> run("int_miss/{n}", n * REPS) {
            var s = 0
            for (_r in range(REPS)) {
                for (i in range(n)) {
                    s += tab?[i + n] ?? 0     // keys [n, 2n) were never inserted
                }
            }
            if (s != 0) {
                b->failNow()
            }
        }
        unsafe { delete tab; }
    }
}

// ---- string keys (no-regression guard; strings keep their 32-bit hash) ----

def build_keys(n : int; prefix : string) : array<string> {
    var keys <- [for (i in range(n)); "{prefix}_{i}"]
    return <- keys
}

[benchmark]
def large_str_build(b : B?) {
    for (n in SIZES) {
        var keys <- build_keys(n, "key")
        b |> run("str_build/{n}", n) {
            var tab : table<string; int>  // nolint:STYLE027  measuring the build
            for (i in range(n)) {
                unsafe(tab[keys[i]]) = i * 2
            }
            unsafe { delete tab; }
        }
        delete keys
    }
}

[benchmark]
def large_str_hit(b : B?) {
    for (n in SIZES) {
        var keys <- build_keys(n, "key")
        var probe <- build_keys(n, "key")    // distinct pointers -> real strcmp confirm
        var tab <- {for (i in range(n)); keys[i] => i * 2}
        let expected = REPS * (n - 1) * n
        b |> run("str_hit/{n}", n * REPS) {
            var s = 0
            for (_r in range(REPS)) {
                for (i in range(n)) {
                    s += tab?[probe[i]] ?? 0
                }
            }
            if (s != expected) {
                b->failNow()
            }
        }
        unsafe { delete tab; }
        delete keys
        delete probe
    }
}

[benchmark]
def large_str_miss(b : B?) {
    for (n in SIZES) {
        var keys <- build_keys(n, "key")
        var misses <- build_keys(n, "absent")
        var tab <- {for (i in range(n)); keys[i] => i * 2}
        b |> run("str_miss/{n}", n * REPS) {
            var s = 0
            for (_r in range(REPS)) {
                for (i in range(n)) {
                    s += tab?[misses[i]] ?? 0
                }
            }
            if (s != 0) {
                b->failNow()
            }
        }
        unsafe { delete tab; }
        delete keys
        delete misses
    }
}
