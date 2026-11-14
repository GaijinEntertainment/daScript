options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require UnitTest
require daslib/fuzzer
require daslib/faker

let {
    TA_TOTAL = 100
}

struct TaTestStruct {
    test : string
}

struct TaTableContainer {
    t : table<string; TaTestStruct>
}

def ta_check(var tc : TaTableContainer; deletedEntries : array<int>) {
    for (i in range(1, TA_TOTAL)) {
        var deleted : bool
        for (x in deletedEntries) {
            if (x == i) {
                deleted = true
                break
            }
        }
        if (deleted) {
            continue
        }
        let n = "name_{i}"
        if (tc.t |> get_value(n).test != n) {
            return false
        }
    }
    return true
}

[sideeffects]
def ta_test_find(t : T?) {
    let const_tab <- { "one" => 1, "two" => 2 }
    get(const_tab, "one") $(num) {
        t |> equal(num, 1)
    }
    t |> equal(const_tab?["one"] ?? 100500, 1)
    var tab <- { "one" => 1, "two" => 2 }
    get(tab, "one") $(var num) {
        num = 3
    }
    t |> equal(tab?["one"] ?? 100500, 3)
    get(tab, "one") $(var num) {
        t |> equal(num, 3)
        num = 4
    }
    t |> equal(tab?["one"] ?? 100500, 4)
    get(tab, "one") $(var num) {
        t |> equal(num, 4)
        num = 5
    }
    t |> equal(tab?["one"] ?? 100500, 5)
    unsafe {
        var dummy = 0
        tab?["one"] ?? dummy = 1
        t |> equal(tab?["one"] ?? 100500, 1)
        t |> equal(dummy, 0)
    }
}

[sideeffects]
def ta_test_erase_collision(t : T?) {
    var tc : TaTableContainer
    var names <- [for (i in range(1, TA_TOTAL)); "name_{i}"]
    for (n in names) {
        tc.t |> emplace(n, TaTestStruct(test = n))
    }
    var deletedEntries : array<int>
    deletedEntries |> reserve(TA_TOTAL)
    verify(ta_check(tc, deletedEntries))
    for (i in range(1, TA_TOTAL)) {
        let x = "name_{i}"
        push(deletedEntries, i)
        erase(tc.t, x)
        verify(ta_check(tc, deletedEntries))
    }
    t |> success(true)
}

[sideeffects]
def ta_test_lock_panic(t : T?) {
    var tab <- { "one" => 1, "two" => 2 }
    var failed = false
    try {
        get(tab, "one") $(_lv) {
            tab |> insert("three", 3)
        }
    } recover {
        failed = true
    }
    t |> success(failed)
    t |> success(!key_exists(tab, "three"))
    failed = false
    try {
        get(tab, "three") $(_lv) {
            erase(tab, "three")
        }
    } recover {
        failed = true
    }
    t |> success(!failed)
}

[sideeffects]
def ta_test_insert_default(t : T?) {
    var td : table<string; int>
    td |> insert_default("one", 1)
    t |> equal(td?["one"] ?? 100500, 1)
    td |> insert_default("one", 2) // no effect
    t |> equal(td?["one"] ?? 100500, 1)
    t |> success(!key_exists(td, "zero"))
    td |> insert_default("zero")
    t |> equal(td?["zero"] ?? 100500, 0)

    var tmap : table<int; array<int>>
    tmap |> emplace(1, [1])
    tmap |> emplace(2, [2, 2])
    tmap |> emplace_default(1) // no effect
    tmap |> emplace_default(2) // no effect
    tmap |> emplace_default(0)
    tmap |> emplace_default(3)
    verify(length(tmap) == 4)
    for (i in range(0, 4)) {
        let found = get(tmap, i) $(val) {
            if (i == 0 || i == 3) {
                assert(empty(val))
            } elif (i == 1) {
                assert(length(val) == 1 && val[0] == 1)
            } elif (i == 2) {
                assert(length(val) == 2 && val[0] == 2 && val[1] == 2)
            }
        }
        verify(found)
    }
    t |> success(true)
}

