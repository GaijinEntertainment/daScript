// options optimize=false, log=true

struct Foo
    bar : int

let
    intnum = 5
    floatnum = 5.5
    stringval = "hello, world"

def foo(b:block<(a:int):int>)
    return invoke(b, 3)

def foobar(b:block<(a:int;b:float):string>)
    return invoke(b,1,2.0) + "bar"

def madd(a,b,c)
    return a*b+c

def take_arr(a:int[])   // infer array size
    var summ = 0
    for x in a
        summ += x
    return summ

[export]
def test
    // regular array
    let a:int[10]
    let b = a
    let c : auto const = b
    // array size
    assert(take_arr([[int[4] 1;2;3;4]])==10)
    // pointer
    var pFoo : Foo? = new Foo
    var pFooI : auto? = pFoo
    pFooI.bar = 1
    pFoo = pFooI
    assert(pFoo.bar==1)
    // array
    let TT_A : array<int>
    let TT_iA : array<auto> <- TT_A
    // table
    let TT_T : table<string,int>
    let TT_iT : table<auto,auto> <- TT_T
    let TT_ikT : table<auto,int> <- TT_T
    let TT_ivT : table<string,auto> <- TT_T
    // block return type
    let f1 = foo() <| $(arg:int)
        return arg+1
    assert(f1==4)
    // block argument
    let f2 = foo() <| $(arg=5)
        return arg+2
    assert(f2==5)
    // forward infer entire block type and argument types
    let f3 = foo() <| $(arg)
        return arg + 3
    assert(f3==6)
    // two arguments
    let blah = foobar() <| $(ia,fb)
        assert(ia==1)
        assert(fb==2.0)
        return "foo"
    assert(blah=="foobar")
    // function return type
    assert(fn1()==6)
    // full-blown generic
    let x = madd(1,2,3)
    assert(x==5)
    let y = madd(1.0,2.0,3.0)
    assert(y==5.0)
    return true

def fn3
    return 3

def fn2
    return fn3() + 1

def fn1
    return fn2() + 2
