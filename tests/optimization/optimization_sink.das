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

[export]
def target_forward_statement(x : int) : int {
    let tripled = x * 3
    g_count += tripled
    return g_count
}

[export]
def target_forward_return(a, b : int) : int {
    let product = a * b + 1
    return product
}

[export]
def target_forward_condition(a, b : int) : int {
    let bigger = a > b
    if (bigger) {
        g_count ++
    }
    return g_count
}

[export]
def target_forward_chain(a : int) : int {
    let first = a + 1
    let second = first * 2
    return second
}

[export]
def target_forward_keep_two_reads(a : int) : int {
    let tripled = a * 3
    return tripled + tripled
}

[export]
def target_forward_keep_effect_first() : int {
    let tripled = g_count * 3
    return bump() + tripled
}

[export]
def target_forward_keep_conditional(a, b : int) : bool {
    let over = a * 3 > b
    return a > 0 && over
}

[export]
def target_forward_keep_raising(arr : array<int>; i : int) : int {
    let item = arr[i]
    return item + 1
}

struct Extent {
    w : int
    h : int
}

struct Panel {
    size : Extent
}

[export]
def target_forward_keep_struct_copy(p : Panel) : int {
    let copied = p.size
    return copied.w + 1
}

[export, never_inline]
def target_forward_keep_annotated(a : int) : int {
    let tripled = a * 3
    return tripled + 1
}

def private built_inside_if(body, text : string) : bool {
    let at_if = find(body, "if (")
    return at_if >= 0 && find(body, text) > at_if
}

//! the local ending in `name` is declared outside the first `if`
def private let_before_if(body, name : string) : bool {
    let at_if = find(body, "if (")
    let at_let = find(body, "{name}:")
    return at_let >= 0 && (at_if < 0 || at_let < at_if)
}

[test(description = "a pure local read only in one branch of the `if` that follows it is built inside that branch: a message, an inlined check's message and one that reads a struct field")]
def test_sink_moves(t : T?) {
    ast_gc_guard() {
        t |> run("a message read only on the failure path moves in") @(t : T?) {
            let b = body_of(t, "target_sink_message")
            t |> success(built_inside_if(b, "is negative"), "sunk: {b}")
        }
        t |> run("an inlined check builds its message inside the failure branch") @(t : T?) {
            let b = body_of(t, "target_sink_inlined_check")
            t |> success(built_inside_if(b, "is negative"), "sunk: {b}")
        }
        t |> run("a message that reads a struct field moves in") @(t : T?) {
            let b = body_of(t, "target_sink_field")
            t |> success(built_inside_if(b, "is off the 32 lattice"), "sunk: {b}")
        }
    }
}

[test(description = "a local stays in front of the `if` when its initializer has an effect or may raise, when the condition has an effect, and when it is read in both arms or after the `if`")]
def test_sink_keeps(t : T?) {
    ast_gc_guard() {
        t |> run("an initializer with an effect stays") @(t : T?) {
            let b = body_of(t, "target_keep_effect")
            t |> success(let_before_if(b, "message"), "kept: {b}")
        }
        t |> run("an initializer that may raise stays") @(t : T?) {
            let b = body_of(t, "target_keep_raising")
            t |> success(let_before_if(b, "message"), "kept: {b}")
        }
        t |> run("a condition with an effect keeps the local in front of it") @(t : T?) {
            let b = body_of(t, "target_keep_effect_cond")
            t |> success(let_before_if(b, "message"), "kept: {b}")
        }
        t |> run("a local read in both arms stays") @(t : T?) {
            let b = body_of(t, "target_keep_both_arms")
            t |> success(let_before_if(b, "message"), "kept: {b}")
        }
        t |> run("a local read after the `if` stays") @(t : T?) {
            let b = body_of(t, "target_keep_used_after")
            t |> success(let_before_if(b, "message"), "kept: {b}")
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

[test(description = "a pure local read once in the statement that follows it gives way to its initializer: under a plain statement, a `return`, an `if` condition, and through a chain of two locals")]
def test_forward_replaces(t : T?) {
    ast_gc_guard() {
        t |> run("a read in the next statement takes the initializer") @(t : T?) {
            let b = body_of(t, "target_forward_statement")
            t |> success(find(b, "tripled") < 0, "forwarded: {b}")
        }
        t |> run("a returned local is the returned expression") @(t : T?) {
            let b = body_of(t, "target_forward_return")
            t |> success(find(b, "product") < 0, "forwarded: {b}")
        }
        t |> run("a local read in the condition of the next `if` moves into it") @(t : T?) {
            let b = body_of(t, "target_forward_condition")
            t |> success(find(b, "bigger") < 0, "forwarded: {b}")
        }
        t |> run("a local that feeds the next local goes with it") @(t : T?) {
            let b = body_of(t, "target_forward_chain")
            t |> success(find(b, "first") < 0 && find(b, "second") < 0, "forwarded: {b}")
        }
    }
}

[test(description = "a local stays when it is read twice, when a call with an effect runs before the read, when the read is under `&&`, when its initializer may raise, when it is a struct copied out of a field, and in a function that carries an annotation")]
def test_forward_keeps(t : T?) {
    ast_gc_guard() {
        t |> run("a local read twice stays") @(t : T?) {
            let b = body_of(t, "target_forward_keep_two_reads")
            t |> success(find(b, "tripled:") >= 0, "kept: {b}")
        }
        t |> run("a call with an effect in front of the read keeps the local") @(t : T?) {
            let b = body_of(t, "target_forward_keep_effect_first")
            t |> success(find(b, "tripled:") >= 0, "kept: {b}")
        }
        t |> run("a read on the right of `&&` keeps the local") @(t : T?) {
            let b = body_of(t, "target_forward_keep_conditional")
            t |> success(find(b, "over:") >= 0, "kept: {b}")
        }
        t |> run("an initializer that may raise stays") @(t : T?) {
            let b = body_of(t, "target_forward_keep_raising")
            t |> success(find(b, "item:") >= 0, "kept: {b}")
        }
        t |> run("a struct copied out of a field stays a local") @(t : T?) {
            let b = body_of(t, "target_forward_keep_struct_copy")
            t |> success(find(b, "copied:") >= 0, "kept: {b}")
        }
        t |> run("a function that carries an annotation keeps its locals") @(t : T?) {
            let b = body_of(t, "target_forward_keep_annotated")
            t |> success(find(b, "tripled:") >= 0, "kept: {b}")
        }
    }
}

[test(description = "functions whose read-once local was forwarded, and the ones that kept it, return the same values and run each effect once")]
def test_forward_behavior(t : T?) {
    g_count = 0
    t |> equal(target_forward_statement(2), 6)
    t |> equal(target_forward_return(3, 4), 13)
    t |> equal(target_forward_condition(5, 2), 7)
    t |> equal(target_forward_condition(1, 2), 7)
    t |> equal(target_forward_chain(4), 10)
    t |> equal(target_forward_keep_two_reads(2), 12)
    g_count = 2
    t |> equal(target_forward_keep_effect_first(), 9, "the local is read before the bump")
    t |> success(target_forward_keep_conditional(2, 5))
    t |> success(!target_forward_keep_conditional(-1, -9))
    let arr <- [10, 20, 30]
    t |> equal(target_forward_keep_raising(arr, 1), 21)
    t |> equal(target_forward_keep_struct_copy(Panel(size = Extent(w = 4, h = 5))), 5)
    t |> equal(target_forward_keep_annotated(2), 7)
}
