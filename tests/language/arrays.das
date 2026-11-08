options gen2
options no_unused_function_arguments = false

require UnitTest
require math
require daslib/sort_boost
require daslib/cmp
require daslib/coroutines
require dastest/testing_boost public

[sideeffects]
def make_test_array(var a : array<int>; extra : int = 0) {
    var i = 0
    resize(a, 10)
    for (x in a) {
        x = i++ + extra
    }
}

[sideeffects]
def verify_test_array(t : T?; a : array<int>; extra : int = 0) {
    t |> equal(length(a), 10)
    var i = 0
    for (x in a) {
        t |> equal(x, extra + i++)
    }
}

[sideeffects]
def pass_array(var arr : array<int>) {
    push(arr, 4)
}

[sideeffects]
def verifyIntArray(t : T?; a : array<int>; var s : int; c, tail : int) {
    for (idx in range(c)) {
        t |> equal(a[idx], s++)
    }
    for (idx in range(c, c + tail)) {
        t |> equal(a[idx], 0)
    }
}

[sideeffects]
def testIntArray(t : T?) {
    var arr, arr2 : array<int>
    let val2 = 2
    t |> equal(length(arr), 0)
    push(arr, 1)
    push(arr, val2)
    push(arr, 3)
    t |> equal(length(arr), 3)
    t |> equal(capacity(arr), 16)
    verifyIntArray(t, arr, 1, 3, 0)
    pass_array(arr)
    t |> equal(length(arr), 4)
    verifyIntArray(t, arr, 1, 4, 0)
    resize(arr, 6)
    t |> equal(length(arr), 6)
    verifyIntArray(t, arr, 1, 4, 2)
    resize(arr, 17)
    verifyIntArray(t, arr, 1, 4, 13)
    t |> equal(capacity(arr), 32)
    reserve(arr, 33)
    verifyIntArray(t, arr, 1, 4, 13)
    t |> equal(capacity(arr), 33)
    resize(arr, 4)
    t |> equal(capacity(arr), 33)
    t |> equal(length(arr), 4)
    arr2 <- arr     // nolint:PERF030 - array move-assign is the tested construct
    t |> equal(length(arr), 0)
    t |> equal(length(arr2), 4)
    var index = 0
    while (index < 4) {
        arr2[index++]++
    }
    verifyIntArray(t, arr2, 2, 4, 0)
    arr <- arr2     // nolint:PERF030 - array move-assign is the tested construct
    verifyIntArray(t, arr, 2, 4, 0)
    resize(arr, 3)
    verifyIntArray(t, arr, 2, 3, 0)
    resize(arr, 4)
    t |> equal(arr[3], 0)
    verifyIntArray(t, arr, 2, 3, 1)
    push(arr, 1, 0)
    verifyIntArray(t, arr, 1, 4, 1)
    resize(arr, 4)
    push(arr, 5, 4)
    verifyIntArray(t, arr, 1, 5, 0)
    push(arr, 7, 2)
    erase(arr, 2)
    verifyIntArray(t, arr, 1, 5, 0)
    erase(arr, 0)
    erase(arr, 3)
    verifyIntArray(t, arr, 2, 3, 0)
}

def testInterop(t : T?) {
    let res = temp_array_example([ "one" ]) $(arr) {
        t |> equal(length(arr), 1)
        t |> equal(arr[0], "one")
    }
    t |> success(res)
}

[test(description = "an int array keeps its elements through fill, move-out and move-back, two-array parallel for, push, resize, reserve and erase, and push, push_from, push_clone_from and emplace_from append struct arrays and fixed arrays")]
def test_array(t : T?) {
    {
        var a : array<int>
        make_test_array(a)
        verify_test_array(t, a)
        make_test_array(a, 13)
        verify_test_array(t, a, 13)
        make_test_array(a)
        verify_test_array(t, a)
        {
            var b : array<int> <- a
            t |> equal(length(a), 0)
            verify_test_array(t, b)
            a <- b      // nolint:PERF030 - moving back into the emptied array is the tested construct
        }
        verify_test_array(t, a)
    }
    {
        var a, b : array<int>
        var i = 0
        resize(a, 10)
        resize(b, 10)
        for (l, r in a, b) {
            l = i
            r = i * 2 - 5
            i++
        }
        verify_test_array(t, a)
        i = 0
        for (l, r in a, b) {
            t |> equal(l, i)
            t |> equal(r, i * 2 - 5)
            i++
        }
        i = 0
        for (l, r in a, b) {
            if (l > r) {
                t |> success(l > r)
                i++
            }
        }
        t |> equal(i, 5)
    }
    {
        var a : array<int>[10]
        var i = 0
        for (b in a) {
            make_test_array(b, i++)
        }
        i = 0
        for (b in a) {
            verify_test_array(t, b, i++)
        }
    }
    testIntArray(t)
    testConstInArray()
    testInterop(t)
    testMultiPush(t)
    testMultiPushFrom(t)
}

// this test bellow is only here to make sure AOT compiles and runs
// for array<blah const>

struct Foo {
    bar : int
}

[sideeffects]
def testAccept(t : array<Foo const>; e : Foo const[10]) {
    for (i in t) {
        return true if (i.bar == 13)
    }
    for (j in e) {
        return false if (j.bar == 13)
    }
    return false
}

[sideeffects]
def testConstInArray {
    var a : array<Foo const>
    resize(a, 20)
    var b : Foo const[10]
    testAccept(a, b)
}

