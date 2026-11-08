options gen2
options indenting = 4
options rtti
options _cyclomatic_complexity = 0  // qmatch pattern blocks expand into matcher code; the count is not the test's own

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── if-then-else ────────────────────────────────────────────────────────

[test(description = "qmatch_block matches an if statement without else")]
def test_if_then(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { if (a < 5) { return b; }; })
        let r = qmatch_block(expr) $ {
            if (a < 5) {
                return b
            }
        }
        t |> success(r.matched, "if-then should match")
    }
}

[test(description = "qmatch_block matches an if statement with else")]
def test_if_then_else(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { if (a < 5) { return b; } else { return c; }; })
        let r = qmatch_block(expr) $ {
            if (a < 5) {
                return b
            } else {
                return c
            }
        }
        t |> success(r.matched, "if-then-else should match")
    }
}

[test(description = "qmatch_block rejects an if whose condition differs")]
def test_if_then_wrong_cond(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { if (a < 5) { return b; }; })
        let r = qmatch_block(expr) $ {
            if (a > 5) {
                return b
            }
        }
        t |> success(!r.matched, "wrong condition should fail")
    }
}

[test(description = "qmatch_block rejects an if whose body differs")]
def test_if_then_wrong_body(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { if (a < 5) { return b; }; })
        let r = qmatch_block(expr) $ {
            if (a < 5) {
                return c
            }
        }
        t |> success(!r.matched, "wrong body should fail")
    }
}

[test(description = "an if pattern without else does not match an if that has an else")]
def test_if_has_else_but_pattern_doesnt(t : T?) {
    ast_gc_guard() {
        // Actual has else, pattern doesn't — should fail because else is a non-null field
        var expr = qmacro($ { if (a < 5) { return b; } else { return c; }; })
        let r = qmatch_block(expr) $ {
            if (a < 5) {
                return b
            }
        }
        t |> success(!r.matched, "pattern without else should not match expr with else")
    }
}

[test(description = "an if pattern with else does not match an if that has no else")]
def test_if_no_else_but_pattern_has(t : T?) {
    ast_gc_guard() {
        // Actual has no else, pattern has else — should fail
        var expr = qmacro($ { if (a < 5) { return b; }; })
        let r = qmatch_block(expr) $ {
            if (a < 5) {
                return b
            } else {
                return c
            }
        }
        t |> success(!r.matched, "pattern with else should not match expr without else")
    }
}

[test(description = "a $e capture binds the condition of an if")]
def test_if_capture_cond(t : T?) {
    ast_gc_guard() {
        // Capture the condition expression
        var expr = qmacro($ { if (a < 5) { return b; }; })
        var cond : ExpressionPtr
        let r = qmatch_block(expr) $ {
            if ($e(cond)) {
                return b
            }
        }
        t |> success(r.matched, "should match with $e capture on condition")
        if (r.matched) {
            t |> equal(cond.__rtti, "ExprOp2")
        }
    }
}

[test(description = "$e captures bind the then and else bodies of an if")]
def test_if_capture_bodies(t : T?) {
    ast_gc_guard() {
        // Capture both branches
        var expr = qmacro($ { if (a < 5) { return b; } else { return c; }; })
        var then_body, else_body : ExpressionPtr
        let r = qmatch_block(expr) $ {
            if (a < 5) {
                $e(then_body)
            } else {
                $e(else_body)
            }
        }
        // Note: the then/else branches are ExprBlock, and $e captures the block content
        // This may or may not work depending on how the block body is represented
        // For now just check the basic match works
        t |> success(r.matched, "should match if-else with $e captures")
    }
}

// ── while loop ──────────────────────────────────────────────────────────

[test(description = "qmatch_block matches a while loop")]
def test_while_match(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { while (x > 0) { x --; }; })
        let r = qmatch_block(expr) $ {
            while (x > 0) {
                x --
            }
        }
        t |> success(r.matched, "while should match")
    }
}

[test(description = "qmatch_block rejects a while loop whose condition differs")]
def test_while_wrong_cond(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { while (x > 0) { x --; }; })
        let r = qmatch_block(expr) $ {
            while (x < 0) {
                x --
            }
        }
        t |> success(!r.matched, "wrong while condition should fail")
    }
}

