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
    run(t,"before gc") <| @ ( t : T? )
        query <| $ ( values:array<int>; i:int )
            for x,v in range(i+1),values
                equal(t, x, v)
    before_gc()
    unsafe
        heap_collect(true,true)
    after_gc()
    run(t,"after gc") <| @ ( t : T? )
        query <| $ ( values:array<int>; i:int )
            for x,v in range(i+1),values
                equal(t, x, v)

[test]
def test_gc_strings( t: T?)
    restart()
    for i in range(3)
        create_entity <| @ ( eid, cmp )
            cmp.txt := "hello_{i}"
    commit()
    run(t,"before string gc") <| @ ( t : T? )
        var i = 0
        query <| $ ( txt:string )
            equal(t, txt, "hello_{i++}")
        equal(t, i, 3)
    before_gc()
    unsafe
        string_heap_collect(true)
    after_gc()
    run(t,"after string gc") <| @ ( t : T? )
        var j = 0
        query <| $ ( txt:string )
            equal(t, txt, "hello_{j++}")
        equal(t, j, 3)

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
    success(t, total_foo==3)
    restart()
    success(t, total_foo==0)

