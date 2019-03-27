options log = true

require UnitTest

// [checkRange(disable=true,dummy=true)]
[checkRange]
struct Foo
    [min=0,max=3] a:int = 1
    [min=10,max=20] b:int = 2
    [min=2] c:int


[export]
def test
    let t : Foo
    return true
