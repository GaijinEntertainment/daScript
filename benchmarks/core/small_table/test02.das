options gen2
options persistent_heap

// Small string-keyed table micro-benchmarks.
// Same N sweep as test01 but with string keys, where the hash (strlen + block)
// and the strcmp tiebreak dominate. Key strings are pre-built outside the measured
// block so we isolate table find/insert cost from string construction.
// This is the regime items 2 (linear scan), 3 (hash-only compare) target.
//
// hit/miss repeat the sweep REPS times (chunk = N*REPS) to amortize harness
// overhead — see test01 for the rationale.

require dastest/testing_boost

let SIZES = [1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64]
let REPS = 64

def build_keys(n : int; prefix : string) : array<string> {
    var keys <- [for (i in range(n)); "{prefix}_{i}"]
    return <- keys
}

[benchmark]
def small_str_build(b : B?) {
    for (n in SIZES) {
        var keys <- build_keys(n, "key")
        b |> run("build/{n}", n) {
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
def small_str_hit(b : B?) {
    for (n in SIZES) {
        var keys <- build_keys(n, "key")
        // probe holds the SAME content but distinct pointers, so the KeyCompare a==b
        // fast path misses and a real strcmp runs — the realistic lookup case (keys
        // are parsed/built independently of what was stored), not interned reuse.
        var probe <- build_keys(n, "key")
        var tab <- {for (i in range(n)); keys[i] => i * 2}
        let expected = REPS * (n - 1) * n     // REPS * sum(i*2, i in 0..n-1)
        b |> run("hit/{n}", n * REPS) {
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
def small_str_miss(b : B?) {
    for (n in SIZES) {
        var keys <- build_keys(n, "key")
        var misses <- build_keys(n, "absent")
        var tab <- {for (i in range(n)); keys[i] => i * 2}
        b |> run("miss/{n}", n * REPS) {
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
