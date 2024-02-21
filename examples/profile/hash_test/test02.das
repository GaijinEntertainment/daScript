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
    for i in range(25)
        hashMap |> clear()
        for j in range(600000)
            hashMap[j] = -j

// note - this speeds up many of the tests, but it's not a fair comparison
//def hash ( t : int )
//    return t>=2 ? uint64(t) : 1099511628211ul

[export]
def main
    print("test02 - insert 600000 elements in a hash map 25 times, with clear inbetween\n")
    profile_test("table<int;int>", type<table<int;int>>, 0)
    profile_test("TBucketHashMap<int,int>", type<BucketHashMap_test>, 0)
    profile_test("TFlatHashMap<int,int>", type<FlatHashMap_test>, 0)



