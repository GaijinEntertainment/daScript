options persistent_heap = true
options gc

require daslib/decs_boost
require dastest/testing_boost public
require daslib/random
require daslib/math

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

[test]
def test_gc_stress( t: T?)
    var seed = random_seed(13)
    restart()
    for count in range(100)
        let morei = (random_int(seed) & 31) + 10
        for i in range(morei)
            create_entity <| @ ( eid, cmp )
                var values <- [{for x in range(i); int2(x,x+13)}]
                cmp |> set("values", values)
                let random_arch = random_int(seed) & 31
                cmp |> set("arch_{random_arch}", random_arch)
                cmp.i := i
        let morej = (random_int(seed) & 31) + 10
        for j in range(morei)
            create_entity <| @ ( eid, cmp )
                cmp.txt := "text_{eid}"
                let random_arch = random_int(seed) & 63
                cmp |> set("arch_{random_arch}", random_arch)
        let delk = (random_int(seed) & 63)
        for j in range(delk)
            query <| $ ( eid:EntityId )
                let chance = random_int(seed) % 100
                if chance <= 4
                    delete_entity(eid)
        before_gc()
        unsafe
            heap_collect(true,true)
        after_gc()
        commit()
        query <| $ ( eid:EntityId; text:string )
            t |> equal(text, "text_{eid}")
        query <| $ ( var values:array<int2>; var i:int )
            let addi = (random_int(seed) & 127) + 10
            for t in range(addi)
                values |> push(int2(i+t,i+t+13))
            i += addi
        query <| $ ( var values:array<int2>; var i:int )
            var subi = (random_int(seed) & 63) + 5
            subi = min(length(values)-10, subi)
            for t in range(subi)
                values |> pop
            i -= subi
        query <| $ ( values:array<int2>; i:int )
            for x,v in range(i+1),values
                if t |> equal(int2(x,x+13), v )
                    break
        before_gc()
        unsafe
            heap_collect(true,true)
        after_gc()
        query <| $ ( values:array<int2>; i:int )
            for x,v in range(i+1),values
                if t |> equal(int2(x,x+13), v )
                    break
