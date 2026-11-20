options gen2
require dastest/testing_boost public

options no_aliasing

require _aliasing_generic_lax

struct Plain {
    a : int
    b : float
}

struct IntPair {
    a : int
    b : int
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

def private store_values(var p : IntPair?; var t : tuple<int; int>?; x : int) {
    *p = IntPair(a = x, b = 1)
    *t = (1, x)
}

[test(description = "a structure or tuple made of plain local values stored through a pointer compiles under no_aliasing")]
def test_store_values(t : T?) {
    var p : IntPair
    var tup : tuple<int; int>
    store_values(unsafe(addr(p)), unsafe(addr(tup)), 7)
    t |> equal(p.a, 7)
    t |> equal(p.b, 1)
    t |> equal(tup._0, 1)
    t |> equal(tup._1, 7)
}

[test(description = "an aliasing generic from a module with no_aliasing off compiles in a no_aliasing caller and swaps through a temporary")]
def test_lax_generic_instance(t : T?) {
    var q = IntPair(a = 3, b = 4)
    swap_fields(q)
    t |> equal(q.a, 4)
    t |> equal(q.b, 3)
}
