options gen2

module _common shared public

require dastest/testing_boost public

//! Minimal shared fixtures for the raw sort-family benchmarks. No SQL table /
//! Car-shaped data here (that lives in benchmarks/sql/_common.das); benchmarks
//! in this directory operate on plain ``array<int>`` plus one small struct.

struct public KeyedItem {
    key : int
    payload_a : int
    payload_b : int
}

def public make_int_array(n : int) : array<int> {
    //! Pseudo-random fixed sequence of ``n`` ints; same modulo trick as
    //! benchmarks/sql/_common.das for cross-benchmark comparability.
    var arr : array<int>
    arr |> reserve(n)
    for (i in range(n)) {
        arr |> push((i * 37) % 1000)
    }
    return <- arr
}

def public make_struct_array(n : int) : array<KeyedItem> {
    //! ``n`` ``KeyedItem`` records with pseudo-random key + indexed payload.
    var arr : array<KeyedItem>
    arr |> reserve(n)
    for (i in range(n)) {
        arr |> push(KeyedItem(key = (i * 37) % 1000, payload_a = i, payload_b = i * 2))
    }
    return <- arr
}
