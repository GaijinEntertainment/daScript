options gen2
options persistent_heap

// Packed (N=8) integer-key find and tab[key] update micro-benchmarks — the paths added
// when the inline SIMD packed find was extended to integer keys and to the insert path.
//
//   int_hit / int_miss : tab?[k] on an int-keyed table. The packed path is a pure <8 x i32>
//                        key compare with a liveness mask — NO hash at all (unlike strings).
//   str_update / int_update : tab[k] = v on keys already present. The at path runs the same
//                        SIMD find; a hit returns the slot with no insert and no C++ call.
//
// All tables are filled once outside the measured block; find lanes never insert.

require dastest/testing_boost

let REPS = 64

let IKEYS = [10, 20, 30, 40, 50, 60, 70, 80]
let IABSENT = [11, 21, 31, 41, 51, 61, 71, 81]
let SKEYS = ["alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel"]

def fill_int(var tab : table<int; int>) {
    for (i in range(length(IKEYS))) {
        unsafe(tab[IKEYS[i]]) = i + 1
    }
}

def fill_str(var tab : table<string; int>) {
    for (i in range(length(SKEYS))) {
        unsafe(tab[SKEYS[i]]) = i + 1
    }
}

// Distinct-pointer copy so str_update's at-hit confirm runs a real strcmp rather than KeyCompare's
// a==b pointer-identity fast path (querying with the stored SKEYS[i] would understate the update).
def fresh(s : string) : string {
    return clone_string(s)
}

[benchmark]
def int_hit(b : B?) {
    var tab : table<int; int>
    fill_int(tab)
    let k0 = IKEYS[0]; let k1 = IKEYS[1]; let k2 = IKEYS[2]; let k3 = IKEYS[3]
    let k4 = IKEYS[4]; let k5 = IKEYS[5]; let k6 = IKEYS[6]; let k7 = IKEYS[7]
    b |> run("int_hit/8", 8 * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            s += tab?[k0] ?? 0; s += tab?[k1] ?? 0; s += tab?[k2] ?? 0; s += tab?[k3] ?? 0
            s += tab?[k4] ?? 0; s += tab?[k5] ?? 0; s += tab?[k6] ?? 0; s += tab?[k7] ?? 0
        }
        if (s != REPS * 36) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}

[benchmark]
def int_miss(b : B?) {
    var tab : table<int; int>
    fill_int(tab)
    let k0 = IABSENT[0]; let k1 = IABSENT[1]; let k2 = IABSENT[2]; let k3 = IABSENT[3]
    let k4 = IABSENT[4]; let k5 = IABSENT[5]; let k6 = IABSENT[6]; let k7 = IABSENT[7]
    b |> run("int_miss/8", 8 * REPS) {
        var s = 0
        for (_r in range(REPS)) {
            s += tab?[k0] ?? 0; s += tab?[k1] ?? 0; s += tab?[k2] ?? 0; s += tab?[k3] ?? 0
            s += tab?[k4] ?? 0; s += tab?[k5] ?? 0; s += tab?[k6] ?? 0; s += tab?[k7] ?? 0
        }
        if (s != 0) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}

[benchmark]
def int_update(b : B?) {
    var tab : table<int; int>
    fill_int(tab)
    let k0 = IKEYS[0]; let k1 = IKEYS[1]; let k2 = IKEYS[2]; let k3 = IKEYS[3]
    let k4 = IKEYS[4]; let k5 = IKEYS[5]; let k6 = IKEYS[6]; let k7 = IKEYS[7]
    b |> run("int_update/8", 8 * REPS) {
        for (_r in range(REPS)) {
            unsafe(tab[k0]) ++; unsafe(tab[k1]) ++; unsafe(tab[k2]) ++; unsafe(tab[k3]) ++
            unsafe(tab[k4]) ++; unsafe(tab[k5]) ++; unsafe(tab[k6]) ++; unsafe(tab[k7]) ++
        }
    }
    unsafe { delete tab; }
}

[benchmark]
def str_update(b : B?) {
    var tab : table<string; int>
    fill_str(tab)
    let k0 = fresh(SKEYS[0]); let k1 = fresh(SKEYS[1]); let k2 = fresh(SKEYS[2]); let k3 = fresh(SKEYS[3])
    let k4 = fresh(SKEYS[4]); let k5 = fresh(SKEYS[5]); let k6 = fresh(SKEYS[6]); let k7 = fresh(SKEYS[7])
    b |> run("str_update/8", 8 * REPS) {
        for (_r in range(REPS)) {
            unsafe(tab[k0]) ++; unsafe(tab[k1]) ++; unsafe(tab[k2]) ++; unsafe(tab[k3]) ++
            unsafe(tab[k4]) ++; unsafe(tab[k5]) ++; unsafe(tab[k6]) ++; unsafe(tab[k7]) ++
        }
    }
    unsafe { delete tab; }
}
