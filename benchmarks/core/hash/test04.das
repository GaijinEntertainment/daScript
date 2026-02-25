options gen2
options persistent_heap
options unsafe_table_lookup = false

// Benchmark: insert 1M unique random numbers, then erase all of them.
// Tests erase performance after a full random-key insert.
// Ported from examples/profile/hash_test/test04.das.

require dastest/testing_boost

require _common
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

typedef CuckooHashMap_test = $TCuckooHashTable < int; int >
typedef FlatHashMap_test = $TFlatHashTable < int; int >

[sideeffects]
def fill_then_erase(hmap : auto(HashMapType); randomNumbers : array<int>) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (num in randomNumbers) {
        hashMap[num] = -num
    }
    for (num in randomNumbers) {
        hashMap.erase(num)
    }
}

def run_bench(b : B?; hmap : auto(HashMapType); randomNumbers : array<int>) {
    b |> run("insert_erase/{TOTAL_RANDOM_NUMBERS}", TOTAL_RANDOM_NUMBERS) <| $() {
        fill_then_erase(hmap, randomNumbers)
    }
}

[benchmark]
def builtin_table(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<table<int; int>>, randomNumbers)
}

[benchmark]
def cuckoo_hashmap(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<CuckooHashMap_test>, randomNumbers)
}

[benchmark]
def flat_hashmap(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<FlatHashMap_test>, randomNumbers)
}
