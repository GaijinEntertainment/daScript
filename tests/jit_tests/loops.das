options gen2

require dastest/testing_boost
require daslib/faker
require daslib/constant_expression

[jit]
def string_for(t : T?) {
    for (c, i in "hello", count()) {
        if (i == 0) {
            t |> equal(c, 'h')
        } elif (i == 1) {
            t |> equal(c, 'e')
        } elif (i == 2 || i == 3) {
            t |> equal(c, 'l')
        } elif (i == 4) {
            t |> equal(c, 'o')
        } else {
            t |> failure("unexpected character")
        }
    }
}

[jit]
def range_for(value : int) {
    var i = 0
    for (_ in range(0, value)) {
        i ++
    }
    return i
}

[jit]
def range_for_range(value : range) {
    var i = 0
    for (_ in value) {
        i ++
    }
    return i
}

[jit]
def range_for_range_extra(value : range) {
    var i = 0
    for (_a, _b in value, range(13, 100500)) {
        i ++
    }
    return i
}

[jit]
def range_for_with_break(value, max_value : int) {
    var i = 0
    for (_ in range(0, value)) {
        if (i >= max_value) {
            break
        }
        i ++
    }
    return i
}

[jit]
def range_for_with_continue(value, max_value : int) {
    var i = 0
    var j = 0
    for (_ in range(0, value)) {
        i ++
        if (i >= max_value) {
            continue
        }
        j ++
    }
    return j
}

[jit]
def range_for_with_finally(value : int) {
    var i = 0
    var tf = 0
    for (_ in range(0, value)) {
        i ++
    } finally {
        tf ++
    }
    return tf
}

[jit]
def dim_for(var a : int[10]) {
    var count = 0
    for (j in a) {
        j = 13
        count ++
    }
    return count
}

[jit]
def dim_and_range_for(var a : int[10]) {
    var count = 0
    for (i, j in range(5), a) {
        j = i
        count ++
    }
    return count
}

[jit]
def array_for(var a : array<int>) {
    var count = 0
    for (j in a) {
        j = 13
        count ++
    }
    return count
}

[jit]
def array_and_range_for(var a : array<int>) {
    var count = 0
    for (i, j in range(5), a) {
        j = i
        count ++
    }
    return count
}

def gen_range(value : int) {
    return <- generator<int> {
        for (i in range(value)) {
            yield i
        }
        return false
    }
}

[jit]
def iterator_for(value : int) {
    var i = 0
    for (_ in gen_range(value)) {
        i ++
    }
    return i
}

[jit]
def hybrid_for(value : int) {
    var i = 0
    for (_a, _b in gen_range(value), range(7)) {
        i ++
    }
    return i
}

[jit]
def array_comp(value : int) => <- [for (x in range(0, value)); x]

[test(description = "a JIT-compiled array comprehension builds the same array as the interpreter")]
def test_comprehension(t : T?) {
    t |> run("comprehension") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@array_comp))
        var a <- array_comp(10)
        for (x, y in a, range(10)) {
            t |> equal(x, y)
        }
    }
}

[jit]
def add_elements(a : array<int>) {
    var summ = 0
    for (i in a) {
        summ += i
    }
    return summ
}

[jit]
def special_count(max, start, step : int) {
    var add = 0
    for (_i, j in range(max), count(start, step)) {
        add += j
    }
    return add
}

