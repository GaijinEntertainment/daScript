struct Foo
    a:int
    b:int = -1

[export]
def test
    var t = Foo(a=10)
    assert(t.a == 10)
    assert(t.b == -1)
    return true

