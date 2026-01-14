options gen2
options persistent_heap

require _framework
require daslib/flat_hash_table
require daslib/cuckoo_hash_table
require slot_map

typedef CuckooHashMap_uint64_int = $CuckooHashTable < uint64, int >
typedef FlatHashMap_uint64_int = $FlatHashTable < uint64, int >
typedef FlatHashMap0_uint64_int = $FlatHashTable < uint64, int > (@@hash0)

typedef SlotTable = table<uint64; int>

typedef SlotMapAdapter_Table = $SlotMap < SlotTable >
typedef SlotMapAdapter_CuckooHashMap = $SlotMap < CuckooHashMap_uint64_int >
typedef SlotMapAdapter_FlatHashMap = $SlotMap < FlatHashMap_uint64_int >
typedef SlotMapAdapter_FlatHashMap0 = $SlotMap < FlatHashMap0_uint64_int >

[sideeffects]
def test(hmap : auto(SlotMapType); randomNumbers) {
    for (i in range(25)) {
        var slotMap <- SlotMapType()
        for (num in randomNumbers) {
            slotMap.emplace(num)
        }
        delete slotMap
    }
}

[export]
def main {
    print("test11 - insert {TOTAL_RANDOM_NUMBERS} unique random numbers into slotmap\n")
    var randomNumbers <- generate_unique_positive_random_numbers()
    profile_test("table<int;int>", default<SlotMapAdapter_Table>, randomNumbers)
    profile_test("TCuckooHashMap<int,int>", default<SlotMapAdapter_CuckooHashMap>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", default<SlotMapAdapter_FlatHashMap>, randomNumbers)
    profile_test("TFlatHashMap<int,int> with hash(x)->x", default<SlotMapAdapter_FlatHashMap0>, randomNumbers)
}

