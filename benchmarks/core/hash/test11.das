options gen2
options persistent_heap

// Benchmark: insert 1M unique random numbers into a SlotMap adapter.
// SlotMap wraps a hash map and assigns monotonically increasing uint64 IDs as keys,
// simulating an entity/slot allocation pattern common in game engines.
// Tests builtin table, cuckoo, and flat hash map backends (uint64 keys).
// Ported from examples/profile/hash_test/test11.das.

require dastest/testing_boost

require _common
require daslib/flat_hash_table
require daslib/cuckoo_hash_table
require _slot_map

typedef CuckooHashMap_uint64_int = $TCuckooHashTable < uint64, int >
typedef FlatHashMap_uint64_int = $TFlatHashTable < uint64, int >
typedef FlatHashMap0_uint64_int = $TFlatHashTable < uint64, int > (@@hash0)

typedef SlotTable = table<uint64; int>

typedef SlotMapAdapter_Table = $SlotMap < SlotTable >
typedef SlotMapAdapter_CuckooHashMap = $SlotMap < CuckooHashMap_uint64_int >
typedef SlotMapAdapter_FlatHashMap = $SlotMap < FlatHashMap_uint64_int >
typedef SlotMapAdapter_FlatHashMap0 = $SlotMap < FlatHashMap0_uint64_int >

[sideeffects]
def fill_slotmap(smap : auto(SlotMapType); randomNumbers : array<int>) {
    var slotMap <- SlotMapType()
    for (num in randomNumbers) {
        slotMap.emplace(num)
    }
}

def run_bench(b : B?; smap : auto(SlotMapType); randomNumbers : array<int>) {
    b |> run("emplace/{TOTAL_RANDOM_NUMBERS}", TOTAL_RANDOM_NUMBERS) <| $() {
        fill_slotmap(smap, randomNumbers)
    }
}

[benchmark]
def builtin_table(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<SlotMapAdapter_Table>, randomNumbers)
}

[benchmark]
def cuckoo_hashmap(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<SlotMapAdapter_CuckooHashMap>, randomNumbers)
}

[benchmark]
def flat_hashmap(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<SlotMapAdapter_FlatHashMap>, randomNumbers)
}

[benchmark]
def flat_hashmap_hash0(b : B?) {
    let randomNumbers <- generate_unique_positive_random_numbers()
    run_bench(b, default<SlotMapAdapter_FlatHashMap0>, randomNumbers)
}
