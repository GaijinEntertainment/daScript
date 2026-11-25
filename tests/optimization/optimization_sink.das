options gen2
options rtti
options no_coverage
options disable_auto_inline    // this file pins sink shapes; auto-splicing would rewrite them

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require dastest/testing_boost public
require strings
require daslib/strings_boost
require _optimized_body_helper

var g_count = 0

def bump() : int {
    g_count ++
    return g_count
}

struct Header {
    width : int
    name : string
}

[inline]
def expect_or_panic(ok : bool; what : string) {
    if (!ok) {
        panic("check: {what}")
    }
}

[export]
def target_sink_message(x : int) : int {
    let message = "item {x} is negative"
    if (x < 0) {
        panic(message)
    }
    return x
}

[export]
def target_sink_inlined_check(x : int) : int {
    expect_or_panic(x >= 0, "item {x} is negative")
    return x
}

[export]
def target_sink_field(h : Header) : int {
    expect_or_panic(h.width % 32 == 0, "width {h.width} of '{h.name}' is off the 32 lattice")
    return h.width
}

[export]
def target_keep_effect(x : int) : int {
    let message = "item {bump()} is negative"
    if (x < 0) {
        panic(message)
    }
    return x
}

[export]
def target_keep_raising(a : array<int>; x : int) : int {
    let message = "item {a[x]} is negative"
    if (x < 0) {
        panic(message)
    }
    return x
}

[export]
def target_keep_effect_cond(x : int) : int {
    let message = "item {x} is negative"
    if (bump() < 0) {
        panic(message)
    }
    return x
}

[export]
def target_keep_both_arms(x : int) : string {
    let message = "item {x}"
    if (x < 0) {
        return "negative {message}"
    } else {
        return "positive {message}"
    }
}

[export]
def target_keep_used_after(x : int) : string {
    let message = "item {x}"
    if (x < 0) {
        panic(message)
    }
    return message
}

//! the local ending in `name` is declared in the body's first `if`
def private let_inside_if(body, name : string) : bool {
    let at_if = find(body, "if (")
    let at_let = find(body, "{name}:")
    return at_if >= 0 && at_let > at_if
}

[test(description = "a pure local read only in one branch of the `if` that follows it moves into that branch: a message, an inlined check's message and one that reads a struct field")]
def test_sink_moves(t : T?) {
    ast_gc_guard() {
        t |> run("a message read only on the failure path moves in") @(t : T?) {
            let b = body_of(t, "target_sink_message")
            t |> success(let_inside_if(b, "message"), "sunk: {b}")
        }
        t |> run("an inlined check builds its message inside the failure branch") @(t : T?) {
            let b = body_of(t, "target_sink_inlined_check")
            t |> success(let_inside_if(b, "arg_what"), "sunk: {b}")
        }
        t |> run("a message that reads a struct field moves in") @(t : T?) {
            let b = body_of(t, "target_sink_field")
            t |> success(let_inside_if(b, "arg_what"), "sunk: {b}")
        }
    }
}

[test(description = "a local stays in front of the `if` when its initializer has an effect or may raise, when the condition has an effect, and when it is read in both arms or after the `if`")]
def test_sink_keeps(t : T?) {
    ast_gc_guard() {
        t |> run("an initializer with an effect stays") @(t : T?) {
            let b = body_of(t, "target_keep_effect")
            t |> success(!let_inside_if(b, "message"), "kept: {b}")
        }
        t |> run("an initializer that may raise stays") @(t : T?) {
            let b = body_of(t, "target_keep_raising")
            t |> success(!let_inside_if(b, "message"), "kept: {b}")
        }
        t |> run("a condition with an effect keeps the local in front of it") @(t : T?) {
            let b = body_of(t, "target_keep_effect_cond")
            t |> success(!let_inside_if(b, "message"), "kept: {b}")
        }
        t |> run("a local read in both arms stays") @(t : T?) {
            let b = body_of(t, "target_keep_both_arms")
            t |> success(!let_inside_if(b, "message"), "kept: {b}")
        }
        t |> run("a local read after the `if` stays") @(t : T?) {
            let b = body_of(t, "target_keep_used_after")
            t |> success(!let_inside_if(b, "message"), "kept: {b}")
        }
    }
}

[test(description = "functions the sink pass rewrote return the same values, keep every call with an effect, and a thousand passing checks build no message")]
def test_sink_behavior(t : T?) {
    t |> equal(target_sink_message(5), 5)
    t |> equal(target_sink_inlined_check(7), 7)
    t |> equal(target_sink_field(Header(width = 64, name = "base")), 64)
    t |> equal(target_keep_both_arms(-2), "negative item -2")
    t |> equal(target_keep_both_arms(3), "positive item 3")
    t |> equal(target_keep_used_after(4), "item 4")
    g_count = 0
    target_keep_effect(1)
    target_keep_effect_cond(1)
    t |> equal(g_count, 2, "both effects ran though their messages are never used")
    let s0 = string_heap_bytes_allocated()
    for (i in range(1000)) {
        target_sink_inlined_check(i)
    }
    t |> equal(string_heap_bytes_allocated(), s0, "no message built for a passing check")
}
