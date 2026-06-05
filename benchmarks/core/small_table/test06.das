options gen2
options persistent_heap

// Lookup-only (never insert) micro-benchmark for the packed (N=8) string-table find.
// Every lane uses tab?[key] (ExprSafeAt = find-only); the table is filled once outside the
// measured block. Targets the JIT-side inlined packed find (none / scalar / vector, toggled
// by JIT_STRING_FIND in llvm_jit_common.das). Miss is the worst case — it scans all 8 slots —
// and is where the branch-free vector scan should pull ahead of scalar / the C++ call.
//
// const_* keys are string literals (hash folds to an immediate); var_* keys arrive through a
// prebuilt array (runtime hash). hit looks up the 8 present keys; miss looks up 8 absent ones.

require dastest/testing_boost

let REPS = 64

let KEYS = ["alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel"]
let ABSENT = ["alpha_x", "bravo_x", "charlie_x", "delta_x", "echo_x", "foxtrot_x", "golf_x", "hotel_x"]

def fill(var tab : table<string; int>) {
    for (i in range(length(KEYS))) {
        unsafe(tab[KEYS[i]]) = i + 1
    }
}

[benchmark]
def const_hit(b : B?) {
    var tab : table<string; int>
    fill(tab)
    b |> run("const_hit/8", 8 * REPS) {
        var s = 0
        for (r in range(REPS)) {
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

[benchmark]
def const_miss(b : B?) {
    var tab : table<string; int>
    fill(tab)
    b |> run("const_miss/8", 8 * REPS) {
        var s = 0
        for (r in range(REPS)) {
            s += tab?["alpha_x"] ?? 0
            s += tab?["bravo_x"] ?? 0
            s += tab?["charlie_x"] ?? 0
            s += tab?["delta_x"] ?? 0
            s += tab?["echo_x"] ?? 0
            s += tab?["foxtrot_x"] ?? 0
            s += tab?["golf_x"] ?? 0
            s += tab?["hotel_x"] ?? 0
        }
        if (s != 0) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}

[benchmark]
def var_hit(b : B?) {
    var tab : table<string; int>
    fill(tab)
    let k0 = KEYS[0]; let k1 = KEYS[1]; let k2 = KEYS[2]; let k3 = KEYS[3]
    let k4 = KEYS[4]; let k5 = KEYS[5]; let k6 = KEYS[6]; let k7 = KEYS[7]
    b |> run("var_hit/8", 8 * REPS) {
        var s = 0
        for (r in range(REPS)) {
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
def var_miss(b : B?) {
    var tab : table<string; int>
    fill(tab)
    let k0 = ABSENT[0]; let k1 = ABSENT[1]; let k2 = ABSENT[2]; let k3 = ABSENT[3]
    let k4 = ABSENT[4]; let k5 = ABSENT[5]; let k6 = ABSENT[6]; let k7 = ABSENT[7]
    b |> run("var_miss/8", 8 * REPS) {
        var s = 0
        for (r in range(REPS)) {
            s += tab?[k0] ?? 0
            s += tab?[k1] ?? 0
            s += tab?[k2] ?? 0
            s += tab?[k3] ?? 0
            s += tab?[k4] ?? 0
            s += tab?[k5] ?? 0
            s += tab?[k6] ?? 0
            s += tab?[k7] ?? 0
        }
        if (s != 0) {
            b->failNow()
        }
    }
    unsafe { delete tab; }
}
