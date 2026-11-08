options gen2
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── Block as call argument — basic matching ─────────────────────────

[test(description = "qmatch matches a call carrying a no-argument block against an identical pattern")]
def test_call_with_block_no_args(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar(); })
        let r = qmatch(expr, foo() $ { bar(); })
        t |> success(r.matched, "call with block should match")
    }
}

[test(description = "qmatch rejects a call whose block body differs from the pattern's")]
def test_call_with_block_wrong_body(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar(); })
        let r = qmatch(expr, foo() $ { baz(); })
        t |> success(!r.matched, "wrong block body should fail")
    }
}

[test(description = "qmatch rejects a block call whose callee name differs from the pattern's")]
def test_call_with_block_wrong_call_name(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar(); })
        let r = qmatch(expr, other() $ { bar(); })
        t |> success(!r.matched, "wrong call name should fail")
    }
}

// ── Block with typed arguments ──────────────────────────────────────

[test(description = "qmatch matches a block with a typed argument against an identical pattern")]
def test_call_with_typed_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $(a : int) { bar(a); })
        let r = qmatch(expr, foo() $(a : int) { bar(a); })
        t |> success(r.matched, "typed block should match")
    }
}

[test(description = "qmatch rejects a block whose argument name differs from the pattern's")]
def test_call_with_typed_block_wrong_arg_name(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $(a : int) { bar(a); })
        let r = qmatch(expr, foo() $(x : int) { bar(a); })
        t |> success(!r.matched, "wrong block arg name should fail")
    }
}

[test(description = "qmatch rejects a block whose argument type differs from the pattern's")]
def test_call_with_typed_block_wrong_arg_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $(a : int) { bar(a); })
        let r = qmatch(expr, foo() $(a : float) { bar(a); })
        t |> success(!r.matched, "wrong block arg type should fail")
    }
}

[test(description = "qmatch matches a block with two typed arguments against an identical pattern")]
def test_call_with_two_block_args(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $(a : int, b : float) { bar(a, b); })
        let r = qmatch(expr, foo() $(a : int, b : float) { bar(a, b); })
        t |> success(r.matched, "two typed block args should match")
    }
}

[test(description = "qmatch rejects a two-argument block whose second argument type differs")]
def test_call_with_two_block_args_wrong_second(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $(a : int, b : float) { bar(a, b); })
        let r = qmatch(expr, foo() $(a : int, b : string) { bar(a, b); })
        t |> success(!r.matched, "wrong second block arg type should fail")
    }
}

// ── Block with return type ──────────────────────────────────────────

[test(description = "qmatch matches a block with a declared return type against an identical pattern")]
def test_block_with_return_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(invoke(cb) $(a : int) : float { return 1.0; })
        let r = qmatch(expr, invoke(cb) $(a : int) : float { return 1.0; })
        t |> success(r.matched, "block with return type should match")
    }
}

[test(description = "qmatch rejects a block whose declared return type differs from the pattern's")]
def test_block_with_wrong_return_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(invoke(cb) $(a : int) : float { return 1.0; })
        let r = qmatch(expr, invoke(cb) $(a : int) : int { return 1.0; })
        t |> success(!r.matched, "wrong return type should fail")
    }
}

[test(description = "a pattern block that omits the return type matches a block with any return type")]
def test_block_no_return_pattern_matches_any(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(invoke(cb) $(a : int) : float { return 1.0; })
        let r = qmatch(expr, invoke(cb) $(a : int) { return 1.0; })
        t |> success(r.matched, "omitted return type should match any")
    }
}

// ── No-arg blocks ───────────────────────────────────────────────────

[test(description = "qmatch matches a no-argument block call against an identical pattern")]
def test_block_no_arg(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(defer() $ { print(); })
        let r = qmatch(expr, defer() $ { print(); })
        t |> success(r.matched, "no-arg block should match")
    }
}

[test(description = "qmatch rejects a no-argument block call whose body differs")]
def test_block_no_arg_wrong_body(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(defer() $ { print(); })
        let r = qmatch(expr, defer() $ { foo(); })
        t |> success(!r.matched, "wrong no-arg block body should fail")
    }
}

// ── Block with multiple statements ──────────────────────────────────

[test(description = "qmatch matches a block of several statements in order")]
def test_block_multi_stmts(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar(); baz(); })
        let r = qmatch(expr, foo() $ { bar(); baz(); })
        t |> success(r.matched, "multi-stmt block should match")
    }
}

[test(description = "qmatch rejects a block whose statements appear in a different order")]
def test_block_multi_stmts_wrong_order(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar(); baz(); })
        let r = qmatch(expr, foo() $ { baz(); bar(); })
        t |> success(!r.matched, "wrong stmt order should fail")
    }
}

[test(description = "qmatch rejects a pattern block with fewer statements than the target block")]
def test_block_multi_stmts_extra(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar(); baz(); })
        let r = qmatch(expr, foo() $ { bar(); })
        t |> success(!r.matched, "fewer pattern stmts should fail")
    }
}

// ── Call with args before block ─────────────────────────────────────

[test(description = "qmatch matches a call that has both plain arguments and a block")]
def test_call_with_args_and_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(1, 2) $ { bar(); })
        let r = qmatch(expr, foo(1, 2) $ { bar(); })
        t |> success(r.matched, "call with args + block should match")
    }
}

[test(description = "qmatch rejects a call with a block when a plain argument differs")]
def test_call_with_wrong_args_and_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(1, 2) $ { bar(); })
        let r = qmatch(expr, foo(1, 3) $ { bar(); })
        t |> success(!r.matched, "wrong call arg should fail")
    }
}

