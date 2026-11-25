options gen2
options no_coverage
options persistent_heap = true

require strings
require UnitTest
require dastest/testing_boost public

struct SomethingSmall {
    a, b, c : int
}

def test_delete_array(t : T?) {
    var a : array<int>[5]
    let w0 = heap_bytes_allocated()
    for (ii in range(5)) {
        resize(a[ii], 10)
    }
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    t |> success(w1 > w0 && w0 == w2)
    for (ii in range(5)) {
        t |> success(empty(a[ii]) && capacity(a[ii]) == 0)
    }
}

def test_delete_table(t : T?) {
    var a : table<int; int>
    let w0 = heap_bytes_allocated()
    a |> insert(0, 1)
    a |> insert(1, 2)
    let w1 = heap_bytes_allocated()
    delete a
    let w2 = heap_bytes_allocated()
    t |> success(w1 > w0 && w0 == w2)
    t |> success(empty(a) && capacity(a) == 0)
}

def test_delete_struct(t : T?) {
    var a : SomethingSmall?
    let w0 = heap_bytes_allocated()
    a = new SomethingSmall
    let w1 = heap_bytes_allocated()
    unsafe {
        delete a
    }
    let w2 = heap_bytes_allocated()
    t |> success(w1 > w0 && w0 == w2)
    t |> equal(a, null)
}

def test_delete_struct_array(t : T?) {
    let w0 = heap_bytes_allocated()
    var a = new <SomethingSmall[10]>
    let w1 = heap_bytes_allocated()
    unsafe {
        delete a
    }
    let w2 = heap_bytes_allocated()
    t |> success(w1 > w0 && w0 == w2)
    for (pA in a) {
        t |> equal(pA, null)
    }
}

def test_delete_handle(t : T?) {
    var a = new TestObjectFoo
    unsafe {
        delete a
    }
    t |> equal(a, null)
}

def test_delete_handle_array(t : T?) {
    var a  = new <TestObjectFoo[10]>
    unsafe {
        delete a
    }
    for (pA in a) {
        t |> equal(pA, null)
    }
}

[sideeffects]
def da_inscope(x) {
    var b = new SomethingSmall
    var a = new <SomethingSmall[10]>
    b.a = x
    for (pA in a) {
        pA.a = x
    }
} finally {
    unsafe {
        delete a
        delete b
    }
}

def test_delete_struct_array_in_scope(t : T?) {
    let w0 = heap_bytes_allocated()
    da_inscope(2)
    let w1 = heap_bytes_allocated()
    t |> equal(w0, w1)
}

[sideeffects]
def test_delete_string(t : T?) {
    let w0 = heap_bytes_allocated()
    var a = "{deref(getPtr())}"
    // assert(a=="0")
    unsafe(delete_string(a))
    let w1 = heap_bytes_allocated()
    t |> equal(w0, w1)
    t |> equal(a, "")
}

[sideeffects]
def test_delete_string_array(t : T?) {
    let w0 = heap_bytes_allocated()
    var a = fixed_array<string>("{deref(getPtr())}_1", "{deref(getPtr())}_2", "{deref(getPtr())}_3")
    for (AA in a) {
        unsafe(delete_string(AA))
    }
    let w1 = heap_bytes_allocated()
    t |> equal(w0, w1)
}

[test(description = "delete frees arrays, tables, structs, struct and handle pointer arrays and strings back to the heap byte count before allocation, and nulls or empties what it freed")]
def test_new_delete(t : T?) {
    test_delete_struct_array_in_scope(t)
    test_delete_handle(t)
    test_delete_handle_array(t)
    test_delete_struct(t)
    test_delete_struct_array(t)
    test_delete_array(t)
    test_delete_table(t)
    test_delete_string(t)
    test_delete_string_array(t)
}

var g_loop_literal = 1

struct LoopLiteralWide {
    cells : int[100]
}

[never_inline]
def loop_literal_wide(v : int) : LoopLiteralWide {
    var w : LoopLiteralWide
    w.cells[0] = v
    return w
}

def loop_literal_yields() : iterator<int> {
    return <- generator<int>() <| $() {   // nolint:STYLE002 piped block only
        for (x in [g_loop_literal, 2, 3]) {
            yield x
        }
        return false
    }
}

