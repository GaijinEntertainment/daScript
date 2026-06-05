options gen2
options persistent_heap

// Crossover prototype: predicts maxLinearCapacity BEFORE packed mode exists.
// `lin_*` lanes inline a hash-only linear scan over an array<uint64> of stored
// hashes — the exact shape a packed small-table find would take (hash the query
// once, scan slots comparing 64-bit hashes, no strcmp, no probe). `hash_*` lanes
// run the real open-addressed table ?[] for the same lookups. The N where lin
// crosses above hash is the predicted threshold. Short vs long keys because the
// strcmp the linear scan skips scales with key length, so the crossover does too.
//
// Query keys are distinct pointers from stored (strcmp actually fires in hash_*).
// The das hash() builtin and the table's internal hash are both hash_blockz64,
// so the per-query hash cost is common to both lanes and cancels in the crossover.

require dastest/testing_boost

let SIZES = [2, 4, 8, 12, 16, 24, 32]
let REPS = 64

def build_keys(n : int; prefix : string) : array<string> {
    var keys <- [for (i in range(n)); "{prefix}{i}"]
    return <- keys
}

def run_lanes(b : B?; n : int; tag : string; prefix : string) {
    var keys <- build_keys(n, prefix)
    var probe <- build_keys(n, prefix)
    var hashes : array<uint64>
    var vals : array<int>
    var tab : table<string; int>
    hashes |> reserve(n)
    vals |> reserve(n)
    for (i in range(n)) {
        hashes |> push(hash(keys[i]))
        vals |> push(i * 2)
        unsafe(tab[keys[i]]) = i * 2
    }
    let expected = REPS * (n - 1) * n
    // linear: hash-only scan, inlined (packed-find shape)
    b |> run("lin_{tag}/{n}", n * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            for (i in range(n)) {
                let h = hash(probe[i])
                var idx = -1
                for (j in range(n)) {
                    if (hashes[j] == h) {
                        idx = j
                        break
                    }
                }
                s += idx >= 0 ? vals[idx] : 0
            }
        }
        if (s != expected) {
            b->failNow()
        }
    }
    // hashed: real open-addressed table find
    b |> run("hash_{tag}/{n}", n * REPS) {
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
    delete hashes
    delete vals
}

[benchmark]
def crossover_short(b : B?) {
    for (n in SIZES) {
        run_lanes(b, n, "short", "key")
    }
}

[benchmark]
def crossover_long(b : B?) {
    for (n in SIZES) {
        run_lanes(b, n, "long", "this/is/a/fairly/long/identifier/")
    }
}
