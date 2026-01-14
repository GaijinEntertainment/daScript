options gen2
options persistent_heap

require _framework
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
def test(hmap : auto(HashMapType); randomNumbers) {
    var inscope hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (num in randomNumbers) {
        unsafe(hashMap[num]) = -num
    }
    for (i in range(10)) {
        var adder : Adder
        for (num in randomNumbers) {
            adder |> step(hashMap, num)
        }
    }
}

[export]
def main {
    print("test09 - insert {TOTAL_RANDOM_NUMBERS} unique random numbers, then find and summ them up\n")
    var randomNumbers <- generate_unique_positive_random_numbers()
    profile_test("table<int;int>", default<table<int; int>>, randomNumbers)
    profile_test("TCuckooHashMap<int,int>", default<CuckooHashMap_test>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", default<FlatHashMap_test>, randomNumbers)
}

