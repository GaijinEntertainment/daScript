options gen2
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/with_boost

struct A {
    f1 : int
    f2 : int
}

[test(description = "with_ over one array element binds it to the default name _ so assignments change the element")]
def test_single_arg_default_name(t : T?) {
    //! `with_(arr[i]) { _.field = ... }` — no block params, `_` injected,
    //! mutation persists in the underlying array.
    var arr <- [A(f1=1, f2=2), A(f1=3, f2=4)]
    with_(arr[0]) {
        _.f1 = 99
    }
    t |> equal(arr[0].f1, 99, "default _ mutation persists in arr[0]")
    t |> equal(arr[0].f2, 2, "unrelated field untouched")
    t |> equal(arr[1].f1, 3, "other element untouched")
    delete arr
}

[test(description = "with_ over one array element can bind it to a named block parameter")]
def test_single_arg_named_binding(t : T?) {
    //! `with_(arr[i]) $(myA) { myA.field = ... }` — user-named param,
    //! constness stripped so mutation persists.
    var arr <- [A(f1=10, f2=20)]
    with_(arr[0]) $(myA) {
        myA.f1 = 111
        myA.f2 = 222
    }
    t |> equal(arr[0].f1, 111, "named binding mutation persists")
    t |> equal(arr[0].f2, 222, "second-field mutation persists")
    delete arr
}

[test(description = "with_ over two array elements binds both and the body can read one and write the other")]
def test_two_arg_array(t : T?) {
    //! Multi-arg form: two arrays bound to two block params,
    //! mutations persist in both.
    var src <- [A(f1=100, f2=200)]
    var dst <- [A(f1=0, f2=0)]
    with_(dst[0], src[0]) $(d, s) {
        d.f1 = s.f1 + 1
        d.f2 = s.f2 + 2
    }
    t |> equal(dst[0].f1, 101, "dst.f1 = src.f1 + 1")
    t |> equal(dst[0].f2, 202, "dst.f2 = src.f2 + 2")
    delete src
    delete dst
}

[test(description = "with_ over three array elements binds all three to their own block parameters")]
def test_three_arg_array(t : T?) {
    //! Arity 3: three arrays, three block params.
    var a <- [A(f1=0, f2=0)]
    var b <- [A(f1=10, f2=20)]
    var c <- [A(f1=100, f2=200)]
    with_(a[0], b[0], c[0]) $(va, vb, vc) {
        va.f1 = vb.f1 + vc.f1
        va.f2 = vb.f2 + vc.f2
    }
    t |> equal(a[0].f1, 110, "sum across three arrays in f1")
    t |> equal(a[0].f2, 220, "sum across three arrays in f2")
    delete a
    delete b
    delete c
}

struct Owner {
    children : array<A>
}

[test(description = "with_ accepts a container reached through a field chain such as obj.children[1]")]
def test_field_chain_container(t : T?) {
    //! Container is `obj.field[i]` — subexpr is the ExprField
    //! `obj.children`, an lvalue chain rooted in `obj` (ExprVar).
    //! The lvalue-chain check passes and pre-bind works.
    var obj <- Owner(children <- [A(f1 = 5, f2 = 6), A(f1 = 7, f2 = 8)])
    with_(obj.children[1]) $(elem) {
        elem.f1 = 555
    }
    t |> equal(obj.children[1].f1, 555, "second-element mutation persists")
    t |> equal(obj.children[0].f1, 5, "first element untouched")
    delete obj.children
}

def helper_set_f1(var aRef : A&; v : int) {
    aRef.f1 = v
}

[test(description = "a with_ binding can be passed by reference to a function that mutates it")]
def test_pass_binding_to_function(t : T?) {
    //! The block-arg can be passed to another function expecting a
    //! mutable ref; mutations through that path also persist.
    var arr <- [A(f1=1, f2=2)]
    with_(arr[0]) $(a) {
        helper_set_f1(a, 777)
    }
    t |> equal(arr[0].f1, 777, "mutation through helper-fn ref persists")
    delete arr
}

struct TA {
    f1 : int
    f2 : int
}

[test(description = "with_ over a table key that exists binds the stored value so writes persist")]
def test_existing_key(t : T?) {
    //! Mutation on an existing key persists in the table.
    var tab : table<string; TA>
    tab |> insert("k", TA(f1 = 1, f2 = 2))
    with_(tab["k"]) $(v) {
        v.f1 = 99
        v.f2 = 100
    }
    t |> equal(tab["k"].f1, 99, "key mutation persists")
    t |> equal(tab["k"].f2, 100, "second field mutation persists")
}

