options gen2

require UnitTest
require dastest/testing_boost public

// Stage 0 characterization for the tFixedArray rework (FIXED_ARRAY_REWORK.md).
// Pins C++ interop: a handled type's native C-array field (TestObjectFoo.fooArray,
// bound via typeFactory<TT[dim]> with isNativeDim) must keep reading/writing and
// keep its type identity through the representation flip. AOT-included: iterate/
// pass/copy/safe-at of a native dim field is the issue #3077 surface, fixed at
// Stage 2 by punning the member access to its TDim view.

[test(description = "a native C array field of a handled type reads, writes, iterates, passes to unsized parameters and copies in and out of das fixed arrays")]
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
        let local = foo.fooArray
        foo.fooArray[1] = 0
        t |> equal(local[1], 42)
    }
    t |> run("native field copies in from das FA") @(t : T?) {
        var foo : TestObjectFoo
        var local : int[3]
        for (i in range(3)) {
            local[i] = (i + 1) * 7
        }
        foo.fooArray = local
        t |> equal(foo.fooArray[0], 7)
        t |> equal(foo.fooArray[1], 14)
        t |> equal(foo.fooArray[2], 21)
    }
    t |> run("native field safe-at") @(t : T?) {
        var foo : TestObjectFoo
        foo.fooArray[1] = 55
        t |> equal(foo.fooArray?[1] ?? -1, 55)
        t |> equal(foo.fooArray?[7] ?? -1, -1)
    }
}

def sum_unsized(a : int[]) : int {
    var sum = 0
    for (x in a) {
        sum += x
    }
    return sum
}

// Stage 0 characterization for the tFixedArray rework (FIXED_ARRAY_REWORK.md).
// Pins iteration: for-loops at every depth, mutation through the loop ref,
// each(), parallel iteration with other sources.

struct Cell {
    v : int
}

[test(description = "for loops over fixed arrays of one and two dimensions yield elements and rows, mutate through the loop reference and iterate structs by reference")]
def test_for_loops(t : T?) {
    t |> run("1D for") @(t : T?) {
        var a : int[4]
        for (i in range(4)) {
            a[i] = i + 1
        }
        var sum = 0
        for (x in a) {
            sum += x
        }
        t |> equal(sum, 10)
    }
    t |> run("2D for yields rows") @(t : T?) {
        var m : int[2][3]
        for (y in range(2)) {
            for (x in range(3)) {
                m[y][x] = y * 3 + x
            }
        }
        var rows = 0
        var total = 0
        for (row in m) {
            rows ++
            for (x in row) {
                total += x
            }
        }
        t |> equal(rows, 2)
        t |> equal(total, 15)
    }
    t |> run("mutation through loop ref") @(t : T?) {
        var a : int[4]
        for (x in a) {
            x = 7
        }
        var sum = 0
        for (x in a) {
            sum += x
        }
        t |> equal(sum, 28)
    }
    t |> run("iterate FA of structs by ref") @(t : T?) {
        var cells : Cell[3]
        for (c in cells) {
            c.v = 5
        }
        t |> equal(cells[0].v + cells[1].v + cells[2].v, 15)
    }
}

[test(description = "each() over a fixed array and parallel iteration with a range or an array visit matching elements")]
def test_each_and_parallel(t : T?) {
    t |> run("each() over 1D") @(t : T?) {
        var s : int[4]
        for (i in range(4)) {
            s[i] = i * 10
        }
        var sum = 0
        for (v in each(s)) {
            sum += v
        }
        t |> equal(sum, 60)
    }
    t |> run("parallel iteration FA + range") @(t : T?) {
        var a : int[4]
        for (x, i in a, range(4)) {
            x = i * 2
        }
        t |> equal(a[3], 6)
    }
    t |> run("parallel iteration FA + array") @(t : T?) {
        var a : int[3]
        var b <- [10, 20, 30]
        var sum = 0
        for (x, y in a, b) {
            sum += x + y
        }
        t |> equal(sum, 60)
        delete b
    }
}
