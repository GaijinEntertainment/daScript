options gen2
options persistent_heap

require _framework
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

[sideeffects]
def test(hmap : auto(HashMapType); dummy) {
    var inscope hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (i in range(25)) {
        hashMap.clear()
        for (j in range(600000)) {
            unsafe(hashMap[j]) = -j
        }
    }
    for (i in range(600000)) {
        let v = hashMap[i]
        assert(v == -i)
    }
}

typedef CuckooHashMap_test = $CuckooHashTable < int; int >
typedef CuckooHashMap_test_inline = $CuckooHashTable < int; int > (@@hash0, @@hash_inline)
typedef FlatHashMap_test = $FlatHashTable < int; int >
typedef FlatHashMap_test_inline = $FlatHashTable < int; int > (@@hash_inline)
typedef FlatHashMap_test0 = $FlatHashTable < int; int > (@@hash0)

def hash_inline(i : int) : uint64 {
    let DAS_WYHASH_SEED = 0x1234567890abcdeful
    let u = uint64(i)
    let ab = mul128(u, DAS_WYHASH_SEED)
    let h = ab.x ^ ab.y
    return h <= 1ul ? 1099511628211ul : h
}

[export]
def main {
    print("test02 - insert 600000 elements in a hash map 25 times, with clear between\n")
    profile_test("table<int;int>", default<table<int; int>>, 0)
    profile_test("TCuckooHashMap<int,int> hash(x)->hash/hash0", default<CuckooHashMap_test>, 0)
    profile_test("TCuckooHashMap<int,int> hash(x)->x/hash intrinsic", default<CuckooHashMap_test>, 0)
    profile_test("TFlatHashMap<int,int>", default<FlatHashMap_test>, 0)
    profile_test("TFlatHashMap<int,int> with hash(x)->hash intrinsic", default<FlatHashMap_test_inline>, 0)
    profile_test("TFlatHashMap<int,int> with hash(x)->x", default<FlatHashMap_test0>, 0)
}