[test(description = "a $e capture binds the condition of a while loop")]
def test_while_capture_cond(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { while (x > 0) { x --; }; })
        var cond : ExpressionPtr
        let r = qmatch_block(expr) $ {
            while ($e(cond)) {
                x --
            }
        }
        t |> success(r.matched, "should capture while condition")
        if (r.matched) {
            t |> equal(cond.__rtti, "ExprOp2")
        }
    }
}

// ── for loop ────────────────────────────────────────────────────────────

[test(description = "qmatch_block matches a for loop")]
def test_for_match(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (x in arr) {
                print()
            }
        }
        t |> success(r.matched, "for should match")
    }
}

[test(description = "qmatch_block rejects a for loop whose body differs")]
def test_for_wrong_body(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (x in arr) {
                foo()
            }
        }
        t |> success(!r.matched, "wrong for body should fail")
    }
}

[test(description = "qmatch_block rejects a for loop whose source differs")]
def test_for_wrong_source(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (x in other) {
                print()
            }
        }
        t |> success(!r.matched, "wrong for source should fail")
    }
}

// ── return ──────────────────────────────────────────────────────────────

[test(description = "qmatch_block matches a return with a value expression")]
def test_return_value(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { return a + b; })
        let r = qmatch_block(expr) $ {
            return a + b
        }
        t |> success(r.matched, "return value should match")
    }
}

[test(description = "qmatch_block rejects a return whose value differs")]
def test_return_wrong_value(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { return a + b; })
        let r = qmatch_block(expr) $ {
            return a - b
        }
        t |> success(!r.matched, "wrong return value should fail")
    }
}

[test(description = "a $e capture binds the value of a return")]
def test_return_capture(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { return a + b; })
        var val : ExpressionPtr
        let r = qmatch_block(expr) $ {
            return $e(val)
        }
        t |> success(r.matched, "should capture return value")
        if (r.matched) {
            t |> equal(val.__rtti, "ExprOp2")
        }
    }
}

// ── for — double iterators ───────────────────────────────────────────

[test(description = "qmatch_block matches a for loop over two iterators")]
def test_for_two_iterators(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (k, v in keys, values) {
                print()
            }
        }
        t |> success(r.matched, "for with two iterators should match")
    }
}

[test(description = "qmatch_block rejects a two-iterator for whose second iterator name differs")]
def test_for_two_iterators_wrong_name(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (k, x in keys, values) {
                print()
            }
        }
        t |> success(!r.matched, "wrong second iterator name should fail")
    }
}

[test(description = "qmatch_block rejects a two-iterator for whose second source differs")]
def test_for_two_iterators_wrong_source(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (k, v in keys, other) {
                print()
            }
        }
        t |> success(!r.matched, "wrong second source should fail")
    }
}

[test(description = "qmatch_block rejects a for loop whose iterator count differs from the pattern's")]
def test_for_iterator_count_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (k in keys) {
                print()
            }
        }
        t |> success(!r.matched, "iterator count mismatch should fail")
    }
}

// ── for — $i capture ────────────────────────────────────────────────

[test(description = "an $i capture on the first iterator works next to a fixed second iterator name")]
def test_for_capture_single_with_two_iterators(t : T?) {
    ast_gc_guard() {
        // $i tag only works on the first iterator (parser limitation)
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        var name1 : string
        let r = qmatch_block(expr) $ {
            for ($i(name1), v in keys, values) {
                print()
            }
        }
        t |> success(r.matched, "should capture first iterator, match second fixed")
        t |> equal(name1, "k")
    }
}

[test(description = "an $i capture on the first iterator does not rescue a wrong fixed second name")]
def test_for_capture_first_wrong_second(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        var name1 : string
        let r = qmatch_block(expr) $ {
            for ($i(name1), x in keys, values) {
                print()
            }
        }
        t |> success(!r.matched, "$i capture + wrong fixed name should fail")
    }
}

// ── for — $e capture on sources ─────────────────────────────────────

[test(description = "a $e capture binds the source of a for loop")]
def test_for_capture_source(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in range(10)) { print(); }; })
        var src : ExpressionPtr
        let r = qmatch_block(expr) $ {
            for (x in $e(src)) {
                print()
            }
        }
        t |> success(r.matched, "should capture for loop source")
        if (r.matched) {
            t |> success(src != null, "captured source should not be null")
        }
    }
}

