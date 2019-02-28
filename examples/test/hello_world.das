options log = true, logStack=true, logNodes = true, optimize=true // , printRef = true, printVarAccess = true, logOptimizationPasses = true

struct Foo
    x, y : int
    a : int[3] = [[int[3] 11; 22; 33]]

struct Bar
    a, b : Foo

[export]
def test
    let pf = new [[Foo() x=1,y=2]]
    print("pf={pf}\n")
    let pf2 = new [[Foo[2]() x=1,y=2;x=3,y=4]]
    print("pf2={pf2}\n")
    let pb = new [[ Bar a=[[Foo() x=1,y=2]], b=[[Foo() x=3,y=4]] ]]
    print("pb={pb}\n")
    let b = [[ Bar a=[[Foo() x=1,y=2]], b=[[Foo() x=3,y=4]] ]]
    print("b={b}\n")
    return true

