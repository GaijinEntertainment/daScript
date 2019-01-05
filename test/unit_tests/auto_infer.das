// options optimize=false, log=true

struct Foo
    bar : int

let
    intnum = 5
    floatnum = 5.5
    stringval = "hello, world"

def foo(b:block<(a:int):int>) : int
    return invoke(b, 3)

def test:bool
    let a:int[10]
    let b = a
    let c : auto const = b
    let pFoo : Foo? = new Foo
    let pFooI : auto? = pFoo
    pFooI.bar = 1
    pFoo = pFooI
    assert(pFoo.bar==1)
    let f1 = foo() <| $(arg:int):auto
        return arg+1
    assert(f1==4)
    let f2 = foo() <| $(arg=5):auto
        return arg+2
    assert(f2==5)
    assert(fn1()==3)
    return true

def fn3:auto
    return 3

def fn2:auto
    return fn3()

def fn1:auto
    return fn2()