[sideeffects]
def ta_test_get_with_default(t : T?) {
    var tab <- { 1 => "1", 2 => "2", 3 => "3" }
    get_with_default(tab, 2) $(val) {
        verify(val == "2")
    }
    get_with_default(tab, 1) $(val) {
        verify(val == "1")
        val += " modified"
    }
    t |> equal(tab?[1] ?? "", "1 modified")
    // add new entry with default value
    get_with_default(tab, 4) $(val) {
        verify(val == "")
        val += "four"
    }
    t |> equal(tab?[4] ?? "", "four")
    modify(tab, 2) <| $(val) => val + " modified"
    t |> equal(tab?[2] ?? "", "2 modified")
    modify(tab, 4) <| $(val) => "'{val}'"
    t |> equal(tab?[4] ?? "", "'four'")
    // modify non-existing entry, add it with default value
    modify(tab, 5) <| $(val) => "{val}five"
    t |> equal(tab?[5] ?? "", "five")
}

[test(description = "table find, get, insert, erase, key_exists, insert_default, emplace_default and get_with_default behave, an erase survives a hash collision, and modifying a locked table panics")]
def test_table_operations(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("table find and get") @(t : T?) {
        ta_test_find(t)
    }
    t |> run("table insert delete verify") @(t : T?) {
        var tab : table<string; int>
        let total : int = 32
        verify(!erase(tab, "0"))
        var i = 0
        while (i != total) {
            tab |> insert(string(i), i)
            i++
        }
        t |> equal(length(tab), total)
        i = 0
        while (i != total) {
            verify(tab |> get_value(string(i)) == i)
            i++
        }
        t |> equal(length(tab), total)
        var del = 0
        i = 0
        while (i < total) {
            verify(erase(tab, string(i)))
            verify(!erase(tab, string(i)))
            del ++
            i += 7
        }
        t |> equal(length(tab), total - del)
        i = 0
        while (i < total) {
            let found = get(tab, string(i)) $(pValue) {
                assert(pValue == i)
            }
            assert(!found == (i % 7 == 0))
            i ++
        }
        {
            var j = 0
            var k = 0
            i = 0
            while (i < total) {
                get(tab, string(i)) $(pVal) {
                    j += pVal
                }
                if (i % 7 != 0) {
                    k += i
                } else {
                    assert(i % 7 == 0)
                }
                i ++
            }
            t |> equal(j, k)
        }
        i = 0
        while (i < total) {
            tab |> insert(string(i), i)
            i += 7
        }
        t |> equal(length(tab), total)
        i = 0
        while (i != total) {
            verify(tab |> get_value(string(i)) == i)
            i++
        }
        t |> equal(length(tab), total)
        var cnt = 0
        for (ky, vl in keys(tab), values(tab)) {
            assert(ky == string(vl))
            cnt ++
        }
        t |> equal(cnt, total)
        verify(erase(tab, "1"))
        verify(!erase(tab, "-1"))
    }
    t |> run("table key_exists for various types") @(t : T?) {
        var tab_bool : table<bool; int>
        t |> success(!key_exists(tab_bool, false))
        t |> success(!key_exists(tab_bool, true))
        tab_bool |> insert(true, 0)
        t |> success(key_exists(tab_bool, true))
        tab_bool |> insert(false, -1)
        t |> success(key_exists(tab_bool, false))

        let tab_rng = {0 .. 5}
        t |> success(key_exists(tab_rng, 0 .. 5))

        var tab_int : table<int; int>
        t |> success(!key_exists(tab_int, -1))
        tab_int |> insert(-1, -1)
        t |> success(key_exists(tab_int, -1))
        erase(tab_int, -1)
        t |> success(!key_exists(tab_int, -1))

        var tab_string : table<string; int>
        t |> success(!key_exists(tab_string, "---"))
        tab_string |> insert("---", -1)
        t |> success(key_exists(tab_string, "---"))
        erase(tab_string, "---")
        t |> success(!key_exists(tab_string, "---"))
        t |> success(!key_exists(tab_string, " "))
        tab_string |> insert(" ", -1)
        t |> success(key_exists(tab_string, " "))
        erase(tab_string, " ")
        t |> success(!key_exists(tab_string, " "))
        tab_string |> insert("", -2)
        t |> success(key_exists(tab_string, ""))
        verify(tab_string |> get_value("") == -2)
        erase(tab_string, "")
        t |> success(!key_exists(tab_string, ""))
    }
    t |> run("table erase collision") @(t : T?) {
        ta_test_erase_collision(t)
    }
    t |> run("table lock panic") @(t : T?) {
        ta_test_lock_panic(t)
    }
    t |> run("table insert_default and emplace_default") @(t : T?) {
        ta_test_insert_default(t)
    }
    t |> run("table get_with_default and modify") @(t : T?) {
        ta_test_get_with_default(t)
    }
}

