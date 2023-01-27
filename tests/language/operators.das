require dastest/testing_boost public

struct Foo
    a : int

struct Bar
    a : float

def operator as Bar ( a : Foo )
    return [[Bar a = float(a.a)]]

def operator ?as Bar ( a : Foo? ) : Bar?
    if a == null
        return null
    return new [[Bar a = float(a.a)]]

def operator is Bar ( anything )
    return false

def operator is Bar ( a : Bar )
    return true

[test]
def test_operators ( t : T? )
    t |> run("as, ?as, is") <| @ ( t : T? )
        var f = [[Foo a= 13]]
        var b = f as Bar
        t |> equal(13., b.a)
        t |> success(b is Bar)
        t |> success(!(f is Bar))
        var pf = unsafe(addr(f))
        var pb = pf ?as Bar ?? [[Bar a = 0.0]]
        t |> equal(13., pb.a)
        var nf : Foo?
        var nb = nf ?as Bar ?? [[Bar a = 1.0]]
        t |> equal(1., nb.a)
