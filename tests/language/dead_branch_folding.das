options gen2
options no_unused_function_arguments = false
// lint-skip-file: these tests rely on infer-time folding of
// constant-false branches, which the lint pipeline's
// no_infer_time_folding policy disables

require dastest/testing_boost public

struct SiFoo {
    a, b : int
}

struct SiBar {
    c, d : int
}

[sideeffects]
def si_setA(var obj; val : auto(valT)) {
    if (typeinfo has_field<a>(obj)) {
        if (typeinfo typename(obj.a) == typeinfo typename(type<valT -const>)) {
            obj.a = val
        }
    }
}

let {
    si_canQQ : bool const = false
}

[test(description = "a branch whose condition folds to a constant false at infer time is dropped before its broken body is type-checked, so a typeinfo-guarded write compiles for structs without the field")]
def test_static_if(t : T?) {
    t |> run("static_if with has_field") @(t : T?) {
        var f : SiFoo
        var b : SiBar
        si_setA(f, 1)
        si_setA(b, 2)
        t |> equal(f.a, 1)
    }
    t |> run("const false eliminates broken code") @(t : T?) {
        if (si_canQQ) {
            broken = badly  // compiled out
        }
        if (2 + 2 == 4) {
            static_assert(true, "nothing to look at")
        } else {
            badly = broken  // compiled out
        }
        t |> success(true)
    }
}

// the same body as containers.inc, which failed_failed_containers.das
// includes with FAIL_TO_COMPILE = true
let FAIL_TO_COMPILE = false

struct DbfFoo {
    a : int
}

def dbf_array_ops(t : T?; var a : array<DbfFoo>; var e : DbfFoo; var et : DbfFoo# implicit) {
    reserve(a, 10)
    for (i in range(10)) {
        push(a, DbfFoo(a = i))
    }
    t |> equal(10, length(a))
    clear(a)
    t |> equal(0, length(a))
    t |> success(empty(a))
    push(a, e)
    push(a, e, 0)
    t |> equal(1, a[0].a)
    t |> equal(1, a[1].a)
    t |> equal(2, length(a))
    t |> equal(1, back(a).a)
    static_if (FAIL_TO_COMPILE) {
        push(a, et)             // can't push temporary value
        push(a, et, 0)
    }
    var b := a
    t |> equal(1, b[0].a)
    t |> equal(1, b[1].a)
    t |> equal(2, length(b))
    clear(a)
    t |> equal(0, length(a))
    a := b
    t |> equal(2, length(a))
    emplace(a, e)
    t |> equal(3, length(a))
    t |> equal(0, e.a)
    if (FAIL_TO_COMPILE) {
        emplace(a, et)          // can't emplace temporary value
        emplace(a, et, 0)
    }
    pop(a)
    erase(a, 0)
    erase(a, 0, 1)
    t |> equal(0, length(a))
    a := b
    for (v in each(a)) {
        v.a = 2
    }
    for (v in each(a)) {
        t |> equal(2, v.a)
    }
    t |> equal(2, a[0].a)
    t |> equal(2, a[1].a)
    t |> equal(2, length(a))
}

def dbf_table_ops(t : T?; var a : table<int; DbfFoo>; var e : DbfFoo; var et : DbfFoo implicit) {
    for (i in range(10)) {
        a |> emplace(i, DbfFoo(a = i))
    }
    t |> equal(10, length(a))
    clear(a)
    t |> equal(0, length(a))
    t |> success(empty(a))
    a |> insert(0, e)
    a |> insert(1, e)
    t |> equal(1, a |> get_value(0).a)
    t |> equal(1, a |> get_value(1).a)
    t |> equal(2, length(a))
    clear(a)
    a |> insert(0, e)
    a |> insert(1, e)
    var b := a
    a := b
    t |> equal(1, b |> get_value(0).a)
    t |> equal(1, b |> get_value(1).a)
    t |> equal(2, length(b))
    erase(a, 0)
    erase(a, 1)
    t |> equal(0, length(a))
    a := b
    t |> equal(1, a |> get_value(0).a)
    t |> equal(1, a |> get_value(1).a)
    t |> equal(2, length(a))
    for (k in keys(a)) {
        t |> equal(1, a?[k]?.a ?? -1)
    }
    for (k in keys(a)) {
        t |> success(k == 0 || k == 1)
    }
    for (v in values(a)) {
        t |> equal(1, v.a)
    }
}

[test(description = "array and table push, emplace, insert, erase, clone and iteration keep their element values, with the push-a-temporary negatives compiled out under a constant-false flag")]
def test_temp_containers(t : T?) {
    t |> run("temporary array operations") @(t : T?) {
        var a : array<DbfFoo>
        var et : DbfFoo := DbfFoo(a = 2)
        dbf_array_ops(t, a, DbfFoo(a = 1), et)
    }
    t |> run("temporary table operations") @(t : T?) {
        var a : table<int; DbfFoo>
        dbf_table_ops(t, a, DbfFoo(a = 1), DbfFoo(a = 2))
    }
}

// const folding must skip a `struct template`'s field initializers:
// v2's init holds an unresolved call that folds to a null func. The
// foldable float4(...) in `ma` is what engages the folder.
class v {}

struct template v2{r = i((3 ? reinterpret<bitfield<>>(([],) < (0 > t)) as r * ([{for((v) in @@o[[]] > (2) > 5, (v)); (0[((3 > (k()) > new<a<v>>() - 0)) as  t8 ]) % (9) % (99) % o }
].) ?[ (T)] .. 2 : [  ]), (3));
}

[export]
def ma => float4(uint(), 2, -9, 3)

[test(description = "const folding skips a struct template's unresolvable field initializer instead of crashing, so a program holding one compiles and runs")]
def test_template_struct_const_folding(t : T?) {
    t |> equal(ma(), float4(0, 2, -9, 3))
}
