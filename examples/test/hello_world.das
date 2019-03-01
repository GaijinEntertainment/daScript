options log = true, logStack=true, logNodes = true, optimize=true // , printRef = true, printVarAccess = true, logOptimizationPasses = true

struct Foo
    x, y : int
    a : int[3] = [[int[3] 11; 22; 33]]

struct Bar
    a, b : Foo

def makeBar
    return [[ Bar a=[[Foo() x=1,y=2]], b=[[Foo() x=3,y=4]] ]]

def makeFooBar
    return makeBar()

[export]
def test
    // let b0 = [[ Bar a=[[Foo() x=1,y=2]], b=[[Foo() x=3,y=4]] ]]
    // print("b0={b0}\n")
    // let b00 : Bar
    // b00 = [[ Bar a=[[Foo() x=1,y=2]], b=[[Foo() x=3,y=4]] ]]
    // print("b00={b00}\n")
    let b1 = makeBar()
    print("b1={b1}\n")
    let b2 : Bar
    b2 = makeFooBar()
    print("b2={b2}\n")
    // let b3 = invoke() <| $()
    //     return [[ Bar a=[[Foo() x=1,y=2]], b=[[Foo() x=3,y=4]] ]]
    // print("b3={b3}\n")
    return true