[test(description = "a loop over an array literal whose elements own no heap allocates nothing, with force_inscope_pod off, for numbers, strings, two sources and a comprehension's source; a literal past 1024 bytes stays a heap array, and that one, one whose elements own heap and one a generator yields from read every element in order")]
def test_array_literal_loop_source(t : T?) {
    var sum = 0
    var letters = 0
    var dot = 0
    let before = heap_bytes_allocated()
    for (i in range(100)) {
        for (x in [i, 2, 3, g_loop_literal]) {
            sum += x
        }
    }
    for (s in ["a", "bc", "d{g_loop_literal}"]) {
        letters += length(s)
    }
    for (x, y in [g_loop_literal, 2, 3], [10, 20, 30]) {
        dot += x * y
    }
    t |> equal(heap_bytes_allocated(), before, "the literals are stack arrays")
    t |> equal(sum, 4950 + 600)
    t |> equal(letters, 5)
    t |> equal(dot, 140)
    let from_fixed <- [for (x in fixed_array(g_loop_literal, 2, 3)); x * 2]
    let fixed_bytes = heap_bytes_allocated() - before
    let doubled <- [for (x in [g_loop_literal, 2, 3]); x * 2]
    t |> equal(heap_bytes_allocated() - before, fixed_bytes * 2ul,
        "a comprehension over the literal allocates what one over a fixed array does")
    t |> equal(doubled[0] + doubled[1] + doubled[2], 12)
    t |> equal(from_fixed[2], 6)
    var wide = 0
    let before_wide = heap_bytes_allocated()
    for (w in [loop_literal_wide(1), loop_literal_wide(2), loop_literal_wide(3)]) {
        wide = wide * 10 + w.cells[0]
    }
    t |> success(heap_bytes_allocated() > before_wide, "1200 bytes of literal stay on the heap")
    t |> equal(wide, 123)
    var rows = 0
    for (row in [[g_loop_literal, 2], [3, 4, 5]]) {
        rows = rows * 10 + length(row)
    }
    t |> equal(rows, 23)
    var yielded = 0
    for (x in loop_literal_yields()) {
        yielded = yielded * 10 + x
    }
    t |> equal(yielded, 123)
}

def loop_literal_sums_between_yields() : iterator<int> {
    return <- generator<int>() <| $() {   // nolint:STYLE002 piped block only
        var sum = 0
        for (x in [g_loop_literal, 2, 3]) {
            sum += x
        }
        yield sum
        for (x in [10, 20 + g_loop_literal]) {
            sum += x
        }
        yield sum
        return false
    }
}

def loop_literal_yields_nested() : iterator<int> {
    return <- generator<int>() <| $() {   // nolint:STYLE002 piped block only
        for (x in [g_loop_literal, 2]) {
            for (y in [10, 20, 30 + g_loop_literal]) {
                yield x * y
            }
        }
        return false
    }
}

def loop_literal_yields_pairs() : iterator<int> {
    return <- generator<int>() <| $() {   // nolint:STYLE002 piped block only
        for (x, y in [g_loop_literal, 2, 3], [100, 200, 300]) {
            yield x + y
        }
        return false
    }
}

[test(description = "a generator reads an array literal loop source right when the loop yields, when it runs between two yields, when two such loops nest, when one loop has two literal sources, and when two generators over literals are advanced in turn")]
def test_array_literal_loop_source_in_generator(t : T?) {
    let between <- [for (x in loop_literal_sums_between_yields()); x]
    t |> equal(length(between), 2)
    t |> equal(between[0], 6)
    t |> equal(between[1], 37)
    var nested = 0
    for (x in loop_literal_yields_nested()) {
        nested += x
    }
    t |> equal(nested, 10 + 20 + 31 + 20 + 40 + 62)
    var pairs = 0
    for (x in loop_literal_yields_pairs()) {
        pairs = pairs * 1000 + x
    }
    t |> equal(pairs, 101202303)
    var first <- loop_literal_yields()
    var second <- loop_literal_yields_nested()
    var a = 0
    var b = 0
    var c = 0
    t |> success(next(first, a) && next(second, b) && next(first, c))
    t |> equal(a * 10000 + b * 100 + c, 11002)
}
