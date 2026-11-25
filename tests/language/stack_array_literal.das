options gen2
options persistent_heap
options force_inscope_pod
options no_coverage

require dastest/testing_boost public
require strings

var g_no_fold = 0

struct Wide {
    cells : int[256]
}

[never_inline]
def any_suffix(word : string; suffixes : array<string>) : bool {
    for (suffix in suffixes) {
        return true if (word |> ends_with(suffix))
    }
    return false
}

[never_inline]
def total(xs : array<int>) : int {
    var sum = 0
    for (x in xs) {
        sum += x
    }
    return sum
}

[never_inline]
def total_of_two(xs, ys : array<int>) : int => total(xs) * 100 + total(ys)

[never_inline]
def make_wide(v : int) : Wide {
    var w : Wide
    for (c in w.cells) {
        c = v
    }
    return w
}

[never_inline]
def total_and_wide(xs : array<int>; w : Wide) : int => total(xs) + w.cells[0]

[never_inline]
def push_one(var xs : array<int>) : int {
    xs |> push(1)
    return length(xs)
}

[never_inline]
def pair_from(v : int) : array<int> => <- [v, v + 1]

[never_inline]
def total_of_all(rows : array<array<int>>) : int {
    var sum = 0
    for (row in rows) {
        sum += total(row)
    }
    return sum
}

[test(description = "with force_inscope_pod on, a loop over an array literal of numbers or strings still allocates nothing and reads every element in order")]
def test_literal_loop_source(t : T?) {
    g_no_fold = 0
    t |> run("numbers") @@(t) {
        var sum = 0
        var digits = 0
        let before = heap_bytes_allocated()
        for (i in range(100)) {
            for (x in [i, 2, 3, g_no_fold]) {
                sum += x
            }
        }
        for (x in [1, 2 + g_no_fold, 3]) {
            digits = digits * 10 + x
        }
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 4950 + 500)
        t |> equal(digits, 123)
    }
    t |> run("strings") @@(t) {
        var letters = 0
        let word = "wide"
        let before = heap_bytes_allocated()
        for (_ in range(100)) {
            for (s in ["a", "bc", word]) {
                letters += length(s)
            }
        }
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(letters, 700)
    }
}

[test(description = "an array literal passed to a constant array parameter allocates nothing, reads right in the callee, and survives a sibling argument that builds a temporary of its own")]
def test_literal_constant_argument(t : T?) {
    g_no_fold = 0
    t |> run("a literal of strings") @@(t) {
        var hits = 0
        let before = heap_bytes_allocated()
        for (_ in range(100)) {
            if (any_suffix("hello", ["xy", "lo"])) {
                hits++
            }
            if (any_suffix("hello", ["xy", "he"])) {
                hits += 1000
            }
        }
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(hits, 100)
    }
    t |> run("a sibling argument with its own temporary") @@(t) {
        let before = heap_bytes_allocated()
        let sum = total_and_wide([1 + g_no_fold, 2, 3, 4], make_wide(100))
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 110)
    }
    t |> run("two literals in one call") @@(t) {
        let before = heap_bytes_allocated()
        let sum = total_of_two([1 + g_no_fold, 2], [3, 4, 5])
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 312)
    }
    t |> run("a builtin generic") @@(t) {
        let before = heap_bytes_allocated()
        let at = find_index([10, 20, 30], 20 + g_no_fold)
        let missing = find_index([10, 20, 30], 25 + g_no_fold)
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(at, 1)
        t |> equal(missing, -1)
    }
}

[test(description = "a literal that binds to a var parameter, whose elements own heap, or whose callee returns an iterator stays a heap array and still reads right")]
def test_literal_left_on_the_heap(t : T?) {
    g_no_fold = 0
    t |> run("a var parameter grows the literal") @@(t) {
        t |> equal(push_one([1 + g_no_fold, 2, 3]), 4)
    }
    t |> run("elements that own heap are collected after the loop") @@(t) {
        var sum = 0
        let before = heap_bytes_allocated()
        for (row in [pair_from(1), pair_from(3 + g_no_fold)]) {
            sum += total(row)
        }
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 10)
    }
    t |> run("elements that own heap, as an argument") @@(t) {
        t |> equal(total_of_all([pair_from(1), pair_from(3 + g_no_fold)]), 10)
    }
    t |> run("an iterator over the literal") @@(t) {
        var sum = 0
        for (x in each([1 + g_no_fold, 2, 3])) {
            sum += x
        }
        t |> equal(sum, 6)
    }
}

[never_inline]
def total_beside_unsafe() : int {
    let seen = unsafe(addr(g_no_fold))
    var sum = *seen
    for (x in [1, 2, 3]) {
        sum += x
    }
    return sum + total([4 + g_no_fold, 5])
}

[never_inline]
def total_beside_recover() : int {
    var sum = 0
    try {
        for (x in [1, 2, 3]) {
            sum += x
        }
        sum += total([4 + g_no_fold, 5])
    } recover {
        sum = -1
    }
    return sum
}

[test(description = "the stack literal applies in a function that has unsafe or try/recover and in a lambda body, and a generator still yields every element of its literal")]
def test_literal_beside_unsafe_recover_lambda_generator(t : T?) {
    g_no_fold = 0
    t |> run("a function with unsafe") @@(t) {
        let before = heap_bytes_allocated()
        let sum = total_beside_unsafe()
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 15)
    }
    t |> run("a function with try/recover") @@(t) {
        let before = heap_bytes_allocated()
        let sum = total_beside_recover()
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 15)
    }
    t |> run("a lambda body") @@(t) {
        var add <- @(v : int) : int {
            var sum = total([v, 1, 2])
            for (x in [3, 4]) {
                sum += x
            }
            return sum
        }
        let before = heap_bytes_allocated()
        var sum = 0
        for (i in range(10)) {
            sum += invoke(add, i)
        }
        t |> equal(heap_bytes_allocated(), before)
        t |> equal(sum, 45 + 100)
        unsafe {
            delete add
        }
    }
    t |> run("a generator") @@(t) {
        var numbers <- generator<int>() <| $() {   // nolint:STYLE002 piped block only
            for (x in [1, 2, 3 + g_no_fold]) {
                yield x
            }
            return false
        }
        var sum = 0
        for (x in numbers) {
            sum += x
        }
        t |> equal(sum, 6)
    }
}
