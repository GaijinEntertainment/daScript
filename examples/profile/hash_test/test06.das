options gen2
options persistent_heap

require _framework
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

struct Value {
    val : int
}

typedef CuckooHashMap_test = $TCuckooHashTable < int; Value >
typedef FlatHashMap_test = $TFlatHashTable < int; Value >

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
    var randomNumbers <- get_random_numbers_with_intersections_10()
    print("test06 - insert {TOTAL_RANDOM_NUMBERS} random numbers with 10% intersections, increment each element's field\n")
    profile_test("table<int;int>", default<table<int; Value>>, randomNumbers)
    profile_test("TCuckooHashMap<int,int>", default<CuckooHashMap_test>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", default<FlatHashMap_test>, randomNumbers)
}