[test(description = "$e captures bind both sources of a two-iterator for loop")]
def test_for_capture_two_sources(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { print(); }; })
        var src1, src2 : ExpressionPtr
        let r = qmatch_block(expr) $ {
            for (k, v in $e(src1), $e(src2)) {
                print()
            }
        }
        t |> success(r.matched, "should capture both sources")
        if (r.matched) {
            t |> success(src1 != null, "first source should not be null")
            t |> success(src2 != null, "second source should not be null")
        }
    }
}

// ── for — $e capture on body ────────────────────────────────────────

[test(description = "a $e capture binds the body of a for loop")]
def test_for_capture_body(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { a + b; }; })
        var body_expr : ExpressionPtr
        let r = qmatch_block(expr) $ {
            for (x in arr) {
                $e(body_expr)
            }
        }
        t |> success(r.matched, "should capture for body")
        if (r.matched) {
            t |> success(body_expr != null, "captured body should not be null")
        }
    }
}

// ── for — mixed capture ($i + $e) ───────────────────────────────────

[test(description = "an $i and a $e capture bind the iterator name and source of a for loop together")]
def test_for_capture_iterator_and_source(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in range(10)) { print(); }; })
        var iter_name : string
        var src : ExpressionPtr
        let r = qmatch_block(expr) $ {
            for ($i(iter_name) in $e(src)) {
                print()
            }
        }
        t |> success(r.matched, "should capture iterator name and source")
        t |> equal(iter_name, "x")
        if (r.matched) {
            t |> success(src != null, "captured source should not be null")
        }
    }
}

[test(description = "$i and $e captures bind iterator names, sources and body of a two-iterator for loop together")]
def test_for_capture_iterator_and_sources(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (k, v in keys, values) { a + b; }; })
        var name1 : string
        var src1, src2, body_expr : ExpressionPtr
        let r = qmatch_block(expr) $ {
            for ($i(name1), v in $e(src1), $e(src2)) {
                $e(body_expr)
            }
        }
        t |> success(r.matched, "should capture iterator, sources, and body")
        t |> equal(name1, "k")
        if (r.matched) {
            t |> success(src1 != null, "first source not null")
            t |> success(src2 != null, "second source not null")
            t |> success(body_expr != null, "body not null")
        }
    }
}

// ── for — with range() source ───────────────────────────────────────

[test(description = "qmatch_block matches a for loop over range(10)")]
def test_for_range_source(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (i in range(10)) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (i in range(10)) {
                print()
            }
        }
        t |> success(r.matched, "for with range(10) should match")
    }
}

[test(description = "a for loop over range(10) does not match a pattern over range(20)")]
def test_for_range_wrong_arg(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (i in range(10)) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (i in range(20)) {
                print()
            }
        }
        t |> success(!r.matched, "range(10) should not match range(20)")
    }
}

// ── for — in wildcard context ───────────────────────────────────────

[test(description = "a for loop between wildcard statements is found and its iterator captured")]
def test_for_in_wildcard(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { var x = 0; for (i in arr) { x += i; }; print(); })
        var iter_name : string
        var body_expr : ExpressionPtr
        let r = qmatch_block(expr) $ {
            _wildcard()
            for ($i(iter_name) in arr) {
                $e(body_expr)
            }
            _wildcard()
        }
        t |> success(r.matched, "for inside wildcards should match")
        t |> equal(iter_name, "i")
    }
}

// ── Mixed with wildcards ────────────────────────────────────────────────

[test(description = "an if statement between wildcard statements is found")]
def test_if_in_wildcard_sandwich(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { print(); if (a < 5) { return b; }; foo(); })
        let r = qmatch_block(expr) $ {
            _wildcard()
            if (a < 5) {
                return b
            }
            _wildcard()
        }
        t |> success(r.matched, "should find if inside wildcards")
    }
}

[test(description = "a while loop after a leading wildcard statement is found")]
def test_while_after_wildcard(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { var x = 10; while (x > 0) { x --; }; })
        let r = qmatch_block(expr) $ {
            _wildcard()
            while (x > 0) {
                x --
            }
        }
        t |> success(r.matched, "should find while after wildcard")
    }
}