[sideeffects]
def testMultiPush(t : T?) {
    var a, b : array<Foo>
    push(b, a)
    t |> equal(length(a), 0)
    t |> equal(length(b), 0)

    a <- [Foo(bar=1), Foo(bar=2)]   // nolint:PERF030 - a is still empty here; the empty-push case above must run first
    push(b, a)
    t |> equal(length(a), 2)
    t |> equal(length(b), 2)
    t |> equal(a[0].bar, 1)
    t |> equal(a[1].bar, 2)
    t |> equal(b[0].bar, 1)
    t |> equal(b[1].bar, 2)

    var c = fixed_array(Foo(bar = 3))
    push(b, c)
    t |> equal(length(b), 3)
    t |> equal(b[2].bar, 3)
    t |> equal(c[0].bar, 3)

    push_clone(b, c)
    t |> equal(length(b), 4)
    t |> equal(b[3].bar, 3)
}

[sideeffects]
def testMultiPushFrom(t : T?) {
    // 1. push_from(arr, array<T> source)
    var a <- [Foo(bar=10), Foo(bar=20)]
    var b : array<Foo>      // nolint:STYLE032 - push_from into a fresh array is the tested construct
    push_from(b, a)
    t |> equal(length(a), 2)
    t |> equal(length(b), 2)
    t |> equal(b[0].bar, 10)
    t |> equal(b[1].bar, 20)

    // 2. push_from(arr, C-array source) — also exercises
    // Arr-already-has-elements reserve
    var c = fixed_array(Foo(bar = 30), Foo(bar = 40))
    push_from(b, c)
    t |> equal(length(b), 4)
    t |> equal(b[2].bar, 30)
    t |> equal(b[3].bar, 40)

    // 3. push_clone_from(arr, array<T> source) — the array<numT> gap-fill
    var d : array<Foo>      // nolint:STYLE032 - push_clone_from into a fresh array is the tested construct
    push_clone_from(d, a)
    t |> equal(length(d), 2)
    t |> equal(d[0].bar, 10)
    t |> equal(d[1].bar, 20)

    // 4. push_clone_from(arr, C-array source)
    push_clone_from(d, c)
    t |> equal(length(d), 4)
    t |> equal(d[2].bar, 30)
    t |> equal(d[3].bar, 40)

    // 5. emplace_from(arr, array<T> source) — the array<numT> gap-fill
    var e : array<Foo>
    emplace_from(e, a)
    t |> equal(length(e), 2)
    t |> equal(e[0].bar, 10)
    t |> equal(e[1].bar, 20)

    // 6. emplace_from(arr, C-array source)
    emplace_from(e, c)
    t |> equal(length(e), 4)
    t |> equal(e[2].bar, 30)
    t |> equal(e[3].bar, 40)
}

[test(description = "array push, by-ref passing, resize, reserve, move, insert and erase keep the elements in place, and resize rounds capacity up to a power of two from a floor of 16 while reserve stays exact")]
def test_dynamic_array(t : T?) { // nolint:STYLE038 — flat list of independent container-op arms
    t |> run("push and length") @(t : T?) {
        var arr : array<int>
        let val2 = 2
        t |> equal(length(arr), 0)
        push(arr, 1)
        push(arr, val2)
        push(arr, 3)
        t |> equal(length(arr), 3)
        t |> equal(capacity(arr), 16)
    }
    t |> run("pass array by ref") @(t : T?) {
        var arr <- [1, 2, 3]
        pass_array_helper(arr)
        t |> equal(length(arr), 4)
        t |> equal(arr[3], 4)
    }
    t |> run("resize and reserve") @(t : T?) {
        var arr <- [1, 2, 3, 4]
        resize(arr, 6)
        t |> equal(length(arr), 6)
        resize(arr, 17)
        t |> equal(capacity(arr), 32)
        reserve(arr, 33)
        t |> equal(capacity(arr), 33)
        resize(arr, 4)
        t |> equal(capacity(arr), 33)
        t |> equal(length(arr), 4)
    }
    t |> run("resize rounds capacity to the next power of two") @(t : T?) {
        var min : array<int>
        resize(min, 3)                  // below the floor -> min capacity 16
        t |> equal(capacity(min), 16)
        var tiny : array<int>
        resize(tiny, 63)                // rounds UP to the next pow2
        t |> equal(capacity(tiny), 64)
        var big : array<int>
        resize(big, 65)                 // pow2 slack keeps append-by-resize amortized O(1)
        t |> equal(capacity(big), 128)
        var huge : array<int>
        resize(huge, 1000)
        t |> equal(capacity(huge), 1024)
        var exact : array<int>
        reserve(exact, 1000)            // reserve is exact: N set, N get
        resize(exact, 1000)             // a reserved resize never grows, so no pow2 round-up
        t |> equal(capacity(exact), 1000)
    }
    t |> run("move semantics") @(t : T?) {
        var arr <- [1, 2, 3, 4]
        var arr2 : array<int>
        arr2 <- arr     // nolint:STYLE011,PERF030 - move-assign into an existing array is what this arm covers
        t |> equal(length(arr), 0)
        t |> equal(length(arr2), 4)
        var index = 0
        while (index < 4) {
            arr2[index++]++
        }
        arr <- arr2     // nolint:PERF030 - arr was emptied by the move above; the round trip is the tested construct
        t |> equal(length(arr2), 0)
        t |> equal(length(arr), 4)
        // let-move
        var a <- [1, 2]
        let b <- a
        t |> equal(length(a), 0)
        t |> equal(length(b), 2)
    }
    t |> run("resize zeroes new elements") @(t : T?) {
        var arr <- [1, 2, 3, 4]
        resize(arr, 3)
        t |> equal(arr[2], 3)
        resize(arr, 4)
        t |> equal(arr[3], 0)
    }
    t |> run("insert and erase") @(t : T?) {
        var arr <- [2, 3, 4]
        // insert at front
        push(arr, 1, 0)
        for (i in range(1, 5)) {
            t |> equal(arr[i - 1], i)
        }
        resize(arr, 4)
        // insert at end
        push(arr, 5, 4)
        // insert in middle, then erase
        push(arr, 7, 2)
        erase(arr, 2)
        for (i in range(1, 5)) {
            t |> equal(arr[i - 1], i)
        }
        // erase first and last
        erase(arr, 0)
        erase(arr, 3)
        for (i in range(2, 4)) {
            t |> equal(arr[i - 2], i)
        }
    }
}

