require dastest/testing_boost public

require daslib/hash_map public

struct Foo
    a : int

def table_ops ( t : T?; var a:auto(AnyTable_int_to_Foo); var e:Foo; var et:Foo implicit )
    for i in range(10)
        a[i] = [[Foo a=i]]
    t |> equal(10, length(a))
    clear(a)
    t |> equal(0, length(a))
    t |> success(empty(a))
    a[0] = e
    a[1] = e
    t |> equal(1, a[0].a)
    t |> equal(1, a[1].a)
    t |> equal(2, length(a) )
    clear(a)
    a[0] = e
    a[1] = e
    var b := a
    a := b
    t |> equal(1, b[0].a)
    t |> equal(1, b[1].a)
    t |> equal(2, length(b) )
    erase(a, 0)
    erase(a, 1)
    t |> equal(0, length(a))
    a := b
    t |> equal(1, a[0].a)
    t |> equal(1, a[1].a)
    t |> equal(2, length(a) )
    for k in keys(a)
        t |> equal(1, a[k].a)
    for k in keys(a)
        t |> success ( k==0 || k==1 )
    for v in values(a)
        t |> equal(1, v.a)
    var found = a |> get(0) <| $ ( v )
        t |> equal(1, v.a)
    t |> success ( found )
    found = a |> get(1) <| $ ( v )
        t |> equal(1, v.a)
    t |> success ( found )
    found = a |> get(2) <| $ ( v )
        t |> failure("we should not be here\n")
    t |> success ( !found )

%spoof_instance~TBucketHashMap(BucketHashMap_int_Foo,int,Foo)%%;

[test]
def all_ops ( t : T? )
    t |> run("BucketHashMap") <| @ ( t : T? )
        var tab <- BucketHashMap_int_Foo()
        t |> success(empty(tab))
        t |> equal(0, length(tab))
        table_ops ( t, tab, [[Foo a=1]], [[Foo a=2]])

[test]
def all_init ( t : T? )
    t |> run("BucketHashMap") <| @ ( t : T? )
        var tab <- BucketHashMap_int_Foo([{auto
            1 => [[Foo a=1]];
            2 => [[Foo a=2]];
            3 => [[Foo a=3]]
        }])
        t |> equal(3, length(tab))
        t |> equal(1, tab[1].a)
        t |> equal(2, tab[2].a)
        t |> equal(3, tab[3].a)
        t |> equal(0, tab[4].a)
