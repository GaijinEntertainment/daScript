options log = true

struct Foo
    [min=0,max=3] a:int = 1
    [precision=3] b:float = 2.


[export]
def test
    let t : Foo
    return true
