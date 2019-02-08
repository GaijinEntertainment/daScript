require UnitTest

expect 30102:2    // invalid_return_type
expect 30303:5    // operator_not_found
expect 30304:2    // function_not_found
expect 30504:6    // cant_write_to_const
expect 30505:2    // cant_move_to_const
expect 30507      // cant_copy

def const_iter(a:table<string,int>)
    for k,v in keys(a),values(a)
        k = "foo"       // can't write to constant value    (30504)
        v = 1           // this one okay

def add(a:int const&; b:int)
    a += b              // no matching operator             (30303)

def set(a:string const&; b:string)
    a = b               // can't write to constant value    (30504)

def move_ar(a:array<int> const;b:array<int>)
    a <- b              // can't move to constant value     (30505)

struct Foo
    a:int
    b:int const

struct TestObjectFoo
    a:int

def setFoo(f:Foo)
    f.a = 0

def setFooPtr(f:Foo?)
    f.a = 0

def goodPassThrough(a:int?):int const?
    return a            // invalid return type              (30102)

def passThrough(a:int? const):int?
    return a            // invalid return type, const       (30102)

def test
    let(a:array<int> const)
        a[0] = 5        // can't write to constant value    (30504)
    let(an:Foo)
        an.a = 1
        an.b = 2        // can't write to constant value    (30504)
    let(ac:Foo const)
        ac.a = 3        // can't write to constant value    (30504)
        ac.b = 4        // can't write to constant value    (30504)
    let(pa:Foo const?;pb:Foo?)
        setFoo(deref(pa))    // no matching function             (30304)
        setFoo(deref(pb))
        setFooPtr(pa)   // no matching function             (30304)
        setFooPtr(pb)
    let(a:table<string,int>;b:table<string,int> const)
        a["key"]++
        b["key"]++      // no matching operator             (30303)
    let(a:int?;b:int const?)
        a = b
    let(a,b:table<string,int>;c:table<string,int> const;d:int)
        a = b           // this type can't be copied        (30507)
        a <- b
        c <- a          // can't move to constant           (30505)
        c <- d          // can only move same type          (30303)
    let(a,b:int)
        a <- b          
        a = b
    let(a: ::TestObjectFoo;b:UnitTest::TestObjectFoo)
        a = b           // not the same type






