options gen2
options indenting = 4
options rtti

require ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── if-then-else ────────────────────────────────────────────────────────

[test]
def test_if_then(t : T?) {
    var inscope expr <- qmacro($ { if (a < 5) { return b; }; })
    let r = qmatch_block(expr) $ {
        if (a < 5) {
            return b
        }
    }
    t |> success(r.matched, "if-then should match")
}

[test]
def test_if_then_else(t : T?) {
    var inscope expr <- qmacro($ { if (a < 5) { return b; } else { return c; }; })
    let r = qmatch_block(expr) $ {
        if (a < 5) {
            return b
        } else {
            return c
        }
    }
    t |> success(r.matched, "if-then-else should match")
}

[test]
def test_if_then_wrong_cond(t : T?) {
    var inscope expr <- qmacro($ { if (a < 5) { return b; }; })
    let r = qmatch_block(expr) $ {
        if (a > 5) {
            return b
        }
    }
    t |> success(!r.matched, "wrong condition should fail")
}

[test]
def test_if_then_wrong_body(t : T?) {
    var inscope expr <- qmacro($ { if (a < 5) { return b; }; })
    let r = qmatch_block(expr) $ {
        if (a < 5) {
            return c
        }
    }
    t |> success(!r.matched, "wrong body should fail")
}

[test]
def test_if_has_else_but_pattern_doesnt(t : T?) {
    // Actual has else, pattern doesn't — should fail because else is a non-null field
    var inscope expr <- qmacro($ { if (a < 5) { return b; } else { return c; }; })
    let r = qmatch_block(expr) $ {
        if (a < 5) {
            return b
        }
    }
    t |> success(!r.matched, "pattern without else should not match expr with else")
}

[test]
def test_if_no_else_but_pattern_has(t : T?) {
    // Actual has no else, pattern has else — should fail
    var inscope expr <- qmacro($ { if (a < 5) { return b; }; })
    let r = qmatch_block(expr) $ {
        if (a < 5) {
            return b
        } else {
            return c
        }
    }
    t |> success(!r.matched, "pattern with else should not match expr without else")
}

[test]
def test_if_capture_cond(t : T?) {
    // Capture the condition expression
    var inscope expr <- qmacro($ { if (a < 5) { return b; }; })
    var inscope cond : ExpressionPtr
    let r = qmatch_block(expr) $ {
        if ($e(cond)) {
            return b
        }
    }
    t |> success(r.matched, "should match with $e capture on condition")
    if (r.matched) {
        t |> equal(string(get_ptr(cond).__rtti), "ExprOp2")
    }
}

[test]
def test_if_capture_bodies(t : T?) {
    // Capture both branches
    var inscope expr <- qmacro($ { if (a < 5) { return b; } else { return c; }; })
    var inscope then_body : ExpressionPtr
    var inscope else_body : ExpressionPtr
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

// ── while loop ──────────────────────────────────────────────────────────

[test]
def test_while_match(t : T?) {
    var inscope expr <- qmacro($ { while (x > 0) { x --; }; })
    let r = qmatch_block(expr) $ {
        while (x > 0) {
            x --
        }
    }
    t |> success(r.matched, "while should match")
}

[test]
def test_while_wrong_cond(t : T?) {
    var inscope expr <- qmacro($ { while (x > 0) { x --; }; })
    let r = qmatch_block(expr) $ {
        while (x < 0) {
            x --
        }
    }
    t |> success(!r.matched, "wrong while condition should fail")
}

[test]
def test_while_capture_cond(t : T?) {
    var inscope expr <- qmacro($ { while (x > 0) { x --; }; })
    var inscope cond : ExpressionPtr
    let r = qmatch_block(expr) $ {
        while ($e(cond)) {
            x --
        }
    }
    t |> success(r.matched, "should capture while condition")
    if (r.matched) {
        t |> equal(string(get_ptr(cond).__rtti), "ExprOp2")
    }
}

// ── for loop ────────────────────────────────────────────────────────────

[test]
def test_for_match(t : T?) {
    var inscope expr <- qmacro($ { for (x in arr) { print(); }; })
    let r = qmatch_block(expr) $ {
        for (x in arr) {
            print()
        }
    }
    t |> success(r.matched, "for should match")
}

[test]
def test_for_wrong_body(t : T?) {
    var inscope expr <- qmacro($ { for (x in arr) { print(); }; })
    let r = qmatch_block(expr) $ {
        for (x in arr) {
            foo()
        }
    }
    t |> success(!r.matched, "wrong for body should fail")
}

[test]
def test_for_wrong_source(t : T?) {
    var inscope expr <- qmacro($ { for (x in arr) { print(); }; })
    let r = qmatch_block(expr) $ {
        for (x in other) {
            print()
        }
    }
    t |> success(!r.matched, "wrong for source should fail")
}

// ── return ──────────────────────────────────────────────────────────────

[test]
def test_return_value(t : T?) {
    var inscope expr <- qmacro($ { return a + b; })
    let r = qmatch_block(expr) $ {
        return a + b
    }
    t |> success(r.matched, "return value should match")
}

[test]
def test_return_wrong_value(t : T?) {
    var inscope expr <- qmacro($ { return a + b; })
    let r = qmatch_block(expr) $ {
        return a - b
    }
    t |> success(!r.matched, "wrong return value should fail")
}

[test]
def test_return_capture(t : T?) {
    var inscope expr <- qmacro($ { return a + b; })
    var inscope val : ExpressionPtr
    let r = qmatch_block(expr) $ {
        return $e(val)
    }
    t |> success(r.matched, "should capture return value")
    if (r.matched) {
        t |> equal(string(get_ptr(val).__rtti), "ExprOp2")
    }
}

// ── Mixed with wildcards ────────────────────────────────────────────────

[test]
def test_if_in_wildcard_sandwich(t : T?) {
    var inscope expr <- qmacro($ { print(); if (a < 5) { return b; }; foo(); })
    let r = qmatch_block(expr) $ {
        _wildcard()
        if (a < 5) {
            return b
        }
        _wildcard()
    }
    t |> success(r.matched, "should find if inside wildcards")
}

[test]
def test_while_after_wildcard(t : T?) {
    var inscope expr <- qmacro($ { var x = 10; while (x > 0) { x --; }; })
    let r = qmatch_block(expr) $ {
        _wildcard()
        while (x > 0) {
            x --
        }
    }
    t |> success(r.matched, "should find while after wildcard")
}

[export]
def main() {}
