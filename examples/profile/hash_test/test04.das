options gen2
options persistent_heap

// OLD das restrictions
options unsafe_table_lookup = false

require _framework
require daslib/flat_hash_table
require daslib/cuckoo_hash_table

typedef CuckooHashMap_test = $TCuckooHashTable < int; int >
typedef FlatHashMap_test = $TFlatHashTable < int; int >

[sideeffects]
def test(hmap : auto(HashMapType); randomNumbers) {
    for (i in range(10)) {
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
        delete hashMap
    }
}

[export]
def main {
    print("test04 - insert {TOTAL_RANDOM_NUMBERS} unique random numbers, then erase them in the order they were inserted\n")
    var randomNumbers <- generate_unique_positive_random_numbers()
    profile_test("table<int;int>", default<table<int; int>>, randomNumbers)
    profile_test("TCuckooHashMap<int,int>", default<CuckooHashMap_test>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", default<FlatHashMap_test>, randomNumbers)
}

