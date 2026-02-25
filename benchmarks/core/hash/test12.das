options gen2
options persistent_heap
options unsafe_table_lookup = false

// Benchmark: pathological key distribution — 8K sequential keys (0..8191) followed by
// random keys (100K total). Tests how hash maps handle a mix of clustered and random
// insertions. Also includes a deliberately bad hash function (pathalogical) that maps
// small keys to a constant, stressing the flat hash map's collision handling (~51x
// slower than normal flat_hashmap). Reduced from the original 1M/32K to keep runtime
// practical (~8s vs ~120s).
// Ported from examples/profile/hash_test/test12.das.

require dastest/testing_boost

require _common
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

typedef CuckooHashMap_test = $TCuckooHashTable < int; int >
typedef FlatHashMap_test = $TFlatHashTable < int; int >
typedef PathalogicalFlatHashMap_test = $TFlatHashTable < int; int > (@@pathalogical)

let PATHO_TOTAL = 100000
let PATHO_SEQUENTIAL = 8192

// Deliberately bad hash: maps values < 2 to a constant, causing collisions.
def pathalogical(t : int) {
    return t >= 2 ? uint64(t) : 1099511628211ul
}

// Generate pathological distribution: first 8K keys are sequential 0..8191,
// rest are unique random numbers. 100K total.
def generate_pathological_numbers() {
    var randomNumbers <- generate_unique_positive_random_numbers(PATHO_TOTAL)
    for (n in range(PATHO_SEQUENTIAL)) {
        randomNumbers[n] = n
    }
    return <- randomNumbers
}

[sideeffects]
def fill_clear_fill(hmap : auto(HashMapType); randomNumbers : array<int>) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (num in randomNumbers) {
        hashMap[num] = -num
    }
    hashMap.clear()
    for (num in randomNumbers) {
        hashMap[num] = -num
    }
}

def run_bench(b : B?; hmap : auto(HashMapType); randomNumbers : array<int>) {
    b |> run("insert_clear_insert/{PATHO_TOTAL}", PATHO_TOTAL) <| $() {
        fill_clear_fill(hmap, randomNumbers)
    }
}

[benchmark]
def builtin_table(b : B?) {
    let randomNumbers <- generate_pathological_numbers()
    run_bench(b, default<table<int; int>>, randomNumbers)
}

[benchmark]
def cuckoo_hashmap(b : B?) {
    let randomNumbers <- generate_pathological_numbers()
    run_bench(b, default<CuckooHashMap_test>, randomNumbers)
}

[benchmark]
def flat_hashmap(b : B?) {
    let randomNumbers <- generate_pathological_numbers()
    run_bench(b, default<FlatHashMap_test>, randomNumbers)
}

[benchmark]
def flat_hashmap_pathalogical(b : B?) {
    let randomNumbers <- generate_pathological_numbers()
    run_bench(b, default<PathalogicalFlatHashMap_test>, randomNumbers)
}