[test(description = "subarray over a range64 copies exactly the selected elements of a const or a var array, and an empty range64 yields an empty array")]
def test_subarray(t : T?) {
    t |> run("subarray range64 of a const array") @(t : T?) {
        let arr <- [0, 10, 20, 30]
        let got <- subarray(arr, range64(1l, 3l))
        t |> equal(length(got), 2)
        t |> equal(got[0], 10)
        t |> equal(got[1], 20)
    }
    t |> run("subarray range64 of a var array") @(t : T?) {
        var arr <- [0, 10, 20, 30]
        let got <- subarray(arr, range64(2l, 4l))
        t |> equal(length(got), 2)
        t |> equal(got[0], 20)
        t |> equal(got[1], 30)
    }
    t |> run("subarray empty range64") @(t : T?) {
        var arr <- [0, 10, 20, 30]
        let got <- subarray(arr, range64(3l, 3l))
        t |> equal(length(got), 0)
    }
}

def pass_array_helper(var arr : array<int>) {
    push(arr, 4)
}

[test(description = "push_from and push_clone_from with two, three or four sources append every source in order into one array")]
def test_variadic_push_from(t : T?) {
    // fixed-arity push_from (>=2 sources -> not STYLE032; one combined
    // reserve)
    var b2 : array<int>
    push_from(b2, [1, 2], [3, 4])
    t |> equal(length(b2), 4)
    t |> equal(b2[0], 1)
    t |> equal(b2[3], 4)

    var b3 : array<int>
    push_from(b3, [1, 2], [3], [4, 5, 6])
    t |> equal(length(b3), 6)
    t |> equal(b3[0], 1)
    t |> equal(b3[5], 6)

    var b4 : array<int>
    push_from(b4, [1], [2], [3], [4])
    t |> equal(length(b4), 4)
    t |> equal(b4[3], 4)

    var dc2 : array<int>
    push_clone_from(dc2, [10, 20], [30])
    t |> equal(length(dc2), 3)
    t |> equal(dc2[2], 30)

    var dc3 : array<int>
    push_clone_from(dc3, [10], [20, 30], [40])
    t |> equal(length(dc3), 4)
    t |> equal(dc3[3], 40)

    var dc4 : array<int>
    push_clone_from(dc4, [1], [2], [3], [4])
    t |> equal(length(dc4), 4)
    t |> equal(dc4[3], 4)
}

[test(description = "push_from and push_clone_from flatten nested int arrays of any depth into an array of int, while an array-of-arrays target receives the sub-arrays as whole elements")]
def test_nested_push_from(t : T?) {
    let nested <- [[100, 200], [300]]
    var fl : array<int>
    push_from(fl, nested)
    t |> equal(length(fl), 3)
    t |> equal(fl[0], 100)
    t |> equal(fl[2], 300)

    let deep <- [[[7, 8], [9]]]
    var fld : array<int>
    push_from(fld, deep)
    t |> equal(length(fld), 3)
    t |> equal(fld[0], 7)
    t |> equal(fld[2], 9)

    let cnested <- [[11, 22], [33]]
    var cfl : array<int>
    push_clone_from(cfl, cnested)
    t |> equal(length(cfl), 3)
    t |> equal(cfl[0], 11)
    t |> equal(cfl[2], 33)

    let cdeep <- [[[5, 6], [7]]]
    var cfld : array<int>
    push_clone_from(cfld, cdeep)
    t |> equal(length(cfld), 3)
    t |> equal(cfld[0], 5)
    t |> equal(cfld[2], 7)

    let src <- [[1, 2], [3]]
    var aoa : array<array<int>>                    // nolint:STYLE032 - push_clone_from is the tested construct
    push_clone_from(aoa, src)
    t |> equal(length(aoa), 2)
    t |> equal(aoa[0][1], 2)
    t |> equal(aoa[1][0], 3)
}

