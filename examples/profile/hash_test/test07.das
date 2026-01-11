options gen2
options persistent_heap

require _framework
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

struct Value {
    val : int
}

typedef CuckooHashMap_test = $CuckooHashTable < int; Value >
typedef FlatHashMap_test = $FlatHashTable < int; Value >

[sideeffects]
def test(hmap : auto(HashMapType); randomNumbers) {
    for (i in range(8)) {
        var hashMap : HashMapType
        static_if (!typeinfo is_table(type<HashMapType>)) {
            hashMap <- HashMapType()
        }
        for (num in randomNumbers) {
            unsafe(hashMap[num]).val ++
        }
        delete hashMap
    }
}

[export]
def main {
    var randomNumbers <- get_random_numbers_with_intersections_50()
    print("test07 - insert {TOTAL_RANDOM_NUMBERS} random numbers with 50% intersections, increment each element's field\n")
    profile_test("table<int;int>", default<table<int; Value>>, randomNumbers)
    profile_test("TCuckooHashMap<int,int>", default<CuckooHashMap_test>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", default<FlatHashMap_test>, randomNumbers)
}