[test(description = "JIT-compiled for loops over strings, ranges, fixed arrays, arrays, iterators and mixed sources visit the interpreter's elements")]
def test_for_loop(t : T?) {
    t |> run("string for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@string_for))
        string_for(t)
    }
    t |> run("range for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@range_for))
        t |> success(!jit_enabled() || is_jit_function(@@range_for_range))
        t |> success(!jit_enabled() || is_jit_function(@@range_for_range_extra))
        t |> success(!jit_enabled() || is_jit_function(@@range_for_with_break))
        t |> success(!jit_enabled() || is_jit_function(@@range_for_with_continue))
        t |> success(!jit_enabled() || is_jit_function(@@range_for_with_finally))
        t |> equal(range_for(10), 10)
        t |> equal(range_for_range(range(100, 110)), 10)
        t |> equal(range_for_range_extra(range(100, 110)), 10)
        t |> equal(range_for_with_break(10, 5), 5)
        t |> equal(range_for_with_continue(10, 5), 4)
        // per-iter finally: finally fires once per iteration, so tf == 10 for range(10)
        t |> equal(range_for_with_finally(10), 10)
    }
    t |> run("dim for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@dim_for))
        t |> success(!jit_enabled() || is_jit_function(@@dim_and_range_for))
        var a : int[10]
        t |> equal(dim_for(a), 10)
        for (i in a) {
            t |> equal(i, 13)
        }
        t |> equal(dim_and_range_for(a), 5)
        for (i, j in a, range(10)) {
            t |> equal(i, j < 5 ? j : 13)
        }
    }
    t |> run("array for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@array_for))
        t |> success(!jit_enabled() || is_jit_function(@@array_and_range_for))
        var a : array<int>
        a |> resize(10)
        t |> equal(array_for(a), 10)
        for (i in a) {
            t |> equal(i, 13)
        }
        t |> equal(array_and_range_for(a), 5)
        for (i, j in a, range(10)) {
            t |> equal(i, j < 5 ? j : 13)
        }
    }
    t |> run("iterator for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@iterator_for))
        t |> equal(iterator_for(10), 10)
    }
    t |> run("hybrid for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@hybrid_for))
        t |> equal(7, hybrid_for(10))
        t |> equal(5, hybrid_for(5))
    }
    t |> run("empty for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@add_elements))
        var a <- array<int>(1, 2, 3)
        t |> equal(6, add_elements(a))
        a |> clear()
        t |> equal(0, add_elements(a))
    }
    t |> run("special count") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@special_count))
        t |> equal(3,  special_count(3, 0, 1))  // 0, 1, 2
        t |> equal(6,  special_count(3, 0, 2))  // 0, 2, 4
        t |> equal(12, special_count(3, 1, 3))  // 1, 4, 7
    }
}

def count_all(value : int) {
    var i = 0
    while (i < value) {
        i ++
    }
    return i
}

def count_with_break(value, max_value : int) {
    var i = 0
    while (i < value) {
        if (i >= max_value) {
            break
        }
        i ++
    }
    return i
}

def count_with_continue(value, max_value : int) {
    var i = 0
    var j = 0
    while (i < value) {
        i ++
        if (i >= max_value) {
            continue
        }
        j ++
    }
    return j
}

def count_with_finally(value : int) {
    var i = 0
    var tf = 0
    while (i < value) {
        i ++
    } finally {
        tf ++
    }
    return tf
}

[test(description = "JIT-compiled while loops count correctly through break, continue and a per-iteration finally")]
def test_while_loop(t : T?) {

    t |> success(!jit_enabled() || is_jit_function(@@count_all))
    t |> success(!jit_enabled() || is_jit_function(@@count_with_break))
    t |> success(!jit_enabled() || is_jit_function(@@count_with_continue))
    t |> success(!jit_enabled() || is_jit_function(@@count_with_finally))

    t |> equal(count_all(10), 10)
    t |> equal(count_with_break(10, 5), 5)
    t |> equal(count_with_continue(10, 5), 4)
    // per-iter finally: finally fires once per iteration, so tf == 10 for while (i < 10)
    t |> equal(count_with_finally(10), 10)
}

var g_label = -1

def test_label(a : int) {
    goto a
    label 0:
    g_label = 0
    return
    label 1:
    g_label = 1
    return
    label 2:
    g_label = 2
    return
    label 3:
    g_label = 3
}

[sideeffects]
def test_static_label(first : bool) {
    if (first) {
        goto 1
    } else {
        goto 2
    }
    label 1:
    g_label = 1
    return
    label 2:
    g_label = 2
    return
}

[test(description = "JIT-compiled regular, panicking and static labels jump to the same places as in the interpreter")]
def test_labels(t : T?) {
    t |> run("regular label") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_label))
        for (i in range(4)) {
            g_label = -1
            test_label(i)
            t |> equal(i, g_label)
        }
    }
    t |> run("panic label") @(t : T?) {
        var failed = false
        try {
            test_label(13)
        } recover {
            failed = true
        }
        t |> success(failed)
    }
    t |> run("static label") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_static_label))
        g_label = -1
        test_static_label(true)
        t |> equal(1, g_label)
        g_label = -1
        test_static_label(false)
        t |> equal(2, g_label)
    }
}

// Smoke tests for per-loop annotations on ExprFor / ExprWhile.
// These must parse and produce identical numeric results to their
// unannotated equivalents — the JIT lowering attaches LLVM loop
// metadata but does not change semantics.

def sum_range_bracketed(n : int) {
    var sum = 0
    for [unroll, vectorize] (i in range(1, n + 1)) {
        sum += i
    }
    return sum
}

def sum_range_metadata(n : int) {
    var sum = 0
    for @unroll_full(i in range(1, n + 1)) {
        sum += i
    }
    return sum
}

def sum_range_count(n : int) {
    var sum = 0
    for [unroll_count = 4] (i in range(1, n + 1)) {
        sum += i
    }
    return sum
}

def sum_range_bare(n : int) {
    var sum = 0
    for (i in range(1, n + 1)) {
        sum += i
    }
    return sum
}