[test(description = "array and table comprehensions, plain and where-filtered, single-line and multiline, in bracket and brace form, produce exactly the mapped elements")]
def test_array_comprehension(t : T?) {
    t |> run("square comprehension") @(t : T?) {
        let q <- [for (x in range(10)); x * x]
        for (y, qV in range(10), q) {
            t |> equal(qV, y * y)
        }
    }
    t |> run("filtered comprehension") @(t : T?) {
        let a <- [for (x in range(10)); x * x; where(x & 1) == 0]
        var count = 0
        for (z in range(10)) {
            if ((z & 1) == 0) {
                t |> equal(a[count++], z * z)
            }
        }
    }
    t |> run("multiline table comprehension (issue #2459)") @(t : T?) {
        var name_to_index <- {
            for (i in range(5));
            (i, i)
        }
        for (i in range(5)) {
            t |> equal(name_to_index[i], i)
        }
    }
    t |> run("multiline array comprehension square form") @(t : T?) {
        let q <- [
            for (x in range(4));
            x * x
        ]
        for (y, qV in range(4), q) {
            t |> equal(qV, y * y)
        }
    }
    t |> run("multiline filtered comprehension brace form") @(t : T?) {
        let even_squares <- {
            for (x in range(10));
            (x, x * x);
            where(x & 1) == 0
        }
        for (z in range(10)) {
            if ((z & 1) == 0) {
                t |> equal(even_squares?[z] ?? -1, z * z)
            } else {
                t |> equal(even_squares?[z] ?? -1, -1)
            }
        }
    }
}

[test(description = "to_array over a fixed array, a range iterator, and each of a static or a dynamic array yields the same elements in the same order")]
def test_to_array(t : T?) {
    t |> run("from fixed_array") @(t : T?) {
        let d = fixed_array<int>(1, 2, 3, 4)
        let a <- to_array(fixed_array<int>(1, 2, 3, 4))
        for (x, y in d, a) {
            t |> equal(x, y)
        }
    }
    t |> run("from range via each") @(t : T?) {
        for (x, y in range(5), each(range(5))) {
            t |> equal(x, y)
        }
        for (x, y in range(5), to_array(each(range(5)))) {
            t |> equal(x, y)
        }
    }
    t |> run("from static array") @(t : T?) {
        let d = fixed_array<int>(1, 2, 3, 4)
        for (x, y in d, each(d)) {
            t |> equal(x, y)
        }
        for (x, y in d, to_array(d)) {
            t |> equal(x, y)
        }
        for (x, y in d, to_array(unsafe(each(d)))) {
            t |> equal(x, y)
        }
    }
    t |> run("from dynamic array") @(t : T?) {
        let a <- to_array(fixed_array<int>(1, 2, 3, 4))
        for (x, y in a, each(a)) {
            t |> equal(x, y)
        }
        for (x, y in a, to_array(unsafe(each(a)))) {
            t |> equal(x, y)
        }
    }
}

[test(description = "lock and lock_data expose the array's own elements for reading and writing, and an element address taken under lock writes through to the array")]
def test_lock_array(t : T?) {
    let arr <- [for (x in range(10)); x]
    lock(arr) $(tarr) {
        for (a, b in arr, tarr) {
            t |> equal(a, b)
        }
    }
    lock_data(arr) $(data, _size) {
        for (j in range(10)) {
            t |> equal(unsafe(data[j]), j)
        }
    }
    var arr2 <- [for (x in range(10)); x]
    lock_data(arr2) $(data, _size) {
        for (j in range(10)) {
            unsafe(data[j]) = j * 2
        }
    }
    for (j in range(10)) {
        t |> equal(arr2[j], j * 2)
    }
    lock(arr2) $(tarr2) {
        for (a, b in arr2, tarr2) {
            t |> equal(a, b)
        }
    }
    // address of a temp, and pass to implicit
    var farr : array<TestObjectFoo>
    push(farr, TestObjectFoo(uninitialized))
    lock(farr) $(ftarr) {
        var q = addr(ftarr[0])
        set_foo_data(q, 1234)
    }
    t |> equal(farr[0].fooData, 1234)
}

[test(description = "map_to_array views raw memory as an array of float whose writes land in the underlying uint buffer")]
def test_map_to_a(t : T?) {
    unsafe {
        var arr : uint[10]
        var pdata : void? = addr(arr[0])
        let data_size = 10 * typeinfo sizeof(type<uint>)
        map_to_array(pdata, data_size) $(var foo : array<float>#) {
            for (x in range(0, 10)) {
                foo[x] = float(x)
            }
        }
        for (x in range(0, 10)) {
            let val = reinterpret<float>(arr[x])
            t |> equal(val, float(x))
        }
    }
}

let {
    DIM_N = 2
    DIM_M = 3
}

var {
    g_dim_a : float[DIM_N][DIM_M]
}

[test(description = "a two-dimensional global fixed array reads back what was written, is laid out row-major under pointer arithmetic, and prints as nested brackets")]
def test_fixed_array_2d(t : T?) {
    t |> run("write and read 2D dim") @(t : T?) {
        for (y in range(2)) {
            for (x in range(3)) {
                let f = float(y * 10 + x)
                g_dim_a[y][x] = f
            }
        }
        for (y in range(2)) {
            for (x in range(3)) {
                let f = float(y * 10 + x)
                t |> equal(g_dim_a[y][x], f)
            }
        }
    }
    t |> run("pointer arithmetic over dim") @(t : T?) {
        for (y in range(2)) {
            for (x in range(3)) {
                g_dim_a[y][x] = float(y * 10 + x)
            }
        }
        unsafe {
            var pa = addr(g_dim_a[0][0])
            for (y in range(2)) {
                for (x in range(3)) {
                    let o = x + y * 3
                    let f = float(y * 10 + x)
                    t |> equal(pa[o], f)
                }
            }
        }
    }
    t |> run("dim to string") @(t : T?) {
        for (y in range(2)) {
            for (x in range(3)) {
                g_dim_a[y][x] = float(y * 10 + x)
            }
        }
        t |> equal("{g_dim_a}", "[ [ 0, 1, 2], [ 10, 11, 12]]")
        let a0 = g_dim_a[0]
        t |> equal("{a0}", "[ 0, 1, 2]")
        let a1 = g_dim_a[1]
        t |> equal("{a1}", "[ 10, 11, 12]")
    }
}

