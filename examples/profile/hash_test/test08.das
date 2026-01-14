options gen2
options persistent_heap

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
            unsafe(hashMap[num]) = -num
        }
        var any = false
        for (num in randomNumbers) {
            any ^^= hashMap.key_exists(num)      // we need `any` to prevent the compiler from optimizing the loop away
        }
        delete hashMap
    }
}

[export]
def main {
    print("test08 - insert {TOTAL_RANDOM_NUMBERS} unique random numbers, then find them\n")
    var randomNumbers <- generate_unique_positive_random_numbers()
    profile_test("table<int;int>", default<table<int; int>>, randomNumbers)
    profile_test("TCuckooHashMap<int,int>", default<CuckooHashMap_test>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", default<FlatHashMap_test>, randomNumbers)
}

