// options log=true, logInferPasses=false, optimize=true

struct Foo
    i : int = 5
    f : float = 6.0
    s : string = "hello"
    n : int64

struct Bar
    i : int
    j : int[3]
    t : array<float>
    k : table<string,int>

def Bar
    let b : Bar
    b.i = 13
    b.j = [[int 1;2;3]]
    resize(b.t,3)
    b.t[0] = 1f
    b.t[1] = 2f
    b.t[2] = 3f
    b.k["a"] = 1
    b.k["b"] = 2
    return <- b

[export]
def test
    let t = Foo()
    assert(t.i==5)
    debug(t,"t=")
    let b <- Bar()
    assert(b.i==13)
    debug(b)
    let m : float4x4
    debug(m)
    return true
