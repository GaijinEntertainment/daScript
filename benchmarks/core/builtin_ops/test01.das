options gen2
options persistent_heap

// empty() / length() call-throughput micro-benchmark for array and table.
//
// Each lane walks a prebuilt vector of containers (half filled, half empty) and either counts
// the empty ones (empty lanes) or sums their lengths (length reference lanes). The vector is
// built outside the measured block; every call reads a distinct runtime element with a
// data-dependent result, so nothing const-folds and the verified accumulator defeats DCE.
//
// Target measurement is INTERP: there all four lanes are plain extern calls, so empty/array
// and empty/table can be compared directly to the length/* reference lanes. Note the JIT
// numbers are asymmetric — length(table) is NOT JIT-inlined today, while empty(array),
// empty(table) and length(array) all inline — so only use JIT output as a smoke check.

require dastest/testing_boost

let N = 1000           // containers per vector (N/2 filled, N/2 empty)
let FILL = 4           // elements pushed into each filled container

def build_arrays : array<array<int>> {
    var arrs : array<array<int>>
    arrs |> reserve(N)
    for (i in range(N)) {
        var a : array<int>
        if ((i & 1) == 1) {
            a |> reserve(FILL)
            for (j in range(FILL)) {
                a |> push(j)
            }
        }
        arrs |> emplace(a)
    }
    return <- arrs
}

def build_tables : array<table<int; int>> {
    var tabs : array<table<int; int>>
    tabs |> reserve(N)
    for (i in range(N)) {
        var t : table<int; int>
        if ((i & 1) == 1) {
            for (j in range(FILL)) {
                t |> insert(j, j)
            }
        }
        tabs |> emplace(t)
    }
    return <- tabs
}

[benchmark]
def empty_array(b : B?) {
    var arrs <- build_arrays()
    b |> run("empty/array", N) {
        var cnt = 0
        for (a in arrs) {
            if (empty(a)) {
                cnt++
            }
        }
        if (cnt != N / 2) {
            b->failNow()
        }
    }
    unsafe { delete arrs; }
}

[benchmark]
def empty_table(b : B?) {
    var tabs <- build_tables()
    b |> run("empty/table", N) {
        var cnt = 0
        for (t in tabs) {
            if (empty(t)) {
                cnt++
            }
        }
        if (cnt != N / 2) {
            b->failNow()
        }
    }
    unsafe { delete tabs; }
}

[benchmark]
def length_array(b : B?) {
    var arrs <- build_arrays()
    b |> run("length/array", N) {
        var total = 0
        for (a in arrs) {
            total += length(a)
        }
        if (total != (N / 2) * FILL) {
            b->failNow()
        }
    }
    unsafe { delete arrs; }
}

[benchmark]
def length_table(b : B?) {
    var tabs <- build_tables()
    b |> run("length/table", N) {
        var total = 0
        for (t in tabs) {
            total += length(t)
        }
        if (total != (N / 2) * FILL) {
            b->failNow()
        }
    }
    unsafe { delete tabs; }
}