[test(description = "a pointer indexes the memory it points into with int, uint, int64 and uint64 indices alike")]
def test_pointer_index(t : T?) {
    t |> run("pointer index int") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        var val = unsafe(p[0])
        t |> equal(val, 10)
        val = unsafe(p[2])
        t |> equal(val, 30)
    }
    t |> run("pointer index uint") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        var val = unsafe(p[0u])
        t |> equal(val, 10)
        val = unsafe(p[2u])
        t |> equal(val, 30)
    }
    t |> run("pointer index int64") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        var val = unsafe(p[0l])
        t |> equal(val, 10)
        val = unsafe(p[2l])
        t |> equal(val, 30)
    }
    t |> run("pointer index uint64") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        var val = unsafe(p[0ul])
        t |> equal(val, 10)
        val = unsafe(p[2ul])
        t |> equal(val, 30)
    }
}

[test(description = "a dynamic array indexes with int, uint, int64 and uint64 indices alike")]
def test_array_index(t : T?) {
    t |> run("array index int") @(t : T?) {
        let ar <- [10, 20, 30]
        t |> equal(ar[0], 10)
        t |> equal(ar[2], 30)
    }
    t |> run("array index uint") @(t : T?) {
        let ar <- [10, 20, 30]
        t |> equal(ar[0u], 10)
        t |> equal(ar[2u], 30)
    }
    t |> run("array index int64") @(t : T?) {
        let ar <- [10, 20, 30]
        t |> equal(ar[0l], 10)
        t |> equal(ar[2l], 30)
    }
    t |> run("array index uint64") @(t : T?) {
        let ar <- [10, 20, 30]
        t |> equal(ar[0ul], 10)
        t |> equal(ar[2ul], 30)
    }
}

[test(description = "a fixed array indexes with int, uint, int64 and uint64 indices alike")]
def test_fixed_array_index(t : T?) {
    t |> run("fixed array index int") @(t : T?) {
        let di = fixed_array(10, 20, 30)
        t |> equal(di[0], 10)
        t |> equal(di[2], 30)
    }
    t |> run("fixed array index uint") @(t : T?) {
        let di = fixed_array(10, 20, 30)
        t |> equal(di[0u], 10)
        t |> equal(di[2u], 30)
    }
    t |> run("fixed array index int64") @(t : T?) {
        let di = fixed_array(10, 20, 30)
        t |> equal(di[0l], 10)
        t |> equal(di[2l], 30)
    }
    t |> run("fixed array index uint64") @(t : T?) {
        let di = fixed_array(10, 20, 30)
        t |> equal(di[0ul], 10)
        t |> equal(di[2ul], 30)
    }
}

[test(description = "an int3 vector indexes its components with int and uint indices")]
def test_vector_index(t : T?) {
    t |> run("vector index int") @(t : T?) {
        let vec = int3(10, 20, 30)
        t |> equal(vec[0], 10)
        t |> equal(vec[2], 30)
    }
    t |> run("vector index uint") @(t : T?) {
        let vec = int3(10, 20, 30)
        t |> equal(vec[0u], 10)
        t |> equal(vec[2u], 30)
    }
}

[test(description = "a table indexes its existing entries by string key and by int key")]
def test_table_index(t : T?) {
    t |> run("table index string key") @(t : T?) {
        var ta <- {"a" => 10, "b" => 20}
        t |> equal(ta["a"], 10)
        t |> equal(ta["b"], 20)
    }
    t |> run("table index int key") @(t : T?) {
        var ta <- {1 => 10, 2 => 20}
        t |> equal(ta[1], 10)
        t |> equal(ta[2], 20)
    }
}

[test(description = "indexing a pointer to int2 elements returns the whole element at that offset")]
def test_struct_pointer_index(t : T?) {
    t |> run("struct pointer index") @(t : T?) {
        var ar = fixed_array(int2(1, 2), int2(3, 4), int2(5, 6))
        var p : int2? = unsafe(addr(ar[0]))
        let val = unsafe(p[1])
        t |> equal(val.x, 3)
        t |> equal(val.y, 4)
    }
}

def test_erase_if(t : T?; var arr : array<int>) {
    let cloned := arr
    var maxVal = -1

    for (v in arr) {
        maxVal = max(maxVal, v)
    }

    arr |> erase_if() $(v : int) {
        return v < 0
    }

    var ok = true
    for (v in arr) {
        if (v < 0) {
            ok = false
        }
    }

    if (length(arr) != maxVal + 1) {
        ok = false
    }

    t |> success(ok)
}

