options gen2

require dastest/testing_boost
require strings

// Distinct-pointer key: built fresh each call so query pointers differ from stored
// ones, forcing a real KeyCompare (not the a==b fast path).
def dkey(prefix : string; i : int) : string {
    return "{prefix}_{i}"
}

def sorted_keys(tab : table<int; int>) : array<int> {
    var got : array<int>
    for (k in keys(tab)) {
        got |> push(k)
    }
    sort(got)
    return <- got
}

def check_keys(t : T?; tab : table<int; int>; expected : array<int>) {
    var got <- sorted_keys(tab)
    t |> equal(length(got), length(expected))
    if (length(got) == length(expected)) {
        for (a, b in got, expected) {
            t |> equal(a, b)
        }
    }
    delete got
}

[test]
def test_packed_int_basic(t : T?) {
    var tab : table<int; int>
    for (i in range(6)) {              // 6 <= maxLinearCapacity (8) -> packed
        unsafe(tab[i]) = i * 10
    }
    t |> equal(length(tab), 6)
    for (i in range(6)) {
        t |> equal(tab?[i] ?? -1, i * 10)        // hit
    }
    t |> equal(tab?[99] ?? -1, -1)               // miss
    t |> equal(key_exists(tab, 3), true)
    t |> equal(key_exists(tab, 99), false)
    check_keys(t, tab, [0, 1, 2, 3, 4, 5])
    unsafe { delete tab; }
}

[test]
def test_packed_string(t : T?) {
    var tab : table<string; int>
    for (i in range(6)) {
        unsafe(tab[dkey("k", i)]) = i * 10
    }
    for (i in range(6)) {
        t |> equal(tab?[dkey("k", i)] ?? -1, i * 10)   // distinct-pointer query
    }
    t |> equal(tab?[dkey("k", 99)] ?? -1, -1)
    t |> equal(key_exists(tab, dkey("k", 2)), true)
    unsafe { delete tab; }
}

[test]
def test_packed_promote(t : T?) {
    // insert past the threshold -> promotes to hashed; everything stays findable
    var tab : table<int; int>
    for (i in range(20)) {
        unsafe(tab[i]) = i * 10
    }
    t |> equal(length(tab), 20)
    for (i in range(20)) {
        t |> equal(tab?[i] ?? -1, i * 10)
    }
    t |> equal(tab?[100] ?? -1, -1)
    unsafe { delete tab; }
}

[test]
def test_packed_erase_swap(t : T?) {
    // swap-remove: erase first / last / middle; survivors stay findable, iteration complete
    var tab : table<int; int>
    for (i in range(6)) {
        unsafe(tab[i]) = i * 10
    }
    t |> equal(erase(tab, 0), true)    // first
    t |> equal(erase(tab, 5), true)    // last
    t |> equal(erase(tab, 2), true)    // middle
    t |> equal(erase(tab, 99), false)  // absent
    t |> equal(length(tab), 3)
    t |> equal(tab?[1] ?? -1, 10)
    t |> equal(tab?[3] ?? -1, 30)
    t |> equal(tab?[4] ?? -1, 40)
    t |> equal(key_exists(tab, 0), false)
    t |> equal(key_exists(tab, 2), false)
    t |> equal(key_exists(tab, 5), false)
    check_keys(t, tab, [1, 3, 4])
    unsafe { delete tab; }
}

[test]
def test_packed_erase_reinsert(t : T?) {
    // dense append after a swap-remove leaves a hole at the tail
    var tab : table<int; int>
    for (i in range(4)) {
        unsafe(tab[i]) = i
    }
    t |> equal(erase(tab, 1), true)
    unsafe(tab[10]) = 100
    t |> equal(length(tab), 4)
    t |> equal(tab?[10] ?? -1, 100)
    t |> equal(tab?[1] ?? -1, -1)
    check_keys(t, tab, [0, 2, 3, 10])
    unsafe { delete tab; }
}

[test]
def test_packed_grow_then_shrink(t : T?) {
    // grow past threshold (hashed), erase back below it -> stays hashed & correct (one-way)
    var tab : table<int; int>
    for (i in range(20)) {
        unsafe(tab[i]) = i
    }
    for (i in range(18)) {
        erase(tab, i)
    }
    t |> equal(length(tab), 2)
    t |> equal(tab?[18] ?? -1, 18)
    t |> equal(tab?[19] ?? -1, 19)
    t |> equal(key_exists(tab, 0), false)
    unsafe { delete tab; }
}

struct PackVal {
    a : int
    b : int
    c : int
}

[test]
def test_packed_struct_value(t : T?) {
    // value larger than a workhorse: exercises valueTypeSize movement in swap-remove + dense copy
    var tab : table<int; PackVal>
    for (i in range(5)) {
        unsafe(tab[i]) = PackVal(a = i, b = i * 2, c = i * 3)
    }
    t |> equal(erase(tab, 2), true)   // swap-removes: last entry's PackVal moves into the hole
    let v3 = unsafe(tab?[3])
    t |> success(v3 != null)
    if (v3 != null) {
        t |> equal(v3.a, 3)
        t |> equal(v3.b, 6)
        t |> equal(v3.c, 9)
    }
    let v4 = unsafe(tab?[4])
    t |> success(v4 != null)
    if (v4 != null) {
        t |> equal(v4.a, 4)
        t |> equal(v4.b, 8)
        t |> equal(v4.c, 12)
    }
    t |> equal(key_exists(tab, 2), false)
    unsafe { delete tab; }
}
