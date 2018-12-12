expect 30303    // operator_not_found
expect 30304:2  // function_not_found
expect 30504:6  // cant_write_to_const
expect 30505    // cant_move_to_const

def const_iter(a:table<string,int>)
    for k,v in keys(a),values(a)
        k = "foo"   // can't write to constant value
        v = 1       // this one okay

def add(a:const int&; b:int)
    a += b          // no matching operator

def set(a:const string&; b:string)
    a = b           // can't write to constant value

def move_ar(a:const array<int>;b:array<int>)
    a <- b          // can't move to constant value

struct Foo
    a:int
    b:const int

def setFoo(f:Foo)
    f.a = 0

def setFooPtr(f:Foo?)
    f.a = 0

def test
    let(a:const array<int>)
        a[0] = 5    // can't write to constant value
    let(an:Foo)
        an.a = 1
        an.b = 2    // can't write to constant value
    let(ac:const Foo)
        ac.a = 3    // can't write to constant value
        ac.b = 4    // can't write to constant value
    let(pa:const Foo?;pb:Foo?)
        setFoo(pa->)    // no matching function
        setFoo(pb->)
        setFooPtr(pa)   // no matching function
        setFooPtr(pb)