[test(description = "erase_if dropping negatives leaves exactly the non-negative elements, for empty, all-negative and mixed arrays with negative runs at either end and in the middle")]
def test_erase_if_all(t : T?) {
    var t_base : array<int>
    test_erase_if(t, t_base)
    var t0 <- array(0)
    test_erase_if(t, t0)
    var t1 <- array(0, 1)
    test_erase_if(t, t1)
    var t2 <- array(-1)
    test_erase_if(t, t2)
    var t3 <- array(-1, -2)
    test_erase_if(t, t3)
    var t4 <- array(0, -9, 1)
    test_erase_if(t, t4)
    var t5 <- array(-9, -9, 0)
    test_erase_if(t, t5)
    var t6 <- array(-9, 0, -9)
    test_erase_if(t, t6)
    var t7 <- array(0, -9, -9)
    test_erase_if(t, t7)
    var t8 <- array(0, -9, 1, -9)
    test_erase_if(t, t8)
    var t9 <- array(0, -9, 1, 2, -9)
    test_erase_if(t, t9)
    var t10 <- array(0, -9, -5, 1, 2, -9)
    test_erase_if(t, t10)
    var t11 <- array(0, -9, -5, 1, 2, -9, -5)
    test_erase_if(t, t11)
    var t12 <- array(-5, 0, -9, -5, 1, 2, -9, -5)
    test_erase_if(t, t12)
    var t13 <- array(-6, -5, 0, -9, -5, 1, 2, -9, -5)
    test_erase_if(t, t13)
    var t14 <- array(0, -1, 1, -1, -2, 2, 3, 4, -6, -6, 5, 6, 7, -5, -5, -5, -5, 8, 9, -5, -5, -5,
                     -5, -5)
    test_erase_if(t, t14)
    var t15 <- array(0, -1, 1, -1, -2, 2, 3, 4, -6, -6, 5, 6, 7, -5, -5, -5, -5, 8, 9, -5, -5, -5,
                     -5, -5, 10)
    test_erase_if(t, t15)
}

struct ResizeFoo {
    x : int = 7
    y : int = 3
}

[test(description = "resize constructs grown struct elements with their field initializers, through the int64 overload too and again after a shrink and regrow, while grown int elements are zero")]
def test_resize_inits(t : T?) {
    t |> run("resize constructs new elements") @(t : T?) {
        var inscope arr : array<ResizeFoo>
        arr |> resize(3)
        t |> equal(arr[0].x, 7)
        t |> equal(arr[2].x, 7)
        t |> equal(arr[2].y, 3)
    }
    t |> run("resize int64 constructs too") @(t : T?) {
        var inscope arr : array<ResizeFoo>
        arr |> resize(int64(3))
        t |> equal(arr[2].x, 7)
    }
    t |> run("shrink then grow re-initializes the regrown range") @(t : T?) {
        var inscope arr : array<ResizeFoo>
        arr |> resize(5)
        arr[4].x = 99
        arr |> resize(2)
        arr |> resize(6)
        t |> equal(arr[4].x, 7)
        t |> equal(arr[5].x, 7)
    }
    t |> run("trivial elements still zero") @(t : T?) {
        var inscope arr : array<int>
        arr |> resize(4)
        t |> equal(arr[3], 0)
    }
}

[test(description = "resize_and_init zeroes trivial elements, fills them with an explicit initial value, and runs the field initializers of a struct element")]
def test_resize_and_init(t : T?) {
    t |> run("trivial type zeroes without a loop") @(t : T?) {
        var inscope arr : array<int>
        arr |> resize_and_init(3)
        t |> equal(arr[2], 0)
    }
    t |> run("initValue overload") @(t : T?) {
        var inscope arr : array<int>
        arr |> resize_and_init(3, 42)
        t |> equal(arr[0], 42)
        t |> equal(arr[2], 42)
    }
    t |> run("init-carrying type") @(t : T?) {
        var inscope arr : array<ResizeFoo>
        arr |> resize_and_init(2)
        t |> equal(arr[1].x, 7)
    }
}

def test_sort(t : T?; var arr : auto(TT)) {
    sort(arr)
    let len = length(arr)
    for (i in range(1, len)) {
        t |> success(arr[i - 1] < arr[i])
    }
    sort(arr, $(a, b) => b < a) // nolint:STYLE047
    for (i in range(1, len)) {
        t |> success(!(arr[i - 1] < arr[i]))
    }
}

def test_sort_dupes(t : T?; var arr : auto(TT); expected_sum : auto(SS)) {
    let len = length(arr)
    sort(arr)
    var asc_sum = default<SS>
    for (v in arr) {
        asc_sum += SS(v)
    }
    t |> equal(asc_sum, expected_sum)
    for (i in range(1, len)) {
        t |> success(!(arr[i] < arr[i - 1]))
    }
    sort(arr, $(a, b) => a > b) // nolint:STYLE047
    var desc_sum = default<SS>
    for (v in arr) {
        desc_sum += SS(v)
    }
    t |> equal(desc_sum, expected_sum)
    for (i in range(1, len)) {
        t |> success(!(arr[i - 1] < arr[i]))
    }
}

def test_sort_comp(t : T?; var arr : auto(TT)) {
    let len = length(arr)
    sort(arr) $(a, b) { // nolint:STYLE047
        return a.x < b.x
    }
    for (i in range(1, len)) {
        t |> success(arr[i - 1].x < arr[i].x)
    }
}

struct SortFoo {
    x, y : int
}

def operator <(a, b : SortFoo) => a.x + a.y < b.x + b.y

def test_vector_sort(t : T?) {
    var arr <- [float2(4, 0), float2(3, 1), float2(5, 2)]
    test_sort_comp(t, arr)
}

