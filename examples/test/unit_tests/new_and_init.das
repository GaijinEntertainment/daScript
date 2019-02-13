options alwaysExportInitializer=true

require UnitTest

struct Foo
    x:int = 1
    y:int = 2
    z:int = 3
    w:int = 4

[export]
def test
    let f : Foo
    let q : Foo? = new_and_init(f)
    assert(q.x==1 & q.y==2 & q.z==3 & q.w==4)
    return true