struct Pt {
    x : int
    y : int
}

[test(description = "each_kv visits every key-value pair of int, string and struct-valued tables, nothing for an empty one, agrees with zipped keys and values, and yields copies")]
def test_each_kv(t : T?) {
    t |> run("int keys, int values") @(t : T?) {
        var tab <- {for (i in range(10)); i => i * 10}
        var n = 0
        var ksum = 0
        var vsum = 0
        for (kv in each_kv(tab)) {
            n++
            ksum += kv.key
            vsum += kv.value
        }
        t |> equal(n, 10)
        t |> equal(ksum, 45)
        t |> equal(vsum, 450)
    }
    t |> run("string keys, float values") @(t : T?) {
        var tab <- {"a" => 1.5, "b" => 2.5}
        var s = 0.0
        for (kv in each_kv(tab)) {
            s += kv.value
        }
        t |> equal(s, 4.0)
    }
    t |> run("struct values") @(t : T?) {
        var tab <- {1 => Pt(x = 10, y = 20), 2 => Pt(x = 30, y = 40)}
        var xs = 0
        var ys = 0
        for (kv in each_kv(tab)) {
            xs += kv.value.x
            ys += kv.value.y
        }
        t |> equal(xs, 40)
        t |> equal(ys, 60)
    }
    t |> run("empty table yields nothing") @(t : T?) {
        let tab : table<int; int>
        var n = 0
        for (_kv in each_kv(tab)) {
            n++
        }
        t |> equal(n, 0)
    }
    t |> run("agrees with zipped keys/values") @(t : T?) {
        var tab <- {1 => "one", 2 => "two", 3 => "three"}
        let roundTrip <- {for (kv in each_kv(tab)); kv.key => kv.value}
        t |> equal(length(roundTrip), length(tab))
        for (k, v in keys(tab), values(tab)) {
            t |> success(key_exists(roundTrip, k))
            t |> equal(roundTrip?[k] ?? "", v)
        }
    }
    t |> run("element is a copy - table unchanged") @(t : T?) {
        var tab : table<int; Pt>
        tab |> insert(1, Pt(x = 10, y = 20))
        for (kv in each_kv(tab)) {
            var local = kv
            local.value.x = 999
        }
        t |> equal((tab?[1] ?? Pt()).x, 10)
    }
}

[test(description = "get_key on a value of an int-keyed table returns the key that value is stored under")]
def test_get_key_int_float(t : T?) {
    t |> run("basic int->float table") @(t : T?) {
        var tab <- {10 => 1.0, 20 => 2.0, 30 => 3.0}
        var sum_keys = 0
        var sum_vals = 0.0
        for (v in values(tab)) {
            let k = get_key(tab, v)
            sum_keys += k
            sum_vals += v
        }
        t |> equal(sum_keys, 60)
        t |> equal(sum_vals, 6.0)
    }
}

[test(description = "get_key on a value of a string-keyed table returns that value's string key")]
def test_get_key_string_int(t : T?) {
    t |> run("string->int table") @(t : T?) {
        var tab <- {"alpha" => 1, "beta" => 2}
        var found : table<string; int>
        for (v in values(tab)) {
            let k = get_key(tab, v)
            found[k] = v
        }
        t |> equal(found["alpha"], 1)
        t |> equal(found["beta"], 2)
    }
}

[test(description = "get_key works on a table passed as a const parameter")]
def test_get_key_const_table(t : T?) {
    t |> run("const table parameter") @(t : T?) {
        var tab : table<int; int>
        tab[100] = 42
        check_const(t, tab)
    }
}

def check_const(t : T?; tab : table<int; int>) {
    for (v in values(tab)) {
        let k = get_key(tab, v)
        t |> equal(k, 100)
        t |> equal(v, 42)
    }
}

[test(description = "get_key returns the right key for every surviving value after erases leave tombstones")]
def test_get_key_after_erase(t : T?) {
    t |> run("tombstones after erase") @(t : T?) {
        var tab <- {for (i in range(10)); i => i * 10}
        tab |> erase(3)
        tab |> erase(5)
        tab |> erase(7)
        var count = 0
        for (v in values(tab)) {
            let k = get_key(tab, v)
            t |> equal(v, k * 10)
            count++
        }
        t |> equal(count, 7)
    }
}

[test(description = "iterating an empty table's values with get_key runs zero times")]
def test_get_key_empty_table(t : T?) {
    t |> run("empty table iterates zero times") @(t : T?) {
        var tab : table<int; int>
        var count = 0
        for (v in values(tab)) {
            let _ = get_key(tab, v)
            count++
        }
        t |> equal(count, 0)
    }
}

