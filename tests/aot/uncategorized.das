options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require UnitTest
require _aot_so_mod

def private read_with_cleanup(value : int const?; enabled : bool; var cleanup_value : int&) : int {
    if (!enabled) { return 0 }
    let selected = value
    if (selected == null) { return -1 }
    var result : int
    { result = *selected } finally { cleanup_value += *selected }
    return result
}

[test(description = "A const pointer hoisted through an early return stays an l-value in AOT")]
def const_pointer_hoisted_through_early_return_retains_lvalue(t : T?) {
    var value = 37
    var cleanup_value = 0
    t |> equal(read_with_cleanup(unsafe(addr(value)), true, cleanup_value), 37)
    t |> equal(cleanup_value, 37)
    t |> equal(read_with_cleanup(null, true, cleanup_value), -1)
    t |> equal(read_with_cleanup(unsafe(addr(value)), false, cleanup_value), 0)
    t |> equal(cleanup_value, 37, "early returns do not execute cleanup from an unentered scope")
}

// Regression for issue #3065 (fuzzer, AOT): a call to the UnitTest module function
// `testFoo`, which is defined inside `namespace das` but was declared nowhere in the
// module's AOT header. AOT-emitted callers `#include "unitTest.h"` then emit a bare
// `testFoo(...)` -> `error: use of undeclared identifier 'testFoo'`. Fixed by declaring
// testFoo in unitTest.h (inside namespace das, so AOT callers link). makeDummy sets
// fooData=1; testFoo sets it to 1234 -- so the side effect proves the call really runs.

[test(description = "A call to a namespaced extern function declared in the module header compiles and runs in AOT")]
def test_call_namespaced_extern(t : T?) {
    t |> run("aot call to namespace-das module extern testFoo") @(t : T?) {
        var foo = makeDummy()
        t |> equal(foo.fooData, 1)
        testFoo(foo)
        t |> equal(foo.fooData, 1234)
    }
}

// #3391 — indexing a raw pointer with int64/uint64 was AOT-ambiguous for the
// non-var pointer form (das_index<T * const> lacked 64-bit at/safe_at overloads).
// Non-var params cover the gap; var / const-pointee params are the already-working controls.

def read_i64(p : float?; i : int64) : float => unsafe(p[i])

def read_u64(p : float?; i : uint64) : float => unsafe(p[i])

def safe_read_i64(p : float?; i : int64) : float => unsafe(p?[i]) ?? -1.0

def safe_read_u64(p : float?; i : uint64) : float => unsafe(p?[i]) ?? -1.0

def write_i64(var p : float?; i : int64; v : float) {
    unsafe(p[i]) = v
}

def read_const_pointee_i64(p : float const?; i : int64) : float => unsafe(p[i])

[test(description = "Indexing a raw pointer with an int64 or uint64 compiles and reads the right element in AOT")]
def test_int64_ptr_index(t : T?) {
    t |> run("int64/uint64 raw-pointer indexing") @(t : T?) {
        var buf : float[8]
        for (k in range(8)) {
            buf[k] = float(k)
        }
        var p = unsafe(addr(buf[0]))
        write_i64(p, int64(3), 30.0)
        t |> equal(read_i64(p, int64(3)), 30.0)
        t |> equal(read_u64(p, uint64(2)), 2.0)
        t |> equal(safe_read_i64(p, int64(1)), 1.0)
        t |> equal(safe_read_u64(p, uint64(4)), 4.0)
        t |> equal(read_const_pointee_i64(p, int64(5)), 5.0)
        let nullp : float? = null
        t |> equal(safe_read_i64(nullp, int64(0)), -1.0)
        t |> equal(safe_read_u64(nullp, uint64(0)), -1.0)
    }
}

def panic_under_finally(var ran : bool&) {
    panic("PANIC_SKIPS_FINALLY_PROBE")
} finally {
    ran = true
}

[test(description = "A panic skips the function's pending finally block and is caught by the caller in AOT")]
def test_panic_skips_finally(t : T?) {
    t |> run("a panic skips the function's finally, as in every tier") @(t : T?) {
        var ran = false
        var caught = false
        try {
            panic_under_finally(ran)
        } recover {
            caught = true
        }
        t |> equal(caught, true)
        t |> equal(ran, false)
    }
}

// AOT most-vexing-parse regression: a for-source made ONLY of constructor-style casts
// (range64(int64(a), int64(b))) used to emit
//   das_iterator<range64> it(range64(int64_t(a),int64_t(b)));
// which C++ parses as a function DECLARATION, not a variable. The emitter now
// double-parenthesizes the initializer.

// mutable globals so the calls can't const-fold — the loop must survive to AOT
var private g_lo = 3
var private g_hi = 7

def private sum_range(tb, te : int) : int64 {
    var s = 0l
    for (t in range64(int64(tb), int64(te))) {
        s += t
    }
    return s
}

