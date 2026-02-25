options gen2
options persistent_heap

// Benchmark: insert 1M random numbers with 50% key collisions, incrementing a struct field.
// Uses `get_random_numbers_with_intersections_50` to generate keys where 50% are duplicates.
// Same workload as test06, but with much higher collision rate.
// Ported from examples/profile/hash_test/test07.das.

require dastest/testing_boost

require _common
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

struct Value {
    val : int
}

typedef CuckooHashMap_test = $TCuckooHashTable < int; Value >
typedef FlatHashMap_test = $TFlatHashTable < int; Value >

[sideeffects]
def insert_and_increment(hmap : auto(HashMapType); randomNumbers : array<int>) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (num in randomNumbers) {
        unsafe(hashMap[num]).val ++
    }
}

def run_bench(b : B?; hmap : auto(HashMapType); randomNumbers : array<int>) {
    b |> run("insert_increment/{TOTAL_RANDOM_NUMBERS}", TOTAL_RANDOM_NUMBERS) <| $() {
        insert_and_increment(hmap, randomNumbers)
    }
}

[benchmark]
def builtin_table(b : B?) {
    let randomNumbers <- get_random_numbers_with_intersections_50()
    run_bench(b, default<table<int; Value>>, randomNumbers)
}

[benchmark]
def cuckoo_hashmap(b : B?) {
    let randomNumbers <- get_random_numbers_with_intersections_50()
    run_bench(b, default<CuckooHashMap_test>, randomNumbers)
}

[benchmark]
def flat_hashmap(b : B?) {
    let randomNumbers <- get_random_numbers_with_intersections_50()
    run_bench(b, default<FlatHashMap_test>, randomNumbers)
}