def countdown_while_annotated(start : int) {
    var x = start
    var iters = 0
    while [unroll_disable] (x > 0) {
        x--
        iters++
    }
    return iters
}

def countdown_while_dont_unroll(start : int) {
    var x = start
    var iters = 0
    while [dont_unroll] (x > 0) {
        x--
        iters++
    }
    return iters
}

def countdown_while_bare(start : int) {
    var x = start
    var iters = 0
    while (x > 0) {
        x--
        iters++
    }
    return iters
}

[test(description = "a for or while loop carrying a bracketed, metadata or unroll hint computes the same result as the bare loop")]
def test_per_loop_hints(t : T?) {
    t |> run("for [bracketed] matches bare") @(t : T?) {
        t |> equal(sum_range_bracketed(100), sum_range_bare(100))
    }
    t |> run("for @metadata matches bare") @(t : T?) {
        t |> equal(sum_range_metadata(100), sum_range_bare(100))
    }
    t |> run("for [unroll_count = N] matches bare") @(t : T?) {
        t |> equal(sum_range_count(100), sum_range_bare(100))
    }
    t |> run("while [unroll_disable] matches bare") @(t : T?) {
        t |> equal(countdown_while_annotated(50), countdown_while_bare(50))
    }
    t |> run("while [dont_unroll] (the SPIR-V spelling) matches bare") @(t : T?) {
        t |> equal(countdown_while_dont_unroll(50), countdown_while_bare(50))
    }
}

[sideeffects]
def test_const_r64 => range64(1, 2)

[sideeffects]
def test_const_ur64 => urange64(3, 4)

[sideeffects]
def test_make_r64_from_int(a : int) => range64(a)

[sideeffects]
def test_make_r64_from_2xr64(a, b : int64) => range64(a, b)

[sideeffects]
def test_make_r64_from_2xfloat(a, b : float) => range64(a, b)

[sideeffects]
def test_r64_yx(r : range64) : range64 => r.yx

[sideeffects]
def test_for_range64 {
    var t = 0ul
    for (_x in 0ul .. 10ul) {
        t ++
    }
    return t
}

[test(description = "range64 and urange64 constants, constructors, swizzles and for-loops JIT to the interpreter's results")]
def test_range64_urange64_intrinsics(t : T?) {
    t |> run("constants") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_const_r64))
        t |> equal(range64(1, 2), test_const_r64())
        t |> success(!jit_enabled() || is_jit_function(@@test_const_ur64))
        t |> equal(urange64(3, 4), test_const_ur64())
    }
    t |> run("constructors") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_make_r64_from_int))
        t |> equal(range64(0, 123), test_make_r64_from_int(123))
        t |> success(!jit_enabled() || is_jit_function(@@test_make_r64_from_2xr64))
        let c1 = 1l << 32l
        let c2 = 1l << 33l
        t |> equal(range64(c1, c2), test_make_r64_from_2xr64(c1, c2))
        t |> success(!jit_enabled() || is_jit_function(@@test_make_r64_from_2xfloat))
        t |> equal(range64(1., 2.), test_make_r64_from_2xfloat(1., 2.))
    }
    t |> run("swizzle") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_r64_yx))
        t |> equal(range64(2, 1), test_r64_yx(range64(1, 2)))
    }
    t |> run("for") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_for_range64))
        t |> equal(10ul, test_for_range64())
    }
}

[test(description = "indexing and swizzling a urange64 vector returns the same components under JIT as the interpreter")]
def test_urange64_vector_access(t : T?) {
    t |> run("heap_allocation_stats index and swizzle") @(t : T?) {
        let hpbs = heap_allocation_stats()
        t |> equal(hpbs[0], heap_allocation_stats()[0])
        t |> equal(hpbs[1], heap_allocation_stats()[1])
        t |> equal(hpbs.x, heap_allocation_stats().x)
        t |> equal(hpbs.y, heap_allocation_stats().y)
    }
}

def each_array_sum(a : array<int>) : int {
    var s = 0
    for (x in each(a)) {
        s += x
    }
    return s
}

def each_dim_sum(a : int[4]) : int {
    var s = 0
    for (x in each(a)) {
        s += x
    }
    return s
}

def each_array_bump(var a : array<int>) {   // nolint:LINT014 - writes go through the element reference
    for (x in each(a)) {
        x++
    }
}

def each_break_at(a : array<int>; stop : int) : int {
    var n = 0
    for (_x in each(a)) {
        n++
        break if (n == stop)
    }
    return n
}

def each_return_at(a : array<int>; stop : int) : int {
    var n = 0
    for (x in each(a)) {
        n++
        return x if (n == stop)
    }
    return -1
}

