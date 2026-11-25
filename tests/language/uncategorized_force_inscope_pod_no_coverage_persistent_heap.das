options gen2
options persistent_heap
options force_inscope_pod
options no_coverage

require dastest/testing_boost public

var g_no_fold = 0

struct Wide {
    cells : int[256]
}

[never_inline]
def wide_row(n : int) : array<Wide> {
    var row : array<Wide>
    row |> resize(n)
    for (i, w in count(), row) {
        w.cells[0] = i + 1
    }
    return <- row
}

[never_inline]
def total_of_row(n : int) : int {
    var sum = 0
    for (w in wide_row(n)) {
        sum += w.cells[0]
    }
    return sum
}

[test(description = "a loop over an array a function returns is collected after the loop and leaves the caller's locals as they were, when one element is larger than the array itself")]
def test_returned_loop_source_keeps_the_callers_locals(t : T?) {
    g_no_fold = 0
    var guards : int[64]
    for (g in guards) {
        g = 0x5a5a5a5a
    }
    let before = heap_bytes_allocated()
    let sum = total_of_row(3 + g_no_fold)
    t |> equal(heap_bytes_allocated(), before)
    t |> equal(sum, 6)
    var spoiled = 0
    for (g in guards) {
        spoiled += g == 0x5a5a5a5a ? 0 : 1
    }
    t |> equal(spoiled, 0)
}

[never_inline]
def twice(v : int) : int => v * 2

[never_inline]
def thrice(v : int) : int => v * 3

[test(description = "a local array of function pointers is collected when its scope ends, like any array that owns only its buffer")]
def test_function_pointer_array_is_collected(t : T?) {
    g_no_fold = 0
    var sum = 0
    let before = heap_bytes_allocated()
    for (_ in range(10)) {
        let steps <- [@@twice, @@thrice]
        for (step in steps) {
            sum += invoke(step, 1 + g_no_fold)
        }
    }
    t |> equal(heap_bytes_allocated(), before)
    t |> equal(sum, 50)
}
