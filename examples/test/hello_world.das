options logInferPasses=true // log=true// , logOptimizationPasses=true

def make_a
    let a:array<int>
    for i in range(0,10)
        push(a,i)
    return <- a

struct Foo
    a: array<int> <- make_a()
    b:int = 3
    c:float

[export]
def test
    let foo <- [[Foo() b=4]]
    print("foo={foo}\n")
    return true


