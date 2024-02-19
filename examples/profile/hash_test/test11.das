options persistent_heap

require framework
require daslib/hash_map
require daslib/spoof

//! BucketHashMap is a hash map that uses separate chaining with buckets.
let public TSlotMapAdapter = %spoof_template~(SlotMap,HashMapType)
[skip_field_lock_check]
struct %SlotMap
    id : uint64
    hashMap : %HashMapType

def %SlotMap
    var self : %SlotMap
    static_if !typeinfo(is_table type<%HashMapType>)
        self.hashMap <- %HashMapType()
    return <- self

def emplace ( var self : %SlotMap explicit; val : int )
    with self
        let r = id
        hashMap[id] = val
        id ++
        return r
%%

%spoof_instance~TBucketHashMap(BucketHashMap_uint64_int,uint64,int)%%;
%spoof_instance~TFlatHashMap(FlatHashMap_uint64_int,uint64,int)%%;

typedef SlotTable = table<uint64;int>

%spoof_instance~TSlotMapAdapter(SlotMapAdapter_Table,SlotTable)%%;
%spoof_instance~TSlotMapAdapter(SlotMapAdapter_BucketHashMap,BucketHashMap_uint64_int)%%;
%spoof_instance~TSlotMapAdapter(SlotMapAdapter_FlatHashMap,FlatHashMap_uint64_int)%%;

[sideeffects]
def test ( hmap : auto(SlotMapType); randomNumbers )
    for i in range(25)
        var slotMap <- SlotMapType()
        for num in randomNumbers
            slotMap |> emplace(num)
        delete slotMap

[export]
def main
    print("test11 - insert {TOTAL_RANDOM_NUMBERS} unique random numbers into slotmap\n")
    var randomNumbers <- generate_unique_positive_random_numbers()
    profile_test("table<int;int>", type<SlotMapAdapter_Table>, randomNumbers)
    profile_test("TBucketHashMap<int,int>", type<SlotMapAdapter_BucketHashMap>, randomNumbers)
    profile_test("TFlatHashMap<int,int>", type<SlotMapAdapter_FlatHashMap>, randomNumbers)

