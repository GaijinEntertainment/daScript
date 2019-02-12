// options log=true, logInferPasses=false, optimize=true

expect 30113, 30109

struct Foo
    i : int = 5
    a : array<int>

let
    g : Foo = new Foo()	// 30113: global variable initialization type mismatch, Foo = Foo?
			// 30109: Foo does not have default initializer

[export]
def test
    push(g.a, 10)
    assert(g.i==5)
    return true