[test(description = "get_key on a one-entry table returns its only key")]
def test_get_key_single_entry(t : T?) {
    t |> run("single entry") @(t : T?) {
        var tab : table<int; string>
        tab[42] = "hello"
        for (v in values(tab)) {
            let k = get_key(tab, v)
            t |> equal(k, 42)
            t |> equal(v, "hello")
        }
    }
}

// vector-, range- and handled-keyed tables past their first grow (8
// slots): a second hash definition for a key type shows up exactly
// here - the rehash re-buckets with it and lookups miss. A builtin
// key hashes as itself, a handled key as its workhorse; Point3 (12
// bytes, workhorse vec4f) is the handled type whose raw bytes and
// workhorse differ
let N = 40

[test(description = "a table keyed by int2 keeps every key findable, erasable and countable across its grows")]
def test_int2_keys(t : T?) {
    var tab : table<int2; string>   // nolint:STYLE027 - one insert at a time is the point: the table must grow and rehash
    for (i in range(N)) {
        tab[int2(i, i * 7)] = "v{i}"
    }
    t |> equal(N, length(tab))
    for (i in range(N)) {
        t |> equal("v{i}", tab?[int2(i, i * 7)] ?? "", "int2 key {i} after the grows")
        t |> success(key_exists(tab, int2(i, i * 7)), "key_exists on int2 key {i}")
    }
    for (h in range(N / 2)) {
        let i = h * 2
        t |> success(tab |> erase(int2(i, i * 7)), "erase of int2 key {i}")
    }
    t |> equal(N / 2, length(tab))
    for (h in range(N / 2)) {
        let i = h * 2 + 1
        t |> equal("v{i}", tab?[int2(i, i * 7)] ?? "", "odd int2 key {i} survives the erases")
    }
}

[test(description = "a table keyed by float3 finds every key across its grows")]
def test_float3_keys(t : T?) {
    var tab : table<float3; int>   // nolint:STYLE027 - one insert at a time is the point: the table must grow and rehash
    for (i in range(N)) {
        tab[float3(float(i), 0.5, -1.0)] = i
    }
    t |> equal(N, length(tab))
    for (i in range(N)) {
        t |> equal(i, tab?[float3(float(i), 0.5, -1.0)] ?? -1, "float3 key {i} after the grows")
    }
}

[test(description = "tables keyed by uint2 and by range find every key across their grows")]
def test_uint2_and_range_keys(t : T?) {
    var tab2 : table<uint2; int>
    var tabr : table<range; int>
    for (i in range(N)) {
        tab2[uint2(uint(i), 3u)] = i
        tabr[range(i, i + 10)] = i
    }
    t |> equal(N, length(tab2))
    t |> equal(N, length(tabr))
    for (i in range(N)) {
        t |> equal(i, tab2?[uint2(uint(i), 3u)] ?? -1, "uint2 key {i} after the grows")
        t |> equal(i, tabr?[range(i, i + 10)] ?? -1, "range key {i} after the grows")
    }
}

[test(description = "tables keyed by handled value types (EntityId, Point3) find every key across their grows")]
def test_handled_keys(t : T?) {
    var ids : table<EntityId; int>
    var pts : table<Point3; int>
    for (i in range(N)) {
        ids[EntityId(i + 100)] = i
        pts[Point3(float(i), 2.0, 3.0)] = i
    }
    t |> equal(N, length(ids))
    t |> equal(N, length(pts))
    for (i in range(N)) {
        t |> equal(i, ids?[EntityId(i + 100)] ?? -1, "EntityId key {i} after the grows")
        t |> equal(i, pts?[Point3(float(i), 2.0, 3.0)] ?? -1, "Point3 key {i} after the grows")
    }
}

[test(description = "a table keyed by int4 finds every key across its grows")]
def test_int4_keys(t : T?) {
    var tab : table<int4; int>   // nolint:STYLE027 - one insert at a time is the point: the table must grow and rehash
    for (i in range(N)) {
        tab[int4(i, 1, 2, 3)] = i
    }
    t |> equal(N, length(tab))
    for (i in range(N)) {
        t |> equal(i, tab?[int4(i, 1, 2, 3)] ?? -1, "int4 key {i} after the grows")
    }
}

