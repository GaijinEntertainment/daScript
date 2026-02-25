options gen2
options persistent_heap

// Benchmark: overwrite the same key repeatedly.
// Inserts key=3 with value=3 into the hash map 300K times per iteration.
// Measures single-key hot-path write performance (first op is insert, rest are overwrites).
// Ported from examples/profile/hash_test/test05.das.

require dastest/testing_boost

require daslib/flat_hash_table
require daslib/cuckoo_hash_table

typedef CuckooHashMap_test = $TCuckooHashTable < int; int >
typedef FlatHashMap_test = $TFlatHashTable < int; int >

let OPS = 300000

[sideeffects]
def overwrite_same_key(hmap : auto(HashMapType); count : int) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (j in range(count)) {
        unsafe(hashMap[3]) = 3
    }
}

def run_bench(b : B?; hmap : auto(HashMapType)) {
    b |> run("overwrite_same_key/{OPS}", OPS) <| $() {
        overwrite_same_key(hmap, OPS)
    }
}

[benchmark]
def builtin_table(b : B?) {
    run_bench(b, default<table<int; int>>)
}

[benchmark]
def cuckoo_hashmap(b : B?) {
    run_bench(b, default<CuckooHashMap_test>)
}

[benchmark]
def flat_hashmap(b : B?) {
    run_bench(b, default<FlatHashMap_test>)
}
