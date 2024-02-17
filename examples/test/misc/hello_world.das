require math

require daslib/spoof public

require daslib/hash_map public

struct Foo
    a : int

%spoof_instance~TBucketHashMap(BucketHashMap_int_Foo,int,Foo)%%;

[export]
def main
    var tab <- BucketHashMap_int_Foo([{auto
        1 => [[Foo a=1]];
        2 => [[Foo a=2]];
        3 => [[Foo a=3]]
    }])
    for k,v in keys(tab),values(tab)
        print("k: {k} => {v.a}\n")
    print("0=>{tab[0].a} 1=>{tab[1].a} 2=>{tab[2].a} 3=>{tab[3].a} 4=>{tab[4].a}\n")
