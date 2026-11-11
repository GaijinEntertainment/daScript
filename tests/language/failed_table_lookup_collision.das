//! two by-reference lookups into the same table in one expression do
//! not compile, since one insert can move the other's slot - in
//! initializers, copy, clone, move and field-access forms
options gen2
expect 30250:12

def take_two_ret(var a : int&; var b : int&) : int {
    return a + b
}

// collision: global variable init with two table ref lookups
var g_tab : table<int; int> <- { 1 => 10, 2 => 20 }
var g_val = take_two_ret(g_tab[1], g_tab[2])

struct Foo {
    tab : table<int; int>
}

// collision: copy assign, both sides are references
def test_copy_assign() {
    var tab : table<int; int>
    tab[1] = tab[2]
}

// collision: clone assign, both sides are references
def test_clone_assign() {
    var tab : table<int; int>
    tab[1] := tab[2]
}

// collision: move assign, both sides are references
def test_move_assign() {
    var tab : table<int; int>
    tab[1] <- tab[2]
}

// collision: same table via field access
def test_field_access() {
    var foo : Foo
    foo.tab[1] = foo.tab[2]
}

// collision: swap-like pattern with same table
def test_swap_pattern() {
    var tab : table<int; int>
    var tmp = tab[1]
    tab[1] = tab[2]
    tab[2] = tmp
    feint("{tmp}\n")
}

// collision: function call with two ref args from same table
def take_two(var a : int&; var b : int&) {
    a = b
}
def test_func_two_refs() {
    var tab : table<int; int>
    take_two(tab[1], tab[2])
}

// ok: different tables, no collision
def test_different_tables() {
    var tab1 : table<int; int>
    var tab2 : table<int; int>
    tab1[1] = tab2[2]
}

// ok: value lookups (r2v / underDeref), no collision
def test_value_lookup() {
    var tab : table<int; int>
    var a = tab[1] + tab[2]
    feint("{a}\n")
}

// ok: separate expressions, no collision
def test_separate_expressions() {
    var tab : table<int; int>
    let a = tab[1]
    let b = tab[2]
    feint("{a} {b}\n")
}

struct Slot {
    on : bool
    v : int
}

// collision: a ternary's condition against the rest of the statement
def test_ternary_condition_outside() {
    var tab : table<int; Slot>
    tab[1].v = tab[2].on ? 1 : 2
}

// collision: a ternary's arm against the rest of the statement
def test_ternary_arm_outside(flag : bool) {
    var tab : table<int; Slot>
    var other : table<int; Slot>
    take_two(tab[1].v, flag ? tab[2].v : other[3].v)
}

// collision: a ternary's arm against a later part of the statement
def test_ternary_arm_before(flag : bool) {
    var tab : table<int; Slot>
    var other : table<int; Slot>
    take_two(flag ? tab[1].v : other[3].v, tab[2].v)
}

// collision: a ternary's condition against a later part of the statement
def test_ternary_condition_before() {
    var tab : table<int; Slot>
    var one = 1
    take_two(tab[1].on ? one : one, tab[2].v)
}

// collision: two lookups inside one arm
def test_ternary_inside_arm(flag : bool) {
    var tab : table<int; Slot>
    let a = flag ? take_two_ret(tab[1].v, tab[2].v) : 0
    feint("{a}\n")
}

// ok: a ternary's condition and its arms, and the two arms
def test_ternary_sequenced(flag : bool) {
    var tab : table<int; Slot>
    let a = tab[1].on ? tab[1].v : tab[2].v
    feint("{a}\n")
}
