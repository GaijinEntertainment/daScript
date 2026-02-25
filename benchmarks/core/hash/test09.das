options gen2
options persistent_heap

// Benchmark: insert 1M unique random numbers, then look up and sum all values via get().
// Measures block-based lookup (get + callback) performance with value accumulation.
// Ported from examples/profile/hash_test/test09.das.

require dastest/testing_boost

require _common
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

typedef CuckooHashMap_test = $TCuckooHashTable < int; int >
typedef FlatHashMap_test = $TFlatHashTable < int; int >

struct Adder {
    val : uint64
}

def step(var adder : Adder; hashMap; search : int) {
    hashMap.get(search) <| $(val) {
        adder.val += uint64(val)
    }
}

[sideeffects]
def fill_map(hmap : auto(HashMapType); randomNumbers : array<int>) : auto(HashMapType) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (num in randomNumbers) {
        unsafe(hashMap[num]) = -num
    }
    return <- hashMap
}

[sideeffects]
def lookup_and_sum(hashMap : auto(HashMapType); randomNumbers : array<int>) {
    var adder : Adder
    for (num in randomNumbers) {
        adder |> step(hashMap, num)
    }
}

def run_bench(b : B?; hmap : auto(HashMapType); randomNumbers : array<int>) {
    let hashMap = fill_map(hmap, randomNumbers)
    b |> run("get_sum/{TOTAL_RANDOM_NUMBERS}", TOTAL_RANDOM_NUMBERS) <| $() {
        lookup_and_sum(hashMap, randomNumbers)
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
