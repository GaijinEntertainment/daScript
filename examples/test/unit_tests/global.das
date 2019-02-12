// options logInferPasses=true

struct Foo
    i : int = 5
    a : array<int>

let
    g : Foo? = new Foo()	

[export]
def test
    push(g.a, 10)
    assert(g.i==5)
    return true
