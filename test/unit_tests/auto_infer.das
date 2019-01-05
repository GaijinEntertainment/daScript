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
    // regular array
    let a:int[10]
    let b = a
    let c : auto const = b
    // pointer
    let pFoo : Foo? = new Foo
    let pFooI : auto? = pFoo
    pFooI.bar = 1
    pFoo = pFooI
    assert(pFoo.bar==1)
    // array
    let TT_A : array<int>
    let TT_iA : array<auto> = TT_A
    // table
    let TT_T : table<string,int>
    let TT_iT : table<auto,auto> = TT_T
    let TT_ikT : table<auto,int> = TT_T
    let TT_ivT : table<string,auto> = TT_T
    // block return type
    let f1 = foo() <| $(arg:int):auto
        return arg+1
    assert(f1==4)
    // block argument
    let f2 = foo() <| $(arg=5):auto
        return arg+2
    assert(f2==5)
    // function return type
    assert(fn1()==3)
    return true

def fn3:auto
    return 3

def fn2:auto
    return fn3()

def fn1:auto
    return fn2()