[test(description = "sort orders numeric, string, user-operator struct and vector arrays and fixed arrays ascending by default and descending under a comparator, and keeps every element when keys repeat")]
def test_sort_all(t : T?) {
    // numeric
    test_sort(t, fixed_array<int>(3, 2, 4, 1))
    test_sort(t, array<int>(3, 2, 4, 1))
    test_sort(t, fixed_array<uint>(3u, 2u, 4u, 1u))
    test_sort(t, array<uint>(3u, 2u, 4u, 1u))
    test_sort(t, fixed_array<int64>(3l, 2l, 4l, 1l))
    test_sort(t, array<int64>(3l, 2l, 4l, 1l))
    test_sort(t, fixed_array<uint64>(3ul, 2ul, 4ul, 1ul))
    test_sort(t, array<uint64>(3ul, 2ul, 4ul, 1ul))
    test_sort(t, fixed_array<float>(3.0, 2.0, 4.0, 1.0))
    test_sort(t, array<float>(3.0, 2.0, 4.0, 1.0))
    test_sort(t, fixed_array<double>(3.0lf, 2.0lf, 4.0lf, 1.0lf))
    test_sort(t, array<double>(3.0lf, 2.0lf, 4.0lf, 1.0lf))
    // string
    test_sort(t, fixed_array<string>("3.0lf", "2.0lf", "4.0lf", "1.0lf"))
    test_sort(t, array<string>("3.0lf", "2.0lf", "4.0lf", "1.0lf"))
    // custom type
    test_sort(t, [SortFoo(x=1, y=2), SortFoo(x=1, y=1), SortFoo(x=2, y=2), SortFoo(x=0, y=1)])
    test_sort(t, [SortFoo(x=1, y=2), SortFoo(x=1, y=1), SortFoo(x=2, y=2), SortFoo(x=0, y=1)])
    // vector
    test_vector_sort(t)
    // duplicates -- exercises strict-weak-ordering comparator on equal
    // elements
    test_sort_dupes(t, array<int>(5, 2, 5, 3, 2, 1, 4, 2, 5), 29l)
    test_sort_dupes(t, fixed_array<int>(5, 2, 5, 3, 2, 1, 4, 2, 5), 29l)
    test_sort_dupes(t, array<float>(1.5, 2.5, 1.5, 2.5, 3.5), 11.5lf)
}

struct Kv {
    key : int
    seq : int   // original position — observes stability
}

// stable_sort by key: equal keys must keep their input order
// (ascending seq).
def test_stable(t : T?; var arr : array<Kv>) {
    let len = length(arr)
    for (i in range(len)) {
        arr[i].seq = i
    }
    stable_sort(arr) $(a, b : Kv) : bool { return a.key < b.key; } // nolint:STYLE047
    for (i in range(1, len)) {
        t |> success(!(arr[i].key < arr[i - 1].key))                                  // sorted
        if (arr[i - 1].key == arr[i].key) {
            t |> success(arr[i - 1].seq < arr[i].seq)                                  // stable
        }
    }
}

[test(description = "stable_sort keeps equal keys in input order across short and long runs, and the bare form sorts arrays, fixed arrays and strings")]
def test_stable_sort_all(t : T?) {
    // stability: many duplicate keys (a reverse-leaning permutation to
    // exercise run reversal)
    test_stable(t, [Kv(key = 3), Kv(key = 1), Kv(key = 3), Kv(key = 2),
                    Kv(key = 1), Kv(key = 3), Kv(key = 2), Kv(key = 1),
                    Kv(key = 2), Kv(key = 3), Kv(key = 1), Kv(key = 2)])
    // larger run to cross the MINRUN/galloping boundary, still stable
    var big <- [for (i in range(200)); Kv(key = (i * 7) % 5)]
    test_stable(t, big)
    // bare form correctness — array + dim + string
    var ints = array<int>(5, 2, 5, 3, 2, 1, 4, 2, 5)
    stable_sort(ints)
    for (i in range(1, length(ints))) {
        t |> success(!(ints[i] < ints[i - 1]))
    }
    var dim = fixed_array<int>(4, 1, 4, 2, 1, 3)
    stable_sort(dim) $(a, b : int) : bool { return a < b; } // nolint:STYLE047
    for (i in range(1, length(dim))) {
        t |> success(!(dim[i] < dim[i - 1]))
    }
    var strs = array<string>("b", "a", "b", "c", "a")
    stable_sort(strs)
    for (i in range(1, length(strs))) {
        t |> success(!(strs[i] < strs[i - 1]))
    }
}

// A user struct element routes the heap and nth_element calls through
// the "any" sort path (builtin_*_array_any_cblock); int arrays take
// the typed path.
struct private Item {
    key : int
    tag : int
}

[test(description = "make_heap and pop_heap on a struct array keyed by cmp_by keep the largest key at the root with its payload and pop the keys in descending order")]
def test_any_path_make_pop_heap(t : T?) {
    var a <- [
        Item(key = 5, tag = 50), Item(key = 2, tag = 20), Item(key = 8, tag = 80),
        Item(key = 1, tag = 10), Item(key = 9, tag = 90), Item(key = 3, tag = 30),
        Item(key = 7, tag = 70)
    ]
    make_heap(a) <| cmp_by(_.key)
    t |> equal(a[0].key, 9)      // max-key at root
    t |> equal(a[0].tag, 90)     // payload travels with the key
    var popped : array<int>
    while (!empty(a)) {
        pop_heap(a) <| cmp_by(_.key)
        popped |> push(a[length(a) - 1].key)
        a |> resize(length(a) - 1)
    }
    t |> equal(length(popped), 7)
    for (i in range(1, length(popped))) {
        t |> success(popped[i] <= popped[i - 1])   // descending
    }
    delete popped
}

