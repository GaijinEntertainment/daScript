options gen2
options persistent_heap

// Constant-string-key lookup micro-benchmark (item 4 target).
// `const_*` lanes use string literals as keys — the hash (and strlen) are
// compile-time constant and can be baked into a specialized node. `var_*` lanes
// look up the same strings via a prebuilt array (runtime hash) as the control.
// At baseline both lanes cost the same; the const lane is what item 4 speeds up.
//
// Short- and long-key lanes: item 4's win scales with key length (the hash walk
// it removes is O(len)), so the long lane shows the headroom the short lane hides.

require dastest/testing_boost

let REPS = 64

let SHORT_KEYS = ["alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel"]
let LONG_KEYS = [
    "this/is/a/fairly/long/identifier/alpha",
    "this/is/a/fairly/long/identifier/bravo",
    "this/is/a/fairly/long/identifier/charlie",
    "this/is/a/fairly/long/identifier/delta",
    "this/is/a/fairly/long/identifier/echo",
    "this/is/a/fairly/long/identifier/foxtrot",
    "this/is/a/fairly/long/identifier/golf",
    "this/is/a/fairly/long/identifier/hotel"
]

def fill(var tab : table<string; int>; keys : array<string>) {
    for (i in range(length(keys))) {
        unsafe(tab[keys[i]]) = i + 1
    }
}

// Distinct-pointer copy (same content, fresh allocation). The var_* lanes query with these so
// the find's candidate confirm runs a real strcmp instead of hitting KeyCompare's a==b
// pointer-identity fast path — the realistic case (a key from I/O / parsing is never the stored
// pointer). Querying with the stored array element (KEYS[i]) would short-circuit the strcmp and
// understate find, badly so for long keys.
def fresh(s : string) : string {
    return clone_string(s)
}

[benchmark]
def const_short(b : B?) {
    var tab : table<string; int>
    fill(tab, SHORT_KEYS)
    b |> run("const_short/8", 8 * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            s += tab?["alpha"] ?? 0
            s += tab?["bravo"] ?? 0
            s += tab?["charlie"] ?? 0
            s += tab?["delta"] ?? 0
            s += tab?["echo"] ?? 0
            s += tab?["foxtrot"] ?? 0
            s += tab?["golf"] ?? 0
            s += tab?["hotel"] ?? 0
        }
        if (s != REPS * 36) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}

// var lanes mirror the const lanes' straight-line shape exactly — 8 inlined
// lookups, not a loop — so the only difference from const is that the key is a
// runtime string (hash computed at run time) rather than a foldable literal.
// At baseline const == var; item 4 makes const drop below var.
[benchmark]
def var_short(b : B?) {
    var tab : table<string; int>
    fill(tab, SHORT_KEYS)
    let k0 = fresh(SHORT_KEYS[0]); let k1 = fresh(SHORT_KEYS[1]); let k2 = fresh(SHORT_KEYS[2]); let k3 = fresh(SHORT_KEYS[3])
    let k4 = fresh(SHORT_KEYS[4]); let k5 = fresh(SHORT_KEYS[5]); let k6 = fresh(SHORT_KEYS[6]); let k7 = fresh(SHORT_KEYS[7])
    b |> run("var_short/8", 8 * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            s += tab?[k0] ?? 0
            s += tab?[k1] ?? 0
            s += tab?[k2] ?? 0
            s += tab?[k3] ?? 0
            s += tab?[k4] ?? 0
            s += tab?[k5] ?? 0
            s += tab?[k6] ?? 0
            s += tab?[k7] ?? 0
        }
        if (s != REPS * 36) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}

[benchmark]
def const_long(b : B?) {
    var tab : table<string; int>
    fill(tab, LONG_KEYS)
    b |> run("const_long/8", 8 * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            s += tab?["this/is/a/fairly/long/identifier/alpha"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/bravo"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/charlie"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/delta"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/echo"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/foxtrot"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/golf"] ?? 0
            s += tab?["this/is/a/fairly/long/identifier/hotel"] ?? 0
        }
        if (s != REPS * 36) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}

[benchmark]
def var_long(b : B?) {
    var tab : table<string; int>
    fill(tab, LONG_KEYS)
    let k0 = fresh(LONG_KEYS[0]); let k1 = fresh(LONG_KEYS[1]); let k2 = fresh(LONG_KEYS[2]); let k3 = fresh(LONG_KEYS[3])
    let k4 = fresh(LONG_KEYS[4]); let k5 = fresh(LONG_KEYS[5]); let k6 = fresh(LONG_KEYS[6]); let k7 = fresh(LONG_KEYS[7])
    b |> run("var_long/8", 8 * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            s += tab?[k0] ?? 0
            s += tab?[k1] ?? 0
            s += tab?[k2] ?? 0
            s += tab?[k3] ?? 0
            s += tab?[k4] ?? 0
            s += tab?[k5] ?? 0
            s += tab?[k6] ?? 0
            s += tab?[k7] ?? 0
        }
        if (s != REPS * 36) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}