// an integer key's bytes are not a float: int2 components in
// [-8388607, -1] spell NaNs, which a float compare never matches, and
// small-magnitude bytes spell denormals an FTZ/DAZ host reads as zero
[test(description = "integer vector keys compare by bits, not as floats: NaN-patterned keys stay findable and erasable, and the +0.0 and -0.0 patterns stay two keys")]
def test_key_bit_patterns(t : T?) {
    var neg : table<int2; int>   // nolint:STYLE027 - one insert at a time is the point: the table must grow and rehash
    for (i in range(N)) {
        neg[int2(-i - 1, -i * 7 - 1)] = i
    }
    t |> equal(N, length(neg))
    for (i in range(N)) {
        t |> equal(i, neg?[int2(-i - 1, -i * 7 - 1)] ?? -1, "negative int2 key {i} after the grows")
        t |> success(key_exists(neg, int2(-i - 1, -i * 7 - 1)),
                     "key_exists on negative int2 key {i}")
    }
    for (i in range(N)) {
        t |> success(neg |> erase(int2(-i - 1, -i * 7 - 1)), "erase of negative int2 key {i}")
    }
    t |> equal(0, length(neg))

    var r : table<range; int>   // nolint:STYLE027 - one insert at a time is the point: the table must grow and rehash
    for (i in range(N)) {
        r[range(-i - 1, i + 1)] = i
    }
    t |> equal(N, length(r))
    for (i in range(N)) {
        t |> equal(i, r?[range(-i - 1, i + 1)] ?? -1, "negative range key {i} after the grows")
    }

    var u : table<uint4; int>   // nolint:STYLE027 - one insert at a time is the point: the table must grow and rehash
    for (i in range(N)) {
        u[uint4(0xFFFFFF00u + uint(i), 1u, 2u, 3u)] = i
    }
    t |> equal(N, length(u))
    for (i in range(N)) {
        t |> equal(i, u?[uint4(0xFFFFFF00u + uint(i), 1u, 2u, 3u)] ?? -1, "NaN-patterned uint4 key {i}")
    }

    // 0 and the sign bit alone spell +0.0 and -0.0, which a float compare
    // ties; they stay two keys
    var z : table<int2; int>   // nolint:STYLE031 - two statements, because the pair is the point
    z[int2(0, 0)] = 1
    z[int2(1 << 31, 0)] = 2
    t |> equal(2, length(z))
    t |> equal(1, z?[int2(0, 0)] ?? -1, "the +0.0-patterned key")
    t |> equal(2, z?[int2(1 << 31, 0)] ?? -1, "the -0.0-patterned key")
}

[test(description = "iterating the keys of int4- and float4-keyed tables with bool values yields every key intact")]
def test_vector_keys_iterate_past_bool_values(t : T?) {
    t |> run("int4 keys, bool values") @(t : T?) {
        var tab := {int4(1, 2, 3, 4) => true, int4(5, 6, 7, 8) => false}
        var sum = int4(0)
        for (k in keys(tab)) {
            sum += k
        }
        t |> equal(sum, int4(6, 8, 10, 12))
    }
    t |> run("float4 keys, bool values") @(t : T?) {
        var tab := {float4(1.0) => true}
        var n = 0
        for (k in keys(tab)) {
            t |> equal(k, float4(1.0))
            n++
        }
        t |> equal(n, 1)
    }
}

[test(description = "a table and a set keyed by a handled value type (EntityId) insert, erase, get, key_exists, index and iterate by value")]
def test_value_table_key(t : T?) {
    var tab : table<EntityId; string>  // nolint:STYLE031 - insert with value keys is under test
    tab |> insert(EntityId(1), "hello")
    tab |> insert(EntityId(2), "world")
    tab |> insert(EntityId(3), "!")
    t |> equal(length(tab), 3)
    tab |> erase(EntityId(3))
    t |> equal(length(tab), 2)

    var found = tab |> get(EntityId(2)) $(val) {
        t |> equal(val, "world")
    }
    t |> success(found)

    found = tab |> get(EntityId(1)) $(val) {
        t |> equal(val, "hello")
    }
    t |> success(found)

    t |> success(tab |> key_exists(EntityId(1)))
    t |> success(!(tab |> key_exists(EntityId(3))))

    let val = tab ?[EntityId(1)] ?? "goodbye"
    t |> equal(val, "hello")
    let ptab = unsafe(addr(tab))
    let q = ptab ?[EntityId(2)] ?? "goodbye"
    t |> equal(q, "world")

    var count = 0
    for (k, v in keys(tab), values(tab)) {
        if (int(k) == 1) {
            t |> equal(v, "hello")
        }
        if (int(k) == 2) {
            t |> equal(v, "world")
        }
        count ++
    }
    t |> equal(count, 2)

    var set : table<EntityId>  // nolint:STYLE031 - set insert with value keys is under test
    set |> insert(EntityId(1))
    set |> insert(EntityId(2))
    set |> insert(EntityId(3))
    t |> equal(length(set), 3)
    t |> success(set |> key_exists(EntityId(3)))
    t |> success(!(set |> key_exists(EntityId(4))))
}