[test(description = "push_heap on a struct heap moves a newly pushed largest key to the root together with its payload")]
def test_any_path_push_heap(t : T?) {
    var a <- [Item(key = 9, tag = 90), Item(key = 5, tag = 50), Item(key = 7, tag = 70),
              Item(key = 1, tag = 10)]
    make_heap(a) <| cmp_by(_.key)
    a |> push(Item(key = 12, tag = 120))
    push_heap(a) <| cmp_by(_.key)
    t |> equal(a[0].key, 12)
    t |> equal(a[0].tag, 120)
}

[test(description = "nth_element on a struct array puts the fifth-smallest key at index 4 with no larger key before it and no smaller key after it")]
def test_any_path_nth_element(t : T?) {
    var a <- [
        Item(key = 7, tag = 70), Item(key = 3, tag = 30), Item(key = 1, tag = 10),
        Item(key = 9, tag = 90),
        Item(key = 5, tag = 50), Item(key = 8, tag = 80), Item(key = 0, tag = 0),
        Item(key = 4, tag = 40), Item(key = 2, tag = 20)
    ]
    nth_element(a, 4) <| cmp_by(_.key)
    t |> equal(a[4].key, 4)   // 5th-smallest key sits at index 4
    for (i in range(0, 4)) {
        t |> success(a[i].key <= a[4].key)
    }
    for (i in range(5, length(a))) {
        t |> success(a[i].key >= a[4].key)
    }
}

var g_a_steps = 0
var g_b_steps = 0

[coroutine]
def step_co_a() {
    g_a_steps ++
    co_continue()
    g_a_steps ++
}

[coroutine]
def step_co_b() {
    g_b_steps ++
    co_continue()
    g_b_steps ++
    co_continue()
    g_b_steps ++
}

def make_iota() : int[4] {
    var r : int[4]
    for (i in range(4)) {
        r[i] = i + 1
    }
    return r
}

struct IntsPair {
    comprehended : array<int>
    moved : array<int>
}

variant IntsOrTag {
    ints : array<int>
    tag : int
}

[test(description = "coroutine iterators moved into an array literal arrive live and each runs to completion under cr_run_all")]
def test_literal_iterator_elements(t : T?) {
    t |> run("moved-in iterator elements arrive live") @(t : T?) {
        g_a_steps = 0
        g_b_steps = 0
        var tasks <- [ <- step_co_a(), <- step_co_b()]
        cr_run_all(tasks)
        t |> equal(g_a_steps, 2)
        t |> equal(g_b_steps, 3)
    }
}

[test(description = "fixed arrays returned by calls keep their contents as the elements of an array literal")]
def test_literal_fixed_array_elements(t : T?) {
    t |> run("fixed-array elements carry their contents") @(t : T?) {
        var arr <- [make_iota(), make_iota()]
        var total = 0
        for (fa in arr) {
            for (x in fa) {
                total += x
            }
        }
        t |> equal(total, 20)
        delete arr
    }
}

[test(description = "a comprehension and a moved array passed as struct constructor fields arrive with all their elements")]
def test_ctor_comprehension_move_init(t : T?) {
    t |> run("a comprehension moved into a constructor argument keeps its elements") @(t : T?) {
        var src <- [1, 2, 3]
        var other <- [7, 8]
        var pair = IntsPair(comprehended <- [for (x in src); x * 10], moved <- other)
        t |> equal(length(pair.comprehended), 3)
        t |> equal(pair.comprehended[0], 10)
        t |> equal(pair.comprehended[2], 30)
        t |> equal(length(pair.moved), 2)
        t |> equal(pair.moved[1], 8)
        delete pair
        delete src
    }
}

[test(description = "comprehension elements land intact inside array, tuple and variant literals")]
def test_comprehension_literal_elements(t : T?) {
    t |> run("comprehension elements land intact in an array literal") @(t : T?) {
        var src <- [1, 2, 3]
        var arr <- [ [for(x in src); x * 10], [for (x in src); x * 100]]
        t |> equal(length(arr), 2)
        t |> equal(length(arr[0]), 3)
        t |> equal(length(arr[1]), 3)
        t |> equal(arr[0][2], 30)
        t |> equal(arr[1][2], 300)
        delete arr
        delete src
    }
    t |> run("a comprehension element lands intact in a tuple literal") @(t : T?) {
        var src <- [1, 2, 3]
        var tup <- ([for (x in src); x * 10], 7)
        t |> equal(length(tup._0), 3)
        t |> equal(tup._0[1], 20)
        t |> equal(tup._1, 7)
        delete tup
        delete src
    }
    t |> run("a comprehension element lands intact in a variant literal") @(t : T?) {
        var src <- [1, 2, 3]
        var v <- IntsOrTag(ints <- [for (x in src); x * 10])
        t |> success(v is ints, "the variant holds the ints alternative")
        t |> equal(length(v as ints), 3)
        t |> equal((v as ints)[0], 10)
        delete v
        delete src
    }
}