[test(description = "with_ over a missing table key creates a default value that the body can fill")]
def test_missing_key_creates_default(t : T?) {
    //! tab[k] is upsert: missing key gets a default entry, then the
    //! body's mutations persist on that newly-created entry.
    var tab : table<string; TA>
    with_(tab["new"]) $(v) {
        v.f1 = 42
    }
    t |> success(key_exists(tab, "new"), "missing key was created")
    t |> equal(tab["new"].f1, 42, "mutation on new entry persists")
}

[test(description = "with_ over a table value binds it to the default name _")]
def test_table_default_name(t : T?) {
    //! Default `_` binding works for tables too.
    var tab : table<string; TA>
    tab |> insert("x", TA(f1 = 5, f2 = 6))
    with_(tab["x"]) {
        _.f1 = 77
    }
    t |> equal(tab["x"].f1, 77, "default _ binding mutation persists")
}

[test(description = "with_ over a table of ints binds the value by reference so assigning changes the table")]
def test_table_workhorse_value(t : T?) {
    //! Table with workhorse value type — block-arg is `int&`, mutation
    //! persists.
    var tab <- { "alpha" => 10, "beta" => 20 }
    with_(tab["alpha"]) $(v) {
        v = 200
    }
    t |> equal(tab["alpha"], 200, "workhorse value mutation persists")
    t |> equal(tab["beta"], 20, "other key untouched")
}

struct NA {
    f1 : int
}

[test(description = "with_ over five arrays in one block binds all five element references")]
def test_five_arrays(t : T?) {
    //! 5 array containers, one block, all-array. Proves the inline
    //! emission scales past the old per-arity-helper limit (3).
    var a1 <- [NA(f1 = 1)]
    var a2 <- [NA(f1 = 2)]
    var a3 <- [NA(f1 = 3)]
    var a4 <- [NA(f1 = 4)]
    var a5 <- [NA(f1 = 5)]
    with_(a1[0], a2[0], a3[0], a4[0], a5[0]) $(x1, x2, x3, x4, x5) {
        x1.f1 = x1.f1 + x2.f1 + x3.f1 + x4.f1 + x5.f1
    }
    t |> equal(a1[0].f1, 15, "1 + 2 + 3 + 4 + 5 = 15 written to a1[0]")
    t |> equal(a2[0].f1, 2, "a2 untouched")
    t |> equal(a5[0].f1, 5, "a5 untouched")
}

[test(description = "with_ over four arrays and a table in one block binds all five")]
def test_mixed_4_arrays_1_table(t : T?) {
    //! 4 arrays + 1 table — single table allowed, mixed positionally.
    var arr_w <- [0]
    var arr_x <- [10]
    var arr_y <- [100]
    var arr_z <- [1000]
    var tab : table<string; int>
    tab |> insert("k", 50)
    with_(arr_w[0], arr_x[0], tab["k"], arr_y[0], arr_z[0]) $(w, x, t_k, y, z) {
        w = x + t_k + y + z
        t_k = 999
    }
    t |> equal(arr_w[0], 10 + 50 + 100 + 1000, "sum across mixed array+table args")
    t |> equal(tab["k"], 999, "table entry mutated")
}

[test(description = "with_ over seven arrays in one block binds all seven")]
def test_seven_arrays(t : T?) {
    //! Even larger N — verifies macro really doesn't have an arity cap.
    var a <- [1]
    var b <- [2]
    var c <- [3]
    var d <- [4]
    var e <- [5]
    var f <- [6]
    var g <- [7]
    with_(a[0], b[0], c[0], d[0], e[0], f[0], g[0]) $(va, vb, vc, vd, ve, vf, vg) {
        va = vb + vc + vd + ve + vf + vg
    }
    t |> equal(a[0], 27, "2+3+4+5+6+7 = 27")
}

struct WA {
    f1 : int
}

[test(description = "with_ over an int array element binds it to _ and assigning changes the element")]
def test_int_array_default_name(t : T?) {
    //! `with_(arr[i]) { _ = X }` on a workhorse-element array — block
    //! param `_` is `int&`, mutation persists in the array.
    var arr <- [10, 20, 30]
    with_(arr[1]) {
        _ = 222
    }
    t |> equal(arr[0], 10, "untouched element stays")
    t |> equal(arr[1], 222, "workhorse mutation persists")
    t |> equal(arr[2], 30, "untouched element stays")
    delete arr
}

