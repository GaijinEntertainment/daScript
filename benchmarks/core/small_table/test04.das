options gen2
options persistent_heap

// Many small tables (the decs / per-entity shape): thousands of independent
// N-element tables rather than one hot table hammered. This is where item 1
// (smaller minCapacity -> less memory per table) and cache locality matter —
// each table's keys/hashes live on different cache lines, so a sweep touches
// cold memory, unlike the single-table micros that stay in L1.
//
// build/* measures the full alloc+insert+free lifecycle per element (watch B/op).
// query/* sweeps one lookup across every table once, so no table stays hot.

require dastest/testing_boost

let NUM_TABLES = 4096
let TABLE_N = 8

[benchmark]
def many_int_build(b : B?) {
    b |> run("int_build/{TABLE_N}", NUM_TABLES * TABLE_N) {
        var tabs : array<table<int; int>>
        tabs |> resize(NUM_TABLES)
        for (ti in range(NUM_TABLES)) {
            for (k in range(TABLE_N)) {
                unsafe(tabs[ti][k]) = k * 2
            }
        }
        for (ti in range(NUM_TABLES)) {
            unsafe { delete tabs[ti]; }
        }
        delete tabs
    }
}

[benchmark]
def many_int_query(b : B?) {
    var tabs : array<table<int; int>>
    tabs |> resize(NUM_TABLES)
    for (ti in range(NUM_TABLES)) {
        for (k in range(TABLE_N)) {
            unsafe(tabs[ti][k]) = k * 2
        }
    }
    b |> run("int_query/{TABLE_N}", NUM_TABLES) {
        var s = 0
        for (ti in range(NUM_TABLES)) {
            s += tabs[ti]?[3] ?? -1
        }
        if (s != NUM_TABLES * 6) {
            b->failNow()
        }
    }
    for (ti in range(NUM_TABLES)) {
        unsafe { delete tabs[ti]; }
    }
    delete tabs
}

[benchmark]
def many_str_build(b : B?) {
    var keys : array<string>
    for (k in range(TABLE_N)) {
        keys |> push("prop_{k}")
    }
    b |> run("str_build/{TABLE_N}", NUM_TABLES * TABLE_N) {
        var tabs : array<table<string; int>>
        tabs |> resize(NUM_TABLES)
        for (ti in range(NUM_TABLES)) {
            for (k in range(TABLE_N)) {
                unsafe(tabs[ti][keys[k]]) = k * 2
            }
        }
        for (ti in range(NUM_TABLES)) {
            unsafe { delete tabs[ti]; }
        }
        delete tabs
    }
    delete keys
}

[benchmark]
def many_str_query(b : B?) {
    var keys : array<string>
    var probe : array<string>
    for (k in range(TABLE_N)) {
        keys |> push("prop_{k}")
        probe |> push("prop_{k}")
    }
    var tabs : array<table<string; int>>
    tabs |> resize(NUM_TABLES)
    for (ti in range(NUM_TABLES)) {
        for (k in range(TABLE_N)) {
            unsafe(tabs[ti][keys[k]]) = k * 2
        }
    }
    b |> run("str_query/{TABLE_N}", NUM_TABLES) {
        var s = 0
        for (ti in range(NUM_TABLES)) {
            s += tabs[ti]?[probe[3]] ?? -1
        }
        if (s != NUM_TABLES * 6) {
            b->failNow()
        }
    }
    for (ti in range(NUM_TABLES)) {
        unsafe { delete tabs[ti]; }
    }
    delete tabs
    delete keys
    delete probe
}
