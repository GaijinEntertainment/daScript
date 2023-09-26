require daslib/forward

struct Foo
    a : float

[forward(type="Foo")]
struct Foo_forward
    b : uint

def take_foo ( f : Foo_forward )
    print("f = {f}\n")

[export]
def test
    var foo = [[Foo a=1.]]
    take_foo(foo)
    return true