def each_with_range(a : array<int>) : int {
    var s = 0
    for (x, i in each(a), range(2)) {
        s += x * (i + 1)
    }
    return s
}

def each_nested(a : array<int>) : int {
    var s = 0
    for (x in each(a)) {
        for (y in each(a)) {
            s += x * y
        }
    }
    return s
}

def each_table_stays_generic(t : table<int; int>) : int {
    var s = 0
    for (kv in each_kv(t)) {
        s += kv.key + kv.value
    }
    return s
}

[test(description = "each() over arrays, fixed arrays and tables inlines correctly: writable element refs, unlocking on break and return, shortest-source stop")]
def test_each_inline(t : T?) {
    t |> run("array source walks inline") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@each_array_sum))
        var a <- [for (i in range(5)); i]
        t |> equal(each_array_sum(a), 10)
        delete a
    }
    t |> run("fixed array source walks inline") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@each_dim_sum))
        var d : int[4]
        for (i in range(4)) {
            d[i] = i * 10
        }
        t |> equal(each_dim_sum(d), 60)
    }
    t |> run("element is a writable reference") @(t : T?) {
        var a <- [for (i in range(4)); i]
        each_array_bump(a)
        t |> equal(each_array_sum(a), 10)
        delete a
    }
    t |> run("break unlocks the array") @(t : T?) {
        var a <- [for (i in range(5)); i]
        t |> equal(each_break_at(a, 3), 3)
        a |> push(5)
        t |> equal(length(a), 6)
        delete a
    }
    t |> run("return unlocks the array") @(t : T?) {
        var a <- [for (i in range(5)); i]
        t |> equal(each_return_at(a, 4), 3)
        a |> push(5)
        t |> equal(length(a), 6)
        delete a
    }
    t |> run("mixed sources stop at the shortest") @(t : T?) {
        var a <- [for (i in range(5)); i + 1]
        t |> equal(each_with_range(a), 5)
        delete a
    }
    t |> run("nested loops over one array") @(t : T?) {
        var a <- [for (i in range(4)); i]
        t |> equal(each_nested(a), 36)
        delete a
    }
    t |> run("table each keeps the generic path") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@each_table_stays_generic))
        var tab <- {1 => 10, 2 => 20}
        t |> equal(each_table_stays_generic(tab), 33)
        delete tab
    }
}

def test_for {
    return <- generator<int> {
        for (t in range(10)) {
            yield t
        }
        return false
    }
}

[test(description = "a JIT-compiled generator yields the same sequence as the interpreter")]
def test_generator(t : T?) {
    t |> run("make generator") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_for))
        var count = 0
        for (i, j in range(10), test_for()) {
            t |> equal(i, j)
            count ++
        }
        t |> equal(10, count)
    }
}

[jit, sideeffects]
def test_min(a, b : int) =>
    a < b ? a : b  // nolint:PERF015 -- the ternary is the PHI shape under test; min() would not emit one

[jit, sideeffects]
def test_min(a, b, c : int) {
    if (a < b) {
        return a < c ? a : c    // nolint:PERF015 -- the ternary is the PHI shape under test; min() would not emit one
    } else {
        return b < c ? b : c    // nolint:PERF015 -- the ternary is the PHI shape under test; min() would not emit one
    }
}

[test(description = "JIT-compiled min2 and min3 merge their branch values correctly")]
def test_phi(t : T?) {

    t |> success(!jit_enabled() || is_jit_function(@@ < (a : int; b : int) : bool > test_min))
    t |> success(!jit_enabled() || is_jit_function(@@ < (a : int; b : int; c : int) : bool > test_min))

    t |> equal(1, test_min(1, 2))
    t |> equal(1, test_min(2, 1))
    t |> equal(1, test_min(1, 2, 3))
    t |> equal(1, test_min(1, 3, 2))
    t |> equal(1, test_min(2, 1, 3))
    t |> equal(1, test_min(2, 3, 1))
    t |> equal(1, test_min(3, 1, 2))
    t |> equal(1, test_min(3, 2, 1))

    t |> run("min2") @(t : T?) {
        var fake <- Faker()
        let a = fake |> random_int
        let b = fake |> random_int
        t |> equal(test_min(a, b), a < b ? a : b)    // nolint:PERF015 -- the oracle mirrors the ternary under test
    }

    t |> run("min3") @(t : T?) {
        var fake <- Faker()
        let a = fake |> random_int
        let b = fake |> random_int
        let c = fake |> random_int
        t |> equal(test_min(a, b, c), a < b ? (a < c ? a : c) : (b < c ? b : c))    // nolint:PERF015 -- the oracle mirrors the ternaries under test
    }
}
