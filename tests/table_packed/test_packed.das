options gen2

require dastest/testing_boost

// Distinct-pointer key: built fresh each call so query pointers differ from stored
// ones, forcing a real KeyCompare (not the a==b fast path).
def dkey(prefix : string; i : int) : string => "{prefix}_{i}"

// Fresh string copy with exact content "k{n}" (distinct pointer each call, forcing real strcmp).
def kcopy(n : int) : string => "k{n}"

def sorted_keys(tab : table<int; int>) : array<int> {
    var got <- [for (k in keys(tab)); k]
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

[test(description = "a small int-keyed table stays in packed mode and reads, writes and finds keys correctly")]
def test_packed_int_basic(t : T?) {
    var tab : table<int; int>    // nolint:STYLE027  built via packed index-assign path under test
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
    delete tab
}

[test(description = "a small string-keyed table in packed mode reads, writes and finds keys correctly")]
def test_packed_string(t : T?) {
    var tab : table<string; int>    // nolint:STYLE027  built via packed index-assign path under test
    for (i in range(6)) {
        unsafe(tab[dkey("k", i)]) = i * 10
    }
    for (i in range(6)) {
        t |> equal(tab?[dkey("k", i)] ?? -1, i * 10)   // distinct-pointer query
    }
    t |> equal(tab?[dkey("k", 99)] ?? -1, -1)
    t |> equal(key_exists(tab, dkey("k", 2)), true)
    delete tab
}

// "k1832" and "k16303" share the same 32-bit table hashKey (0xf9102e8c) but are different
// strings (found by brute force over uint(hash(s))). Packed string tables store the full 64-bit
// hash and compare it exactly, so these two 32-bit-colliding keys coexist in packed mode with no
// strcmp and no promotion — the 64-bit hashes differ. Both must remain findable with their values.
[test(description = "two string keys with the same 32-bit hash both survive in a packed table")]
def test_packed_string_hashkey32_collision(t : T?) {
    var tab : table<string; int>  // nolint:STYLE031 — packed-mode insert path is under test
    unsafe(tab[kcopy(1832)]) = 111
    unsafe(tab[kcopy(16303)]) = 222
    t |> equal(length(tab), 2)
    t |> equal(tab?[kcopy(1832)] ?? -1, 111)  // distinct-pointer queries (clean 64-bit hash compare)
    t |> equal(tab?[kcopy(16303)] ?? -1, 222)
    t |> equal(key_exists(tab, kcopy(1832)), true)
    t |> equal(key_exists(tab, kcopy(16303)), true)
    t |> equal(tab?[kcopy(9999)] ?? -1, -1)
    delete tab
}

// False-positive guard: a single-entry packed table whose only key shares a 32-bit hashKey with
// the query must MISS — the packed find compares the full 64-bit hash, so the 32-bit clash is moot.
[test(description = "a lookup of a missing string key that shares a 32-bit hash with a stored key misses")]
def test_packed_string_hashkey32_miss(t : T?) {
    var tab : table<string; int>
    unsafe(tab[kcopy(1832)]) = 111
    t |> equal(length(tab), 1)
    t |> equal(tab?[kcopy(16303)] ?? -777, -777)   // 32-bit hashKey clash, distinct 64-bit hash -> miss
    t |> equal(key_exists(tab, kcopy(16303)), false)
    t |> equal(tab?[kcopy(1832)] ?? -1, 111)       // the real key still hits
    delete tab
}

[test(description = "a packed table grown past its packed capacity is promoted and keeps every key")]
def test_packed_promote(t : T?) {
    // insert past the threshold -> promotes to hashed; everything stays findable
    var tab : table<int; int>    // nolint:STYLE027  built via index-assign to exercise packed->hashed promotion
    for (i in range(20)) {
        unsafe(tab[i]) = i * 10
    }
    t |> equal(length(tab), 20)
    for (i in range(20)) {
        t |> equal(tab?[i] ?? -1, i * 10)
    }
    t |> equal(tab?[100] ?? -1, -1)
    delete tab
}

[test(description = "swap-removing the first, last and middle keys of a packed table keeps survivors findable and an absent key reports false")]
def test_packed_erase_swap(t : T?) {
    // swap-remove: erase first / last / middle; survivors stay findable, iteration complete
    var tab : table<int; int>    // nolint:STYLE027  built via index-assign then swap-erased under test
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
    delete tab
}

[test(description = "a key erased from a packed table can be inserted again")]
def test_packed_erase_reinsert(t : T?) {
    // dense append after a swap-remove leaves a hole at the tail
    var tab : table<int; int>    // nolint:STYLE027  built via index-assign then erase+reinsert under test
    for (i in range(4)) {
        unsafe(tab[i]) = i
    }
    t |> equal(erase(tab, 1), true)
    unsafe(tab[10]) = 100
    t |> equal(length(tab), 4)
    t |> equal(tab?[10] ?? -1, 100)
    t |> equal(tab?[1] ?? -1, -1)
    check_keys(t, tab, [0, 2, 3, 10])
    delete tab
}

[test(description = "a table grown past packed mode and then erased down keeps the surviving keys")]
def test_packed_grow_then_shrink(t : T?) {
    // grow past threshold (hashed), erase back below it -> stays hashed & correct (one-way)
    var tab : table<int; int>    // nolint:STYLE027  built via index-assign to exercise grow-then-shrink regime
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
    delete tab
}

struct PackVal {
    a : int
    b : int
    c : int
}

[test(description = "a packed table holding struct values reads back every field")]
def test_packed_struct_value(t : T?) {
    // value larger than a workhorse: exercises valueTypeSize movement in swap-remove + dense copy
    var tab : table<int; PackVal>    // nolint:STYLE027  built via index-assign to exercise struct-value swap-remove
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
    delete tab
}

// Const-string table keys exercise the SimNode_*_WithHash interpreter path: the key bytes and
// their hash are baked at simulate time, so the lookup skips the per-call hash walk. tab["lit"]
// and tab?["lit"] are direct intrinsics (ExprAt / ExprSafeAt); find / key_exists go through daslib
// generic wrappers, so the WithHash nodes for those only fire when __builtin_table_* is called
// directly (until the wrappers inline) — tested here via the __builtin_ forms.

[test(description = "indexing a table with a constant string key inserts and reads through the hashed-constant path")]
def test_constkey_index(t : T?) {
    var tab : table<string; int>  // nolint:STYLE031 — TableIndex_WithHash insert node is under test
    tab["foo"] = 1                                   // TableIndex_WithHash (insert)
    tab["bar"] = 2
    tab["baz"] = 3
    t |> equal(length(tab), 3)
    t |> equal(tab["foo"], 1)                         // TableIndex_WithHash (hit, r2v)
    t |> equal(tab["bar"], 2)
    t |> equal(tab["baz"], 3)
    tab["foo"] = 11                                   // overwrite, no new slot
    t |> equal(tab["foo"], 11)
    t |> equal(length(tab), 3)
    delete tab
}

[test(description = "safe indexing with a constant string key hits for a present key and misses for an absent one")]
def test_constkey_safe_index(t : T?) {
    var tab : table<string; int>  // nolint:STYLE031 — TableIndex_WithHash insert node is under test
    tab["foo"] = 1
    tab["bar"] = 2
    t |> equal(tab?["foo"] ?? -1, 1)                  // SafeTableIndex_WithHash (hit)
    t |> equal(tab?["bar"] ?? -1, 2)
    t |> equal(tab?["missing"] ?? -1, -1)             // SafeTableIndex_WithHash (miss, no insert)
    t |> equal(length(tab), 2)                        // safe-index miss did not grow the table
    delete tab
}

[test(description = "key_exists with a constant string key reports presence and absence correctly")]
def test_constkey_builtin_key_exists(t : T?) {
    var tab : table<string; int>  // nolint:STYLE031 — TableIndex_WithHash insert node is under test
    tab["foo"] = 1
    tab["bar"] = 2
    t |> equal(__builtin_table_key_exists(tab, "foo"), true)    // KeyExists_WithHash (hit)
    t |> equal(__builtin_table_key_exists(tab, "bar"), true)
    t |> equal(__builtin_table_key_exists(tab, "missing"), false) // KeyExists_WithHash (miss)
    delete tab
}

[test(description = "find with a constant string key returns a pointer on a hit and null on a miss")]
def test_constkey_builtin_find(t : T?) {
    var tab : table<string; int>  // nolint:STYLE031 — TableIndex_WithHash insert node is under test
    tab["foo"] = 10
    tab["bar"] = 20
    let pf = __builtin_table_find(tab, "foo")        // TableFind_WithHash (hit)
    t |> success(pf != null)
    if (pf != null) {
        t |> equal(*(unsafe(reinterpret<int?>(pf))), 10)
    }
    let pm = __builtin_table_find(tab, "missing")    // TableFind_WithHash (miss)
    t |> equal(pm == null, true)
    delete tab
}

// "k1832" and "k16303" share the same 32-bit table hashKey but differ in the full 64-bit hash.
// As literal keys the WithHash node bakes each one's 64-bit hash, so a packed table holds both
// with no strcmp and no promotion; each must be found with its own value, and a 32-bit-colliding
// third literal must miss.
[test(description = "constant string keys whose 32-bit hashes collide are stored as distinct entries")]
def test_constkey_hashkey32_collision(t : T?) {
    var tab : table<string; int>  // nolint:STYLE031 — TableIndex_WithHash insert node is under test
    tab["k1832"] = 111                               // TableIndex_WithHash
    tab["k16303"] = 222
    t |> equal(length(tab), 2)
    t |> equal(tab?["k1832"] ?? -1, 111)             // SafeTableIndex_WithHash, exact 64-bit hash
    t |> equal(tab?["k16303"] ?? -1, 222)
    t |> equal(__builtin_table_key_exists(tab, "k1832"), true)
    t |> equal(__builtin_table_key_exists(tab, "k16303"), true)
    t |> equal(tab?["k9999"] ?? -1, -1)
    delete tab
}

// Large (open-addressed) table regression for the non-string control-byte representation
// (commit B): N > TABLE_MAX_LINEAR_CAPACITY (8) forces promotion past the packed regime, so a
// non-string table stores a 1-byte control state per slot and compares keys directly. Strings keep
// their 32-bit hash; the string lane here guards that the shared (if-constexpr-split) large-table
// find/reserve/erase still works.

// Distinct-pointer string key (fresh each call) so queries differ from the stored pointers,
// forcing a real KeyCompare on the large-table string path.
def large_dkey(i : int) : string => "key_{i}"

def sorted_int_keys(tab : table<int; int>) : array<int> {
    var got <- [for (k in keys(tab)); k]
    sort(got)
    return <- got
}

def check_int_keys(t : T?; tab : table<int; int>; expected : array<int>) {
    var got <- sorted_int_keys(tab)
    t |> equal(length(got), length(expected))
    if (length(got) == length(expected)) {
        for (a, b in got, expected) {
            t |> equal(a, b)
        }
    }
    delete got
}

[test(description = "a large int-keyed table finds present keys and misses absent ones")]
def test_large_int_hit_miss(t : T?) {
    var tab : table<int; int>    // nolint:STYLE027  index-assign builds the large table under test
    for (i in range(40)) {        // 40 > 8 -> open-addressed
        unsafe(tab[i]) = i * 10
    }
    t |> equal(length(tab), 40)
    for (i in range(40)) {
        t |> equal(tab?[i] ?? -1, i * 10)        // hit
    }
    t |> equal(tab?[999] ?? -1, -1)              // miss past the end
    t |> equal(tab?[-7] ?? -1, -1)               // miss (negative key)
    t |> equal(key_exists(tab, 17), true)
    t |> equal(key_exists(tab, 100), false)
    var expected <- [for (i in range(40)); i]
    check_int_keys(t, tab, expected)
    delete expected
    delete tab
}

[test(description = "after erasing every third key of a large table, lookups skip the tombstones and miss the erased keys")]
def test_large_int_erase_tombstone(t : T?) {
    var tab : table<int; int>    // nolint:STYLE027  index-assign + erase under test
    for (i in range(40)) {
        unsafe(tab[i]) = i
    }
    // erase every third key -> tombstones scattered through the probe chains
    for (i in range(40)) {
        if (i % 3 == 0) {
            t |> equal(erase(tab, i), true)
        }
    }
    for (i in range(40)) {
        if (i % 3 == 0) {
            t |> equal(tab?[i] ?? -1, -1)        // erased -> miss (probe skips the tombstone)
            t |> equal(key_exists(tab, i), false)
        } else {
            t |> equal(tab?[i] ?? -1, i)         // survivor still findable
        }
    }
    // reinsert the erased keys -> tombstone reuse
    for (i in range(40)) {
        if (i % 3 == 0) {
            unsafe(tab[i]) = i + 1000
        }
    }
    for (i in range(40)) {
        let want = (i % 3 == 0) ? i + 1000 : i
        t |> equal(tab?[i] ?? -1, want)
    }
    t |> equal(length(tab), 40)
    delete tab
}

[test(description = "a large table that grows through repeated rehashes keeps every key and value")]
def test_large_int_grow_rehash(t : T?) {
    // grow through several doublings; each grow rehashes the control-byte source, recomputing
    // every live key's hash to re-bucket into the larger target.
    var tab : table<int; int>    // nolint:STYLE027  index-assign drives repeated grow
    for (i in range(300)) {
        unsafe(tab[i]) = i * 2
    }
    t |> equal(length(tab), 300)
    for (i in range(300)) {
        t |> equal(tab?[i] ?? -1, i * 2)
    }
    t |> equal(tab?[300] ?? -1, -1)
    delete tab
}

[test(description = "repeated insert and erase rounds on a large table leave it consistent")]
def test_large_int_churn(t : T?) {
    // insert/erase churn builds tombstones -> same-capacity rehash on the control-byte path.
    var tab : table<int; int>    // churn under test
    let lastRound = 5
    for (round in range(lastRound + 1)) {
        for (i in range(50)) {
            unsafe(tab[i]) = round * 1000 + i
        }
        for (i in range(50)) {
            if (i % 2 == 0) {
                erase(tab, i)
            }
        }
    }
    // odd keys carry the last round's value; even keys were erased last
    for (i in range(50)) {
        if (i % 2 == 0) {
            t |> equal(key_exists(tab, i), false)
        } else {
            t |> equal(tab?[i] ?? -1, lastRound * 1000 + i)
        }
    }
    delete tab
}

[test(description = "a large string-keyed table finds present keys and misses absent ones")]
def test_large_string(t : T?) {
    var tab : table<string; int>    // nolint:STYLE027  index-assign builds the large string table
    for (i in range(40)) {
        unsafe(tab[large_dkey(i)]) = i * 7
    }
    for (i in range(40)) {
        t |> equal(tab?[large_dkey(i)] ?? -1, i * 7)   // distinct-pointer query (real KeyCompare)
    }
    t |> equal(tab?[large_dkey(999)] ?? -1, -1)
    t |> equal(key_exists(tab, large_dkey(13)), true)
    t |> equal(erase(tab, large_dkey(13)), true)
    t |> equal(key_exists(tab, large_dkey(13)), false)
    t |> equal(tab?[large_dkey(14)] ?? -1, 98)
    t |> equal(length(tab), 39)
    // reinsert after erase: a new key reuses the freed tombstone slot (no grow)
    unsafe(tab[large_dkey(1000)]) = 7000
    t |> equal(length(tab), 40)
    t |> equal(tab?[large_dkey(1000)] ?? -1, 7000)
    t |> equal(tab?[large_dkey(13)] ?? -1, -1)
    t |> equal(tab?[large_dkey(0)] ?? -1, 0)
    delete tab
}

struct LargeVal {
    a : int
    b : int
    c : int
}

[test(description = "a large table holding struct values reads back every field")]
def test_large_struct_value(t : T?) {
    // struct value larger than a workhorse: exercises valueTypeSize movement in the large-table
    // erase + rehash copy.
    var tab : table<int; LargeVal>    // nolint:STYLE027  struct-value large table under test
    for (i in range(40)) {
        unsafe(tab[i]) = LargeVal(a = i, b = i * 2, c = i * 3)
    }
    t |> equal(erase(tab, 10), true)
    t |> equal(erase(tab, 20), true)
    let v = unsafe(tab?[30])
    t |> success(v != null)
    if (v != null) {
        t |> equal(v.a, 30)
        t |> equal(v.b, 60)
        t |> equal(v.c, 90)
    }
    t |> equal(key_exists(tab, 10), false)
    t |> equal(length(tab), 38)
    delete tab
}

[test(description = "a table crossing from packed to large keeps all of its entries")]
def test_promote_packed_to_large(t : T?) {
    // 8 packed entries, then the 9th promotes to open addressing: the packed source copies into the
    // control-byte target. Every key must survive.
    var tab : table<int; int>    // nolint:STYLE027  index-assign exercises packed->large promotion
    for (i in range(8)) {
        unsafe(tab[i]) = i * 100
    }
    t |> equal(length(tab), 8)
    unsafe(tab[8]) = 800     // 9th insert -> promote
    t |> equal(length(tab), 9)
    for (i in range(9)) {
        t |> equal(tab?[i] ?? -1, i * 100)
    }
    delete tab
}

[test(description = "iterating the keys of a packed table visits exactly its entries without reading past them")]
def test_keys_iter_packed_no_overread(t : T?) {
    // Regression: TableKeysIterator must not read the key one-past the keys block. A non-string
    // packed table has no hash region after keys, so the past-end read on exhaustion / empty
    // iteration ran off the allocation (ASan heap-buffer-overflow). Cover a full iteration (the
    // final next lands on table_end) and an emptied table (first lands on table_end).
    var tab : table<int; int>    // nolint:STYLE027  packed table iterated under test
    for (i in range(5)) {
        unsafe(tab[i]) = i
    }
    var n1 = 0
    for (_k in keys(tab)) {
        n1++
    }
    t |> equal(n1, 5)
    for (i in range(5)) {
        erase(tab, i)
    }
    t |> equal(length(tab), 0)
    var n2 = 0
    for (_k in keys(tab)) {
        n2++
    }
    t |> equal(n2, 0)
    delete tab
}
