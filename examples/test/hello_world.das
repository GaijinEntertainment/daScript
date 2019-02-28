options log = true, logStack=true, logNodes = true, optimize=true // , printRef = true, printVarAccess = true, logOptimizationPasses = true

struct Foo
    x, y : int

[export]
def test
    let pf = new [[Foo x=1,y=2]]
    print("pf={pf}\n")
    return true