// ── Block with var argument ─────────────────────────────────────────

[test(description = "qmatch matches a block whose argument is declared var")]
def test_block_var_arg(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(build_string() $(var writer) { write(writer, "hello"); })
        let r = qmatch(expr, build_string() $(var writer) { write(writer, "hello"); })
        t |> success(r.matched, "block with var arg should match")
    }
}

// ── Nested blocks ───────────────────────────────────────────────────

[test(description = "qmatch matches a block nested inside another block")]
def test_nested_blocks(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar() $(x : int) { baz(x); }; })
        let r = qmatch(expr, foo() $ { bar() $(x : int) { baz(x); }; })
        t |> success(r.matched, "nested blocks should match")
    }
}

[test(description = "qmatch rejects nested blocks whose inner body differs")]
def test_nested_blocks_wrong_inner_body(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar() $(x : int) { baz(x); }; })
        let r = qmatch(expr, foo() $ { bar() $(x : int) { other(x); }; })
        t |> success(!r.matched, "wrong inner block body should fail")
    }
}

[test(description = "qmatch rejects nested blocks whose inner argument name differs")]
def test_nested_blocks_wrong_inner_arg(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar() $(x : int) { baz(x); }; })
        let r = qmatch(expr, foo() $ { bar() $(y : int) { baz(x); }; })
        t |> success(!r.matched, "wrong inner block arg name should fail")
    }
}

[test(description = "qmatch rejects nested blocks whose inner argument type differs")]
def test_nested_blocks_wrong_inner_arg_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo() $ { bar() $(x : int) { baz(x); }; })
        let r = qmatch(expr, foo() $ { bar() $(x : float) { baz(x); }; })
        t |> success(!r.matched, "wrong inner block arg type should fail")
    }
}

// ── Three-level nesting ─────────────────────────────────────────────

[test(description = "qmatch matches blocks nested three levels deep")]
def test_three_level_nesting(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a() $ { b() $ { c() $ { print(); }; }; })
        let r = qmatch(expr, a() $ { b() $ { c() $ { print(); }; }; })
        t |> success(r.matched, "three-level nested blocks should match")
    }
}

[test(description = "qmatch rejects three-level nesting when only the deepest body differs")]
def test_three_level_nesting_wrong_deepest(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a() $ { b() $ { c() $ { print(); }; }; })
        let r = qmatch(expr, a() $ { b() $ { c() $ { foo(); }; }; })
        t |> success(!r.matched, "wrong deepest block body should fail")
    }
}

// ── Blocks inside qmatch_block ──────────────────────────────────────

[test(description = "qmatch_block matches a block call inside the matched block")]
def test_block_in_qmatch_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo() $ { bar(); }; })
        let r = qmatch_block(expr) $ {
            foo() $ {
                bar()
            }
        }
        t |> success(r.matched, "block inside qmatch_block should match")
    }
}

[test(description = "qmatch_block rejects a nested block call whose body differs")]
def test_block_in_qmatch_block_wrong(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo() $ { bar(); }; })
        let r = qmatch_block(expr) $ {
            foo() $ {
                baz()
            }
        }
        t |> success(!r.matched, "wrong block inside qmatch_block should fail")
    }
}

[test(description = "qmatch_block matches a typed-argument block call inside the matched block")]
def test_typed_block_in_qmatch_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo() $(x : int) { bar(x); }; })
        let r = qmatch_block(expr) $ {
            foo() $(x : int) {
                bar(x)
            }
        }
        t |> success(r.matched, "typed block inside qmatch_block should match")
    }
}

// ── Block after wildcard ────────────────────────────────────────────

[test(description = "qmatch_block matches a block call that follows a wildcard statement")]
def test_block_after_wildcard(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { setup(); foo() $(x : int) { bar(x); }; })
        let r = qmatch_block(expr) $ {
            _wildcard()
            foo() $(x : int) {
                bar(x)
            }
        }
        t |> success(r.matched, "block after wildcard should match")
    }
}

// ── invoke() ────────────────────────────────────────────────────────

[test(description = "qmatch matches invoke with a callee and a block")]
def test_invoke_with_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(invoke(cb) $ { print(); })
        let r = qmatch(expr, invoke(cb) $ { print(); })
        t |> success(r.matched, "invoke with block should match")
    }
}

[test(description = "qmatch rejects invoke with a block when the first argument differs")]
def test_invoke_wrong_first_arg(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(invoke(cb) $ { print(); })
        let r = qmatch(expr, invoke(other) $ { print(); })
        t |> success(!r.matched, "invoke with wrong first arg should fail")
    }
}

// ── Sibling blocks at same level ────────────────────────────────────

[test(description = "qmatch_block matches two sibling block calls in sequence")]
def test_sibling_blocks(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo() $ { bar(); }; baz() $(x : int) { qux(x); }; })
        let r = qmatch_block(expr) $ {
            foo() $ {
                bar()
            }
            baz() $(x : int) {
                qux(x)
            }
        }
        t |> success(r.matched, "sibling blocks should match")
    }
}

[test(description = "qmatch_block rejects sibling blocks when the second block's argument type differs")]
def test_sibling_blocks_wrong_second(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo() $ { bar(); }; baz() $(x : int) { qux(x); }; })
        let r = qmatch_block(expr) $ {
            foo() $ {
                bar()
            }
            baz() $(x : float) {
                qux(x)
            }
        }
        t |> success(!r.matched, "wrong second sibling block arg type should fail")
    }
}
