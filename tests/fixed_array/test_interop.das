// Stage 0 characterization for the tFixedArray rework (FIXED_ARRAY_REWORK.md).
// Pins C++ interop: a handled type's native C-array field (TestObjectFoo.fooArray,
// bound via typeFactory<TT[dim]> with isNativeDim) must keep reading/writing and
// keep its type identity through the representation flip.
options gen2
require UnitTest
require dastest/testing_boost public

[test]
def test_native_dim_field(t : T?) {
    t |> run("read/write native int[3] field") @(t : T?) {
        var foo : TestObjectFoo
        for (i in range(3)) {
            foo.fooArray[i] = (i + 1) * 11
        }
        t |> equal(foo.fooArray[0], 11)
        t |> equal(foo.fooArray[1], 22)
        t |> equal(foo.fooArray[2], 33)
    }
    t |> run("native field typename") @(t : T?) {
        var _foo : TestObjectFoo
        t |> equal(typeinfo typename(_foo.fooArray), "int[3]")
        t |> equal(typeinfo dim(_foo.fooArray), 3)
    }
    t |> run("native field iterates") @(t : T?) {
        var foo : TestObjectFoo
        for (i in range(3)) {
            foo.fooArray[i] = i + 1
        }
        var sum = 0
        for (x in foo.fooArray) {
            sum += x
        }
        t |> equal(sum, 6)
    }
    t |> run("native field passes to unsized param") @(t : T?) {
        var foo : TestObjectFoo
        for (i in range(3)) {
            foo.fooArray[i] = i + 10
        }
        t |> equal(sum_unsized(foo.fooArray), 33)
    }
    t |> run("native field copies out to das FA") @(t : T?) {
        var foo : TestObjectFoo
        foo.fooArray[1] = 42
        var local : int[3]
        local = foo.fooArray
        foo.fooArray[1] = 0
        t |> equal(local[1], 42)
    }
}

def sum_unsized(a : int[]) : int {
    var sum = 0
    for (x in a) {
        sum += x
    }
    return sum
}
