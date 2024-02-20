options persistent_heap

require framework
require daslib/hash_map

%spoof_instance~TBucketHashMap(BucketHashMap_test,int,int)%%;
%spoof_instance~TFlatHashMap(FlatHashMap_test,int,int)%%;

[sideeffects]
def test ( hmap : auto(HashMapType); dummy )
    var inscope hashMap : HashMapType
    static_if !typeinfo(is_table type<HashMapType>)
        hashMap <- HashMapType()
    for i in range(50)
        for j in range(599999)
            hashMap[j] = -j

[export]
def main
    print("test02 - insert 599999 elements in a hash map 50 times\n")
    profile_test("table<int;int>", type<table<int;int>>, 0)
    profile_test("TBucketHashMap<int,int>", type<BucketHashMap_test>, 0)
    profile_test("TFlatHashMap<int,int>", type<FlatHashMap_test>, 0)



