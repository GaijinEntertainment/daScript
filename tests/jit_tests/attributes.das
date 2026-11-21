options gen2
options bound_check_elision

require dastest/testing_boost

[jit]
def str_lengths_jit(strs : array<string>; extra : string) : int {
    var t = 0
    for (s in strs) {
        t += length(s) + length(extra)
    }
    return t
}

[no_jit, no_aot]
def str_lengths_interp(strs : array<string>; extra : string) : int {
    var t = 0
    for (s in strs) {
        t += length(s) + length(extra)
    }
    return t
}

[jit]
def length_across_assign_jit() : int {
    var s = "ab"
    let a = length(s)
    s = "abcde"
    let b = length(s)
    return a * 10 + b
}

[test(description = "a side-effect-free builtin is evaluated again when its argument changes")]
def test_pure_call(t : T?) {
    t |> run("loop over strings") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@str_lengths_jit))
        let strs <- ["a", "bcd", "", "efgh"]
        t |> equal(str_lengths_interp(strs, "xy"), str_lengths_jit(strs, "xy"))
        t |> equal(16, str_lengths_jit(strs, "xy"))
    }
    t |> run("argument assigned between two calls") @(t : T?) {
        t |> equal(25, length_across_assign_jit())
    }
}

[jit]
def grow_while_reading_jit(var a : array<int>; n : int) {
    for (i in range(n)) {
        a |> push(a[i] * 2)   // nolint:PERF006
    }
}

[no_jit, no_aot]
def grow_while_reading_interp(var a : array<int>; n : int) {
    for (i in range(n)) {
        a |> push(a[i] * 2)   // nolint:PERF006
    }
}

struct Mixed {
    f : float
    n : int
}

[jit]
def mixed_fields_jit(var m : array<Mixed>) : int {
    var t = 0
    for (i in range(length(m))) {   // nolint:PERF018
        m[i].f = float(m[i].n) * 0.5
        t += m[i].n + int(m[i].f)
    }
    return t
}

[no_jit, no_aot]
def mixed_fields_interp(var m : array<Mixed>) : int {
    var t = 0
    for (i in range(length(m))) {   // nolint:PERF018
        m[i].f = float(m[i].n) * 0.5
        t += m[i].n + int(m[i].f)
    }
    return t
}

[jit]
def push_then_read_last_jit(n : int) : int {
    var a : array<int>
    a |> reserve(n)
    var t = 0
    for (i in range(n)) {
        a |> push(i + 1)
        t += a[length(a) - 1] * length(a)
    }
    return t
}

struct Pair {
    a : int
    b : int
}

[jit]
def copy_after_field_write_jit(var p : Pair; n : int) : array<Pair> {
    var res : array<Pair>
    res |> reserve(n)
    for (i in range(n)) {
        p.a = i
        p.b = i * 3
        res |> push(p)
    }
    return <- res
}

[test(description = "array headers and fields stay correct when a loop writes the same array")]
def test_array_header(t : T?) {
    t |> run("push while indexing") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@grow_while_reading_jit))
        var a <- [1, 2, 3]
        var b <- [1, 2, 3]
        grow_while_reading_jit(a, 40)
        grow_while_reading_interp(b, 40)
        t |> equal(length(b), length(a))
        for (x, y in a, b) {
            t |> equal(y, x)
        }
    }
    t |> run("float and int fields of one struct") @(t : T?) {
        var m1 <- [for (i in range(37)); Mixed(f = 0.0, n = i)]
        var m2 <- [for (i in range(37)); Mixed(f = 0.0, n = i)]
        t |> equal(mixed_fields_interp(m2), mixed_fields_jit(m1))
    }
    t |> run("size read right after an in-place push") @(t : T?) {
        t |> equal(2870, push_then_read_last_jit(20))
    }
    t |> run("struct copied after its fields are written") @(t : T?) {
        let res <- copy_after_field_write_jit(Pair(a = -1, b = -1), 9)
        t |> equal(9, length(res))
        for (p, i in res, count()) {
            t |> equal(i, p.a)
            t |> equal(i * 3, p.b)
        }
    }
}

[jit]
def ref_sum_jit(a : int&; flags : array<bool>) : int {
    var t = 0
    for (f in flags) {
        if (f) {
            t += a
        }
    }
    return t
}

[jit]
def proven_index_sum_jit(a : array<int>) : int {
    var s = 0
    for (i in range(length(a))) {   // nolint:PERF018
        s += a[i]
    }
    return s
}

[jit]
def unproven_index_jit(a : array<int>; i : int) : int => a[i]

[test(description = "a reference argument and an elided bounds check read the right values, and an unproven index still panics")]
def test_ref_and_bounds(t : T?) {
    t |> run("reference read in a branch") @(t : T?) {
        let x = 7
        t |> equal(21, ref_sum_jit(x, [true, false, true, true]))
        t |> equal(0, ref_sum_jit(x, [false]))
    }
    t |> run("proven index") @(t : T?) {
        t |> equal(4950, proven_index_sum_jit([for (i in range(100)); i]))
        t |> equal(0, proven_index_sum_jit([for (i in range(0)); i]))
    }
    t |> run("unproven index panics") @(t : T?) {
        let a <- [1, 2, 3]
        var failed = false
        try {
            unproven_index_jit(a, 3)
        } recover {
            failed = true
        }
        t |> success(failed)
        t |> equal(3, unproven_index_jit(a, 2))
    }
}
