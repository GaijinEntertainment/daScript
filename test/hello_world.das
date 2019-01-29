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

    return <- b

[export]
def test
    let t = Foo()
    assert(t.i==5)
    debug(t,"t=")
    let b <- Bar()
    assert(b.i==13)
    debug(b)
    return true
