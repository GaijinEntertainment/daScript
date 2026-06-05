options gen2
options persistent_heap

// Realistic "JSON" read: an array of many small string-keyed records (objects), summed by
// looking up each field with a STRING LITERAL key. daslang pools constant strings, so the
// literal used to build a field (`tab["foo"] = 1`) and the literal used to read it
// (`tab?["foo"]` / `tab["foo"]`) are the *same pointer* — the packed find's candidate confirm
// then hits KeyCompare's `a==b` fast path and skips strcmp. This is the common case for code
// that addresses records by fixed field names (config, JSON-shaped data, per-entity property
// maps), as opposed to the distinct-pointer lanes in test03/06 (keys arriving from I/O).
//
//   json_find (v1): count += tab?["foo"] ?? 0   -- the find path (ExprSafeAt)
//   json_at   (v2): count += tab["foo"]          -- the at/index path (ExprAt)

require dastest/testing_boost

let NTABLES = 4096
let NKEYS = 6                       // <= 8 -> every record stays packed
let PER_TABLE_SUM = 21              // 1+2+3+4+5+6
let EXPECTED = NTABLES * PER_TABLE_SUM

def build_record(var tab : table<string; int>) {
    tab["foo"] = 1
    tab["bar"] = 2
    tab["far"] = 3
    tab["baz"] = 4
    tab["qux"] = 5
    tab["quux"] = 6
}

def make_tables(var tables : array<table<string; int>>) {
    tables |> reserve(NTABLES)
    for (i in range(NTABLES)) {
        var tab : table<string; int>
        build_record(tab)
        tables |> emplace(tab)
    }
}

def free_tables(var tables : array<table<string; int>>) {
    for (t in tables) {
        unsafe { delete t; }
    }
    unsafe { delete tables; }
}

// v1 — find path: tab?[key] ?? 0
[benchmark]
def json_find(b : B?) {
    var tables : array<table<string; int>>
    make_tables(tables)
    b |> run("json_find/6", NTABLES * NKEYS) {
        var count = 0
        for (tab in tables) {
            count += tab?["foo"] ?? 0
            count += tab?["bar"] ?? 0
            count += tab?["far"] ?? 0
            count += tab?["baz"] ?? 0
            count += tab?["qux"] ?? 0
            count += tab?["quux"] ?? 0
        }
        if (count != EXPECTED) {
            b->failNow()
        }
    }
    free_tables(tables)
}

// v2 — at/index path: tab[key] (keys all present, so it reads; routes through the insert-or-get at)
[benchmark]
def json_at(b : B?) {
    var tables : array<table<string; int>>
    make_tables(tables)
    b |> run("json_at/6", NTABLES * NKEYS) {
        var count = 0
        for (tab in tables) {
            count += tab["foo"]
            count += tab["bar"]
            count += tab["far"]
            count += tab["baz"]
            count += tab["qux"]
            count += tab["quux"]
        }
        if (count != EXPECTED) {
            b->failNow()
        }
    }
    free_tables(tables)
}
