options gen2

require dastest/testing_boost public
require UnitTest

// Vector-, range- and handled-keyed tables past their first grow (8 slots): every key inserted
// stays findable, erasable and countable. A second hash definition for a key type shows up
// exactly here - the rehash a grow performs re-buckets with it, and the lookups then miss. A
// builtin key hashes as itself, a handled key as its workhorse; Point3 (12 bytes, workhorse
// vec4f) is the handled type whose raw bytes and workhorse differ.

let N = 40

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
