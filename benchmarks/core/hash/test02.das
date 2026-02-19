options gen2
options persistent_heap

require dastest/testing_boost

require daslib/flat_hash_table
require daslib/cuckoo_hash_table

typedef CuckooHashMap_test = $TCuckooHashTable < int; int >
typedef CuckooHashMap_test_inline = $TCuckooHashTable < int; int > (@@hash0, @@hash_inline)
typedef FlatHashMap_test = $TFlatHashTable < int; int >
typedef FlatHashMap_test_inline = $TFlatHashTable < int; int > (@@hash_inline)
typedef FlatHashMap_test0 = $TFlatHashTable < int; int > (@@hash0)

[sideeffects]
def read_map(b : B?; hmap : auto(HashMapType)) {
    for (i in range(hmap.length())) {
        let v = hmap?[i] ?? 0
        if (v != -i) {
            b->failNow()
        }
    }
}

[sideeffects]
def fill_map(hmap : auto(HashMapType); size : int) : auto(HashMapType) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (j in range(size)) {
        unsafe(hashMap[j]) = -j
    }
    return <- hashMap
}

def run_write_bench(b : B?; hmap : auto(HashMapType)) {
    b |> run("insert/{HASH_SIZE}", HASH_SIZE) <| $() {
        fill_map(hmap, HASH_SIZE)
    }
}

def run_read_bench(b : B?; hmap : auto(HashMapType)) {
    let m = fill_map(hmap, HASH_SIZE)
    b |> run("read/{HASH_SIZE}", HASH_SIZE) <| $() {
        read_map(b, m)
    }
}

def hash_inline(i : int) : uint64 {
    let DAS_WYHASH_SEED = 0x1234567890abcdeful
    let u = uint64(i)
    let ab = mul128(u, DAS_WYHASH_SEED)
    let h = ab.x ^ ab.y
    return h <= 1ul ? 1099511628211ul : h
}

let HASH_SIZE = 600000

[benchmark]
def builtin_table(b : B?) {
    run_write_bench(b, default<table<int; int>>)
    run_read_bench(b, default<table<int; int>>)
}

[benchmark]
def cuckoo_hashmap(b : B?) {
    run_write_bench(b, default<CuckooHashMap_test>)
    run_read_bench(b, default<CuckooHashMap_test>)
}

[benchmark]
def cuckoo_hashmap_inlinehash(b : B?) {
    run_write_bench(b, default<CuckooHashMap_test_inline>)
    run_read_bench(b, default<CuckooHashMap_test_inline>)
}

[benchmark]
def flat_hashmap(b : B?) {
    run_write_bench(b, default<FlatHashMap_test>)
    run_read_bench(b, default<FlatHashMap_test>)
}

[benchmark]
def flat_hashmap_inlinehash(b : B?) {
    run_write_bench(b, default<FlatHashMap_test_inline>)
    run_read_bench(b, default<FlatHashMap_test_inline>)
}

[benchmark]
def flat_hashmap_hash0(b : B?) {
    run_write_bench(b, default<FlatHashMap_test0>)
    run_read_bench(b, default<FlatHashMap_test0>)
}
