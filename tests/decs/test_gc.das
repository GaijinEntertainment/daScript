options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public

[test]
def test_gc_arrays( t: T?)
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set("values", [{for x in range(i+1); x}])
            cmp |> set("i", i)
    commit()
    t |> run("before gc") <| @ ( t : T? )
        query <| $ ( values:array<int>; i:int )
            for x,v in range(i+1),values
                t |> equal( x, v)
    before_gc()
    unsafe
        heap_collect(true,true)
    after_gc()
    t |> run("after gc") <| @ ( t : T? )
        query <| $ ( values:array<int>; i:int )
            for x,v in range(i+1),values
                t |> equal( x, v)

[test]
def test_gc_strings( t: T?)
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp.txt := "hello_{i}"
    commit()
    t |> run("before string gc") <| @ ( t : T? )
        var i = 0
        query <| $ ( txt:string )
            t |> equal( txt, "hello_{i++}")
        t |> equal( i, 3)
    before_gc()
    unsafe
        string_heap_collect(true)
    after_gc()
    t |> run("after string gc") <| @ ( t : T? )
        var j = 0
        query <| $ ( txt:string )
            t |> equal( txt, "hello_{j++}")
        t |> equal( j, 3)

var total_foo = 0
class FooWithLifetime
    def FooWithLifetime
        total_foo ++
    def operator delete
        total_foo --

[test]
def test_arrays_lifetime( t: T?)
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp |> set("pfoo", new FooWithLifetime())
    commit()
    t |> success( total_foo==3)
    restart()
    t |> success( total_foo==0)

