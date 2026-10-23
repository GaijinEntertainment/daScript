options gen2
options rtti
options no_coverage

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require dastest/testing_boost public
require strings
require daslib/strings_boost
require _inline_scope_helper
require _inline_unoptimized_scope_helper
require _optimized_body_helper

// structural FIRED proof + behavioral safety net for cross-module splices whose
// bodies use the callee module's private symbols (src/ast/ast_inline.cpp): the
// spliced body clone rides a generated `with (module <origin>)` wrapper, so its
// privates and require graph resolve exactly as at the callee's own compile.
// argument and prefix temps bind OUTSIDE the wrapper (the mirror rule): caller
// expressions keep resolving at the caller, proven by the g_shared collision test.

var g_shared = 555      // collides with _inline_scope_helper's g_shared on purpose

def has(s, sub : string) : bool => !empty(sub) && find(s, sub) >= 0

[export]
def target_scoped_add(a, b : int) : int {
    return scoped_add(a, b)
}

[export]
def target_scoped_pick(v : int) : int {
    return scoped_pick(v)
}

[export]
def target_mirror_rule() : int {
    return scoped_echo(g_shared)
}

[export]
def target_block_literal(src : array<int>) : int => scoped_filter_sum(src) $(v) => v % 2 == 0

[export]
def target_inside_with(a, b : int) : int {
    var r : int
    with (module _inline_scope_helper) {
        // the enclosing scope IS the resolution context - the splice lands bare here
        r = scoped_add(a, b)
    }
    return r
}

[export]
def target_visible_dependency() : string {
    var root : string
    with (module _inline_scope_helper) {
        root = must_inline_visibility_root()
    }
    return root
}

[export]
def target_generated_dependency_scope() : string =>
    must_inline_visibility_root()

[export]
def target_unoptimized_scope() : int => scoped_unoptimized_value()

[test]
def test_scope_wrapped_inline_fired(t : T?) {
    ast_gc_guard() {
        t |> run("[inline] callee with private use splices under with (module)") @(t : T?) {
            let b = body_of(t, "target_scoped_add")
            t |> success(!has(b, "scoped_add("), "call is gone: {b}")
            t |> success(has(b, "hidden_boost("), "private call spliced in: {b}")
        }
        t |> run("multi-return callee rides the same wrapped splice") @(t : T?) {
            let b = body_of(t, "target_scoped_pick")
            t |> success(!has(b, "scoped_pick("), "call is gone: {b}")
        }
        t |> run("call inside user with (module) splices bare") @(t : T?) {
            let b = body_of(t, "target_inside_with")
            t |> success(!has(b, "scoped_add("), "call is gone: {b}")
        }
        t |> run("caller scope resolves an unseen dependency") @(t : T?) {
            let b = body_of(t, "target_visible_dependency")
            t |> success(!has(b, "must_inline_visibility_root("),
                "call is gone: {b}")
        }
        t |> run("generated scope resolves an unseen dependency") @(t : T?) {
            let b = body_of(t, "target_generated_dependency_scope")
            t |> success(!has(b, "must_inline_visibility_root("),
                "call is gone: {b}")
        }
        t |> run("unoptimized callee keeps its inner scope") @(t : T?) {
            let b = body_of(t, "target_unoptimized_scope")
            t |> success(!has(b, "scoped_unoptimized_value("),
                "call is gone: {b}")
        }
    }
}

[test]
def test_scope_wrapped_inline_behavior(t : T?) {
    t |> run("private helper and private global values") @(t : T?) {
        t |> equal(target_scoped_add(1, 2), (1 + 7) + (2 + 7))
        t |> equal(target_scoped_pick(5), 5 + 7)
        t |> equal(target_scoped_pick(-5), -(5 + 7))
        t |> equal(target_inside_with(3, 4), (3 + 7) + (4 + 7))
        t |> success(!empty(target_visible_dependency()))
        t |> success(!empty(target_generated_dependency_scope()))
        t |> equal(target_unoptimized_scope(), 43)
    }
    t |> run("mirror rule: caller global wins over callee module's same-named global") @(t : T?) {
        // caller g_shared = 555 substitutes outside the wrap; body's own g_shared
        // read resolves to the helper's (100). wrong binding gives 100100 or 555555
        t |> equal(target_mirror_rule(), 555 * 1000 + 100)
    }
    t |> run("block-literal auto tier with private use in the body") @(t : T?) {
        // even numbers boosted by secret_bias: (2+7) + (4+7)
        t |> equal(target_block_literal([1, 2, 3, 4]), 9 + 11)
    }
    t |> run("private global write stays visible to the callee module") @(t : T?) {
        with (module _inline_scope_helper) {
            secret_bias = 20
        }
        t |> equal(target_scoped_add(1, 2), (1 + 20) + (2 + 20))
        with (module _inline_scope_helper) {
            secret_bias = 7
        }
    }
}