[test(description = "with_ over an int array element can bind it to a named parameter")]
def test_int_array_named_binding(t : T?) {
    //! Named binding `$(x)` — same propagation, daslang infers `x : int&`
    //! from the helper sig.
    var arr <- [1, 2, 3]
    with_(arr[0]) $(x) {
        x = 999
    }
    t |> equal(arr[0], 999, "named workhorse binding mutation persists")
    delete arr
}

[test(description = "with_ over a float array element can bind it to a named parameter")]
def test_float_array_named(t : T?) {
    var arr <- [1.0f, 2.0f, 3.0f]
    with_(arr[2]) $(x) {
        x = 99.5f
    }
    t |> equal(arr[2], 99.5f, "float workhorse mutation persists")
    delete arr
}

[test(description = "with_ over elements of two int arrays binds both so the body can combine them")]
def test_multi_arg_workhorse(t : T?) {
    //! Multi-arg form across two workhorse arrays.
    var a <- [10, 20, 30]
    var b <- [100, 200, 300]
    with_(a[0], b[1]) $(x, y) {
        x = y + 1
    }
    t |> equal(a[0], 201, "x = y + 1 with workhorse types")
    t |> equal(b[1], 200, "source unchanged")
    delete a
    delete b
}

[test(description = "with_ over an int element and a struct element binds both")]
def test_mixed_workhorse_struct(t : T?) {
    var ints <- [10, 20]
    var structs <- [WA(f1 = 100)]
    with_(ints[0], structs[0]) $(i, s) {
        i = s.f1 + 1
    }
    t |> equal(ints[0], 101, "workhorse + struct multi-arg")
    delete ints
    delete structs
}

// Block-arg `$(a)` / `$(v)` etc. are present for syntax coverage but
// the body only exercises the lock-vs-mutation path on the container,
// not the bound element. Suppress unused-block-arg lint here.


//! When the with_ body panics inside the macro's invoke target, the
//! lock is intentionally not released (daslang panic is fatal — see
//! CLAUDE.md "Panic is fatal, not an exception"). These tests
//! deliberately don't try to ``delete`` the leaked-locked containers
//! afterwards — that would compound the failure. We only verify the
//! panic is caught at the with_ boundary via try/recover.

struct LA {
    f1 : int
}

[test(description = "pushing to an array inside with_ panics, because the container is locked, and the panic is caught at the with_ boundary")]
def test_push_inside_panics(t : T?) {
    var arr <- [LA(f1=1), LA(f1=2)]
    var panicked = false
    try {
        with_(arr[0]) $(_a) {
            arr |> push(LA(f1 = 99))
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "push during with_ body must panic")
}

[test(description = "erasing from an array inside with_ panics and is caught")]
def test_erase_inside_panics(t : T?) {
    var arr <- [LA(f1=1), LA(f1=2), LA(f1=3)]
    var panicked = false
    try {
        with_(arr[1]) $(_a) {
            arr |> erase(0)
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "erase during with_ body must panic")
}

[test(description = "resizing an array inside with_ panics and is caught")]
def test_resize_inside_panics(t : T?) {
    var arr <- [LA(f1=1)]
    var panicked = false
    try {
        with_(arr[0]) $(_a) {
            arr |> resize(100)
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "resize during with_ body must panic")
}

[test(description = "clearing an array inside with_ panics and is caught")]
def test_clear_inside_panics(t : T?) {
    var arr <- [LA(f1=1)]
    var panicked = false
    try {
        with_(arr[0]) $(_a) {
            arr |> clear
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "clear during with_ body must panic")
}

[test(description = "inserting into a table inside with_ panics and is caught")]
def test_table_insert_inside_panics(t : T?) {
    var tab : table<string; LA>
    tab |> insert("a", LA(f1 = 1))
    var panicked = false
    try {
        with_(tab["a"]) $(_v) {
            tab |> insert("b", LA(f1 = 2))
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "table insert during with_ body must panic")
}

[test(description = "erasing from a table inside with_ panics and is caught")]
def test_table_erase_inside_panics(t : T?) {
    var tab <- { "a" => LA(f1 = 1), "b" => LA(f1 = 2) }
    var panicked = false
    try {
        with_(tab["a"]) $(_v) {
            tab |> erase("b")
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "table erase during with_ body must panic")
}

[test(description = "pushing to one array while with_ holds two arrays panics and is caught")]
def test_multi_arg_inner_push_panics(t : T?) {
    var a <- [LA(f1=1)]
    var b <- [LA(f1=2)]
    var panicked = false
    try {
        with_(a[0], b[0]) $(_va, _vb) {
            b |> push(LA(f1 = 99))
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "multi-arg: mutation to either locked array must panic")
}
