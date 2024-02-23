options persistent_heap

require framework
require daslib/hash_map

%spoof_instance~TBucketHashMap(BucketHashMap_test,int,int)%%;
%spoof_instance~TFlatHashMap(FlatHashMap_test,int,int)%%;
%spoof_instance~TInterleavedFlatHashMap(InterleavedFlatHashMap_test,int,int)%%;

[sideeffects]
def test ( hmap : auto(HashMapType); dummy )
    var inscope hashMap : HashMapType
    static_if !typeinfo(is_table type<HashMapType>)
        hashMap <- HashMapType()
    for i in range(25)
        for j in range(300000)
            hashMap[3] = 3

[export]
def main
    print("test05 - insert 300000 elements of the same value 25 times\n")
    profile_test("table<int;int>", type<table<int;int>>,0)
    profile_test("TBucketHashMap<int,int>", type<BucketHashMap_test>,0)
    profile_test("TFlatHashMap<int,int>", type<FlatHashMap_test>,0)
    profile_test("TInterleavedFlatHashMap<int,int>", type<InterleavedFlatHashMap_test>,0)



