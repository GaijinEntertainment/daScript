require UnitTest

expect 30106
expect 30303:4    // operator_not_found
expect 30304:3    // function_not_found
expect 30504:6    // cant_write_to_const
expect 30505:2    // cant_move_to_const
expect 30507      // cant_copy

def const_iter(var a:table<string;int>)
    for k,v in keys(a),values(a)
        k = "foo"       // can't write to constant value    (30504)
        v = 1           // this one okay

def add(a:int const&; b:int)
    a += b              // 30304: no matching functions or generics _::+= ( int const&, int const& )
                        // 30303: numeric operator += left side can't be constant.

def set(a:string const&; b:string)
    a = b               // can't write to constant value    (30504)

def move_ar(a:array<int> const;b:array<int>)
    a <- b              // can't move to constant value     (30505)

struct Foo
    a:int
    b:int const

struct TestObjectFoo
    a:int

def setFoo(var f:Foo)
    f.a = 0

def setFooPtr(var f:Foo?)
    f.a = 0

def goodPassThrough(var a:int?):int const?
    return a            // fine

def passThrough(a:int? const):int?
    return a

def test
    if true
        var a:array<int> const
        a[0] = 5        // can't write to constant value    (30504)
    if true
        var an:Foo
        an.a = 1
        an.b = 2        // can't write to constant value    (30504)
    if true
        var ac:Foo const
        ac.a = 3        // can't write to constant value    (30504)
        ac.b = 4        // can't write to constant value    (30504)
    if true
        var pa:Foo const?
        var pb:Foo?
        setFoo(deref(pa))    // no matching function        (30304)
        setFoo(deref(pb))
        setFooPtr(pa)        // no matching function         (30304)
        setFooPtr(pb)
    if true
        var a:table<string;int>
        var b:table<string;int> const
        a["key"]++
        b["key"]++      // 30106: can't index in the constant table, use find instead
    if true
        var a:int?
        var b:int const?
        a = b
    if true
        var a,b:table<string;int>
        var c:table<string;int> const
        var d:int
        a = b           // this type can't be copied        (30507)
        a <- b
        c <- a          // can't move to constant           (30505)
        c <- d          // can only move same type          (30303)
    if true
        var a,b:int
        a <- b
        a = b
    if true
        var a : ::TestObjectFoo
        var b : UnitTest::TestObjectFoo
        a = b           // not the same type






