options log = true

struct Foo
    a : int
    b : float

[export]
def test
    let x = [[ Foo[3] a=1,b=2.0; a=2,b=3.0; a=3,b=4.0 ]]
    let y : Foo[3]
    y = x
    return true
