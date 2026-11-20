options gen2
require dastest/testing_boost public

options no_aliasing

struct Plain {
    a : int
    b : float
}

def private store_constants(var p : Plain?; var d : Plain?; var t : tuple<int; string>?) {
    *p = default<Plain>
    *d = Plain(a = 1, b = 2.0)
    *t = (3, "three")
}

[test(description = "a default or a structure or tuple made of constants stored through a pointer compiles under no_aliasing and holds those constants")]
def test_store_constants(t : T?) {
    var p = Plain(a = 9, b = 9.0)
    var d : Plain
    var tup : tuple<int; string>
    store_constants(unsafe(addr(p)), unsafe(addr(d)), unsafe(addr(tup)))
    t |> equal(p.a, 0)
    t |> equal(p.b, 0.0)
    t |> equal(d.a, 1)
    t |> equal(d.b, 2.0)
    t |> equal(tup._0, 3)
    t |> equal(tup._1, "three")
}
