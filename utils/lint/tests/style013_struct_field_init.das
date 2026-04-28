options gen2
// STYLE013: struct var with default/empty init followed by a run of field assignments
//
// Problem: Declaring a struct var via `Foo()` (or as `var a : Foo` for
// `[safe_when_uninitialized]` structs) and then patching it with TWO OR MORE
// contiguous field assignments is the same "I-forgot-the-idiomatic-init"
// shape as STYLE012 — and the named-argument constructor reads better and
// avoids the redundant zero-init.
//
// Bad pattern:
//   var a = Foo()
//   a.x = 1
//   a.y = 2
//
// Good pattern:
//   var a = Foo(x = 1, y = 2)
//
// A single trailing field assignment stays silent — it is often the more
// readable form (e.g. when the struct is mostly defaulted and only one
// field is patched), and we want this rule to flag the bug case, not
// every possible rewrite.

struct Foo {
    x : int = 0
    y : int = 0
    s : string = ""
}

[safe_when_uninitialized]
struct Bar {
    x : int = 0
    y : int = 0
}

struct Movable {
    tag : string = ""
    payload : array<int>
}

// bad: var a = Foo() then two field assigns (1 warning)
def bad_default_ctor() {
    var a = Foo()
    a.x = 1
    a.y = 2
}

// bad: var a : Bar (uninitialized, struct is safe_when_uninitialized) (1 warning)
def bad_uninit() {
    var a : Bar
    a.x = 3
    a.y = 4
}

// bad: three contiguous assigns still produce a single warning (1 warning)
def bad_three_fields() {
    var a = Foo()
    a.x = 1
    a.y = 2
    a.s = "hi"
}

// bad: clone-assign on string field (1 warning, via ExprClone branch)
def bad_clone_assign() {
    var a = Foo()
    a.x = 1
    a.s := "via_clone"
}

// bad: move-assign on a non-copyable field (1 warning, via ExprMove branch)
def bad_move_assign() {
    var a = Movable()
    a.tag = "lbl"
    a.payload <- [1, 2, 3]
}

// good: single assign — silent, often the more readable form
def good_single_field() {
    var a = Foo()
    a.x = 42
}

// good: named-argument constructor — the idiomatic form
def good_named_ctor() {
    let a = Foo(x = 1, y = 2, s = "ok")
    let _ = a.x
}

// good: partial constructor + one trailing assign — init is non-empty,
// so STYLE013 stays silent (and a single trailing patch is acceptable)
def good_partial_ctor() {
    var a = Foo(x = 1)
    a.y = 2
}

// good: inscope variable — matches STYLE011/012 exclusion
def good_inscope() {
    var inscope a = Foo()
    a.x = 1
    a.y = 2
}

// good: assignments split by an intervening statement — not contiguous
def good_interleaved() : int {
    var a = Foo()
    let stride = 10
    a.x = stride
    return a.x
}

// good: modifying an existing struct value (init is not an empty MakeStruct)
def make_foo : Foo {
    return Foo(x = 7)
}
def good_modify_existing() {
    var a = make_foo()
    a.x = 1
    a.y = 2
}