[test(description = "A for loop over range64 built from constructor-style casts compiles as a variable and iterates in AOT")]
def test_range64_ctor_arg_source(t : T?) {
    t |> equal(18l, sum_range(g_lo, g_hi))
    t |> equal(0l, sum_range(g_hi, g_hi))
}

[export]
def zero_function_ref() => type <function&>

[test(description = "A reference to a freshly made local compiles and reads back in AOT")]
def test_ref_make_local(t : T?) {
    t |> success(zero_function_ref() == null, "zeroed function reference")
}

//! Regression: iterating a vector-like container of a value-annotation handle
//! (ManagedValueAnnotation<T>, e.g. `clock` / `das::Time`) used to emit a value
//! variable declaration in AOT C++ but pointer-dereference usage, breaking the
//! AOT build. The AOT emitter must keep the `*` suffix when substitute_ref is
//! requested, even for value handles.

def sum_clocks(clocks : array<clock>) : int64 {
    var total = 0l
    for (c in clocks) {
        total += int64(c)
    }
    return total
}

def sum_clocks_const(clocks : array<clock> const) : int64 {
    var total = 0l
    for (c in clocks) {
        total += int64(c)
    }
    return total
}

def first_clock(clocks : array<clock>) : clock {
    for (c in clocks) {
        return c
    }
    return mktime(1970, 1, 1, 0, 0, 0)
}

[test(description = "Iterating a container of value-annotation handles compiles with the right pointer form in AOT")]
def test_iter_value_handle(t : T?) {
    t |> run("for over array<clock>") @(t : T?) {
        let clocks <- [mktime(2024, 1, 1, 0, 0, 0), mktime(2024, 1, 2, 0, 0, 0), mktime(2024, 1, 3, 0, 0, 0)]
        let expected = int64(clocks[0]) + int64(clocks[1]) + int64(clocks[2])
        t |> equal(sum_clocks(clocks), expected)
    }
    t |> run("for over array<clock> const") @(t : T?) {
        let clocks <- [mktime(2024, 1, 1, 0, 0, 0), mktime(2024, 1, 2, 0, 0, 0)]
        let expected = int64(clocks[0]) + int64(clocks[1])
        t |> equal(sum_clocks_const(clocks), expected)
    }
    t |> run("return element from iteration") @(t : T?) {
        let clocks <- [mktime(2024, 1, 1, 0, 0, 0), mktime(2024, 1, 2, 0, 0, 0)]
        t |> equal(int64(first_clock(clocks)), int64(clocks[0]))
    }
}

//! Regression: a writable reference parameter of a value-annotation handle
//! (ManagedValueAnnotation<T>, e.g. ``clock`` / ``das::Time``, or ECS
//! ``EntityId``) must emit ``T &`` in AOT C++, not ``T`` by value. The
//! over-broad ``valueHandle`` exclusion in ``describeCppTypeEx`` skipped the
//! ``&`` suffix for every ``tHandle`` with ``isRefType == false``, which
//! silently turned RW lambda/function parameters into by-value copies --
//! mutations done by AOT code were lost on the C++ caller side.

def set_clock_to_2030(var c : clock&) {
    c = mktime(2030, 1, 1, 0, 0, 0)
}

def swap_clocks(var a : clock&; var b : clock&) {
    let tmp = a
    a = b
    b = tmp
}

[test(description = "A writable reference parameter of a value-annotation handle is passed by reference in AOT, so its mutations reach the caller")]
def test_value_handle_ref_param(t : T?) {
    t |> run("writable ref to value-handle preserves mutation") @(t : T?) {
        var c = mktime(2024, 1, 1, 0, 0, 0)
        set_clock_to_2030(c)
        t |> equal(int64(c), int64(mktime(2030, 1, 1, 0, 0, 0)))
    }
    t |> run("two writable refs swap independently") @(t : T?) {
        var a = mktime(2024, 1, 1, 0, 0, 0)
        var b = mktime(2030, 6, 15, 0, 0, 0)
        let a0 = int64(a)
        let b0 = int64(b)
        swap_clocks(a, b)
        t |> equal(int64(a), b0)
        t |> equal(int64(b), a0)
    }
}

// Regression for issue #3212: an AOT-required module whose struct holds a by-value field
// of a later-declared struct. The generated C++ for THIS file emits _aot_so_mod's structs
// (Outer holds Inner by value, declared Outer-first), which only compiles once the emitter
// emits the required module's struct definitions dependency-first.

[test(description = "A struct of an AOT-required module holding a later-declared struct by value is emitted dependency-first")]
def test_struct_decl_order(t : T?) {
    t |> run("aot required-module by-value struct in reverse decl order") @(t : T?) {
        var o : Outer
        o.inner.x = 1
        o.inner.y = 5
        o.tag = 2
        t |> equal(o.inner.x, 1)
        t |> equal(o.inner.y, 5)
        t |> equal(o.tag, 2)
    }
}