// a value type whose workhorse is vec4f: das compares the workhorse,
// so the C++ type needs no operator ==
[test(description = "a table keyed by a vec4f-workhorse value type inserts, finds and erases keys by their workhorse")]
def test_wide_value_table_key(t : T?) {
    let a = unsafe(reinterpret<BigEntityId>(int4(1, 2, 3, 4)))
    let b = unsafe(reinterpret<BigEntityId>(int4(9, 8, 7, 6)))
    var tab : table<BigEntityId; string>  // nolint:STYLE031 - insert with value keys is under test
    tab |> insert(a, "alpha")
    tab |> insert(b, "beta")
    t |> equal(length(tab), 2)
    t |> success(tab |> key_exists(a))
    t |> success(tab |> key_exists(b))
    t |> success(!(tab |> key_exists(BigEntityId())))
    let va = tab ?[a] ?? "none"
    t |> equal(va, "alpha")
    let vb = tab ?[b] ?? "none"
    t |> equal(vb, "beta")
    tab |> erase(a)
    t |> equal(length(tab), 1)
    t |> success(!(tab |> key_exists(a)))
}

var g_tab : table<EntityId; int>
var g_vec : table<int2; int>
var g_pts : table<Point3; int>

let TCT_N = 40

// one table reached by both tiers in one program: [no_aot] keeps the
// write interpreted inside the AOT binary, while the read is AOT; the
// index is written here directly, not through a daslib generic, so
// the hashing happens in this function. A value type is hashed by its
// workhorse in every tier; a builtin vector key the other direction
// (the interpreter hashes the int2 itself)
[no_aot, no_jit]
def write_interpreted {
    g_tab[EntityId(7)] = 42
    for (i in range(TCT_N)) {
        g_vec[int2(i, i * 7)] = i
        g_pts[Point3(float(i), 2.0, 3.0)] = i
    }
}

def read_aot : int => g_tab ?[EntityId(7)] ?? -1

def read_aot_misses : int {
    var misses = 0
    for (i in range(TCT_N)) {
        misses += (g_vec ?[int2(i, i * 7)] ?? -1) != i ? 1 : 0
        misses += (g_pts ?[Point3(float(i), 2.0, 3.0)] ?? -1) != i ? 1 : 0
    }
    return misses
}

[test(description = "keys an interpreted function writes into a table are found by an AOT function reading it, for value-type, int2 and Point3 keys")]
def test_cross_tier_table_hash(t : T?) {
    write_interpreted()
    t |> equal(length(g_tab), 1)
    t |> equal(read_aot(), 42)
    t |> equal(length(g_vec), TCT_N)
    t |> equal(length(g_pts), TCT_N)
    t |> equal(read_aot_misses(), 0,
               "every int2 and Point3 key the interpreter wrote is found from AOT")
}

// tables and sets are unordered; contents are compared sorted
def sorted_keys(tab : table<int>) : array<int> {
    var got <- [for (k in keys(tab)); k]
    sort(got)
    return <- got
}

def check(t : T?; tab : table<int>; expected : array<int>) {
    var got <- sorted_keys(tab)
    t |> equal(length(got), length(expected))
    if (length(got) == length(expected)) {
        for (a, b in got, expected) {
            t |> equal(a, b)
        }
    }
    delete got
}

