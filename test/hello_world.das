// options log=true, logStack=true, optimize=true

struct Foo
    i : int = 5
    f : float = 6.0
    s : string = "hello"

[export]
def test
    let t = Foo()
    assert(t.i==5)
    assert(t.f==6.0)
    assert(t.s=="hello")
    let q = [[Foo i=6, s="world"]]
    assert(q.i==6)
    assert(q.f==0.0)
    assert(q.s=="world")
    let qq3 = [[Foo i=1; i=2; i=3]]
    assert(qq3[0].i==1 & qq3[1].i==2 & qq3[2].i==3)
    return true

