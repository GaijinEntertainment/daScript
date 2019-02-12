struct Foo
    x:int = 1
    y:int = 2
    z:int = 3
    w:int = 4

[export]
def test
    let t0 = [[Foo x=13]]
    assert(t0.x==13 & t0.y==0 & t0.z==0 & t0.w==0)
    let t1 = [[Foo() x=13]]
    assert(t1.x==13 & t1.y==2 & t1.z==3 & t1.w==4)
    return true