[test(description = "a set table inserts, erases, clones, and builds from a literal or to_table with exactly the expected members")]
def test_set_table(t : T?) {
    t |> run("insert and iterate") @(t : T?) {
        var tab : table<int>
        t |> equal(length(tab), 0)
        for (i in 3..6) {
            tab |> insert(i)
        }
        check(t, tab, [3, 4, 5])
    }
    t |> run("erase and iterate") @(t : T?) {
        var tab : table<int>
        for (i in 3..6) {
            tab |> insert(i)
        }
        tab |> erase(4)
        check(t, tab, [3, 5])
    }
    t |> run("clone set") @(t : T?) {
        var tab : table<int>
        for (i in 3..6) {
            tab |> insert(i)
        }
        tab |> erase(4)
        let tc := tab
        check(t, tc, [3, 5])
        t |> success(tc |> key_exists(3))
        t |> success(!tc |> key_exists(4))
    }
    t |> run("literal and to_table") @(t : T?) {
        let ttt <- {1, 2, 3, 4}
        check(t, ttt, [1, 2, 3, 4])
        let qqq <- to_table(fixed_array<int>(1, 2, 3, 4))
        check(t, qqq, [1, 2, 3, 4])
    }
}

[test(description = "to_table over a fixed array of key-value tuples maps every key to its value")]
def test_to_table(t : T?) {
    t |> run("from fixed_array of tuples") @(t : T?) {
        let a = fixed_array((1, "one"), (2, "two"), (3, "three"))
        let b <- to_table(fixed_array((1, "one"), (2, "two"), (3, "three")))
        for (tup in a) {
            t |> equal(b?[tup._0] ?? "", tup._1)
        }
    }
}

def not_empty_hash(t : T?, val) {
    if (hash(val) == 0x1234567890abcdeful) {
        t.failure("Hash of value is empty:")
        debug(val)
    }
}
def empty_hash(t : T?, val) {
    if (hash(val) != 0x1234567890abcdeful) {
        t.failure("Hash of value is not empty:")
        debug(val)
    }
}

struct TVoid {
    a : void?
}

[test(description = "a void pointer hashes like a struct holding it, and not to the empty hash")]
def test_hash_void(t : T?) {
    var a : void?

    t.equal(hash(a), hash(TVoid()))
    t.not_empty_hash(a)
    t.not_empty_hash(TVoid())
}

struct TInt8 {
    a : int8
}

[test(description = "an int8 hashes like a struct holding it, and never to the empty hash, on fuzzed values")]
def test_hash_int8(t : T?) {
    var fake <- Faker()
    fuzz() $ {
        let a = fake |> random_int8()
        t.equal(hash(a), hash(TInt8(a = a)))
        t.not_empty_hash(a)
        t.not_empty_hash(TInt8(a = a))
    }
}

struct TUint8 {
    a : uint8
}

[test(description = "a uint8 hashes like a struct holding it, and never to the empty hash, on fuzzed values")]
def test_hash_uint8(t : T?) {
    var fake <- Faker()
    fuzz() $ {
        let a = fake |> random_uint8()
        t.equal(hash(a), hash(TUint8(a = a)))
        t.not_empty_hash(a)
        t.not_empty_hash(TUint8(a = a))
    }
}

struct TInt16 {
    a : int16
}

struct TInt8_8 {
    a : int8
    b : int8
}

[test(description = "an int16 hashes like a struct holding it, and never to the empty hash, on fuzzed values")]
def test_hash_int16(t : T?) {
    var fake <- Faker()
    fuzz() $ {
        let a = fake |> random_int16()
        t.equal(hash(a), hash(TInt16(a = a)))
        t.not_empty_hash(a)
        t.not_empty_hash(TInt16(a = a))
    }
}

struct TUint16 {
    a : uint16
}

[test(description = "a uint16 hashes like a struct holding it, and never to the empty hash, on fuzzed values")]
def test_hash_uint16(t : T?) {
    var fake <- Faker()
    fuzz() $ {
        let a = fake |> random_uint16()
        t.equal(hash(a), hash(TUint16(a = a)))
        t.not_empty_hash(a)
        t.not_empty_hash(TUint16(a = a))
    }
}

struct TFunction {
    a : function<void>
}

[export] def nada() {} [test(description = "a null function pointer hashes to the empty hash and a real one does not, alone or inside a struct")]
def test_hash_function(t : T?) {
    let a : function<void>
    t.equal(hash(a), hash(TFunction(a = a)))
    t.empty_hash(a)
    t.empty_hash(TFunction(a = a))

    let b = @@nada
    t.equal(hash(b), hash(TFunction(a = b)))
    t.not_empty_hash(b)
    t.not_empty_hash(TFunction(a = b))
}

enum TEnumValue {
    A, B, C
}

struct TEnum {
    a : TEnumValue
}

[test(description = "an enum value hashes like a struct holding it, and never to the empty hash, on fuzzed values")]
def test_hash_enum(t : T?) {
    var fake <- Faker()
    fuzz() $ {
        let a = fake |> any_enum(TEnumValue.A)
        t.equal(hash(a), hash(TEnum(a = a)))
        t.not_empty_hash(a)
        t.not_empty_hash(TEnum(a = a))
    }
}

struct LookupMember {
    linked : bool
    root : string
}

struct LookupCtx {
    members : table<string; LookupMember>
}

def lookup_root_of(var ctx : LookupCtx; k : string) : string {
    if (ctx.members[k].linked) {
        return ctx.members[k].root
    }
    return k
}

def lookup_link(var ctx : LookupCtx; dst, src : string) {
    let root = lookup_root_of(ctx, src)
    ctx.members[dst].root = root
}

[test(description = "a ternary's condition and its arms may each look up one table, written out or produced by an inlined call")]
def test_ternary_table_lookups(t : T?) {
    var ctx : LookupCtx
    ctx.members["a"] = LookupMember(linked = true, root = "r")
    lookup_link(ctx, "b", "a")
    t |> equal(ctx.members["b"].root, "r", "the inlined callee's two lookups")
    let k = "a"
    let direct = ctx.members[k].linked ? ctx.members[k].root : k
    t |> equal(direct, "r", "condition and arm")
    let either = k == "a" ? ctx.members["a"].root : ctx.members["b"].root
    t |> equal(either, "r", "both arms")
}

def make_name_set(n : int) : table<string> {
    return <- {for (i in range(n)); "name{i}"}
}

def make_squares(n : int) : table<int; int> {
    return <- {for (i in range(n)); i => i * i}
}

def make_const_squares(n : int) : table<int; int> const {
    return <- {for (i in range(n)); i => i * i}
}

def same_table(var tab : table<int; int>) : table<int; int>& {
    unsafe {
        return tab
    }
}

[test(description = "a table made in the loop header - by a call, by a literal - stays alive for the whole loop over its keys, values or pairs while the body declares locals of its own")]
def test_loop_over_a_temporary_table(t : T?) {
    t |> run("keys of a returned table, the body allocates") @(t : T?) {
        var total = 0
        for (_ in range(20)) {
            for (k in keys(make_name_set(50))) {
                var inscope other : table<string>
                other |> insert(k + "x")
                total += length(k)
            }
        }
        t |> equal(total, 5800)
    }
    t |> run("values of a returned table, the body holds a local array") @(t : T?) {
        var total = 0
        for (v in values(make_squares(10))) {
            var pad : int[16]
            pad[0] = v
            total += pad[0]
        }
        t |> equal(total, 285)
    }
    t |> run("pairs of a returned table") @(t : T?) {
        var total = 0
        for (kv in each_kv(make_squares(10))) {
            var pad : int[16]
            pad[0] = kv.key
            total += kv.value - pad[0]
        }
        t |> equal(total, 240)
    }
    t |> run("two sources, each over its own returned table") @(t : T?) {
        var total = 0
        for (k, v in keys(make_squares(10)), values(make_squares(10))) {
            var pad : int[16]
            pad[0] = k
            total += v - pad[0]
        }
        t |> equal(total, 240)
    }
    t |> run("keys of a table literal") @(t : T?) {
        var total = 0
        for (k in keys({ "a" => 1, "bb" => 2, "ccc" => 3 })) {
            var pad : int[16]
            pad[0] = length(k)
            total += pad[0]
        }
        t |> equal(total, 6)
    }
}

[test(description = "a loop over a table the header receives by reference, as a const value or inside a generator sees every entry, and the table's owner keeps its contents")]
def test_loop_over_a_table_the_header_does_not_own(t : T?) {
    t |> run("a table returned by reference is not moved out of its owner") @(t : T?) {
        var held <- make_squares(10)
        var total = 0
        for (v in values(same_table(held))) {
            total += v
        }
        t |> equal(total, 285)
        t |> equal(length(held), 10)
    }
    t |> run("a const table returned by value is still a loop source") @(t : T?) {
        var count = 0
        for (_ in values(make_const_squares(10))) {
            count++
        }
        t |> equal(count, 10)
    }
    t |> run("inside a generator, in a loop that does not yield") @(t : T?) {
        var gen <- generator<int>() <| $() { // nolint:STYLE002 only the piped form parses here
            var inner = 0
            for (v in values(make_squares(4))) {
                var pad : int[16]
                pad[0] = v
                inner += pad[0]
            }
            yield inner
            return false
        }
        var total = 0
        for (v in gen) {
            total += v
        }
        t |> equal(total, 14)
    }
}
