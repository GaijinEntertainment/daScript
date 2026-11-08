options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── Helpers to build test expressions ───────────────────────────────────

def make_int(val : int) : ExpressionPtr => qmacro($v(val))

def make_bool(val : bool) : ExpressionPtr => qmacro($v(val))

def make_add_ab() : ExpressionPtr => qmacro(a + b)

def make_add_xb() : ExpressionPtr => qmacro(x + b)

// ── Basic matching ──────────────────────────────────────────────────────

[test(description = "the QmResult helpers report matched true with no error for a success and the recorded error for a failure")]
def test_result_types(t : T?) {
    ast_gc_guard() {
        let r = qm_ok()
        t |> equal(r.matched, true)
        let r2 = qm_fail_null()
        t |> equal(r2.matched, false)
        t |> equal(int(r2.error), int(QMatchError.null_expression))
    }
}

[test(description = "qmatch matches an int constant against the same value")]
def test_const_int_match(t : T?) {
    ast_gc_guard() {
        var expr = make_int(42)
        t |> success(qmatch(expr, 42).matched, "42 should match 42")
    }
}

[test(description = "qmatch rejects an int constant against a different value")]
def test_const_int_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = make_int(42)
        let r = qmatch(expr, 99)
        t |> success(!r.matched, "42 should not match 99")
        t |> equal(int(r.error), int(QMatchError.field_mismatch))
    }
}

[test(description = "qmatch rejects a bool pattern against an int constant")]
def test_rtti_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = make_int(42)
        let r = qmatch(expr, true)
        t |> success(!r.matched, "int should not match bool pattern")
        t |> equal(int(r.error), int(QMatchError.rtti_mismatch))
    }
}

[test(description = "qmatch matches a bool constant against the same value")]
def test_const_bool_match(t : T?) {
    ast_gc_guard() {
        var expr = make_bool(true)
        t |> success(qmatch(expr, true).matched, "true == true")
    }
}

[test(description = "qmatch rejects a bool constant against the opposite value")]
def test_const_bool_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = make_bool(false)
        t |> success(!qmatch(expr, true).matched, "false != true")
    }
}

[test(description = "qmatch matches a binary operator with identical operands")]
def test_op2_match(t : T?) {
    ast_gc_guard() {
        var expr <- make_add_ab()
        t |> success(qmatch(expr, a + b).matched, "a+b matches a+b")
    }
}

[test(description = "qmatch rejects a binary operator of a different kind")]
def test_op2_wrong_op(t : T?) {
    ast_gc_guard() {
        var expr <- make_add_ab()
        t |> success(!qmatch(expr, a - b).matched, "a+b does not match a-b")
    }
}

[test(description = "qmatch rejects a binary operator whose operand variable differs")]
def test_op2_wrong_var(t : T?) {
    ast_gc_guard() {
        var expr <- make_add_xb()
        t |> success(!qmatch(expr, a + b).matched, "x+b does not match a+b")
    }
}

// ── $e capture ──────────────────────────────────────────────────────────

[test(description = "$e captures bind both operands of a binary operator")]
def test_capture_e_op2(t : T?) {
    ast_gc_guard() {
        // Match a + b, capture left and right as ExpressionPtr
        var expr <- make_add_ab()
        var left, right : ExpressionPtr
        let r = qmatch(expr, $e(left) + $e(right))
        t |> success(r.matched, "should match with $e captures")
        if (r.matched) {
            t |> equal(left.__rtti, "ExprVar")
            t |> equal(right.__rtti, "ExprVar")
        }
    }
}

[test(description = "an operator pattern with $e operands rejects a constant")]
def test_capture_e_rtti_mismatch(t : T?) {
    ast_gc_guard() {
        // $e captures should still fail on RTTI mismatch at the outer level
        var expr = make_int(42)
        var captured : ExpressionPtr
        let r = qmatch(expr, $e(captured) + $e(captured))
        t |> success(!r.matched, "int should not match op2 pattern even with $e")
        t |> equal(int(r.error), int(QMatchError.rtti_mismatch))
    }
}

// ── $v capture ──────────────────────────────────────────────────────────

[test(description = "a $v capture extracts the value of an int constant")]
def test_capture_v_int(t : T?) {
    ast_gc_guard() {
        var expr = make_int(42)
        var val : int
        let r = qmatch(expr, $v(val))
        t |> success(r.matched, "should match ExprConstInt with $v")
        if (r.matched) {
            t |> equal(val, 42)
        }
    }
}

// TODO: $v with wrong type (float var on int expr) — currently only extracts int

// ── $i capture ──────────────────────────────────────────────────────────

// ── $i capture ──────────────────────────────────────────────────────────

[test(description = "an $i capture extracts the name of a variable")]
def test_capture_i_var(t : T?) {
    ast_gc_guard() {
        // Extract variable name from ExprVar
        var expr = qmacro(my_variable)
        var name : string
        let r = qmatch(expr, $i(name))
        t |> success(r.matched, "should match ExprVar with $i")
        if (r.matched) {
            t |> equal(name, "my_variable")
        }
    }
}

[test(description = "$i captures extract the variable names of both operands of an operator")]
def test_capture_i_from_op2(t : T?) {
    ast_gc_guard() {
        // Extract variable names from a + b
        var expr <- make_add_ab()
        var left_name, right_name : string
        let r = qmatch(expr, $i(left_name) + $i(right_name))
        t |> success(r.matched, "should match a + b with $i captures")
        if (r.matched) {
            t |> equal(left_name, "a")
            t |> equal(right_name, "b")
        }
    }
}

// TODO: $c(var) for call name extraction (Phase 2)

// ── Null expression ─────────────────────────────────────────────────────

[test(description = "qmatch fails without crashing on a null expression")]
def test_null_expr_fails(t : T?) {
    ast_gc_guard() {
        var expr : ExpressionPtr
        let r = qmatch(expr, 42)
        t |> success(!r.matched, "null expr should fail")
        t |> equal(int(r.error), int(QMatchError.null_expression))
    }
}

// ── String constant ─────────────────────────────────────────────────────

[test(description = "qmatch matches a string constant against the same text")]
def test_const_string_match(t : T?) {
    ast_gc_guard() {
        var expr = qmacro("hello")
        t |> success(qmatch(expr, "hello").matched, "hello == hello")
    }
}

[test(description = "qmatch rejects a string constant against different text")]
def test_const_string_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro("hello")
        t |> success(!qmatch(expr, "world").matched, "hello != world")
    }
}

// ── Float constant ──────────────────────────────────────────────────────

[test(description = "qmatch matches a float constant against the same value")]
def test_const_float_match(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(3.14)
        t |> success(qmatch(expr, 3.14).matched, "3.14 == 3.14")
    }
}

[test(description = "qmatch rejects a float constant against a different value")]
def test_const_float_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(3.14)
        t |> success(!qmatch(expr, 2.71).matched, "3.14 != 2.71")
    }
}

// ── Cross-type mismatch ─────────────────────────────────────────────────

[test(description = "qmatch rejects a string pattern against an int constant")]
def test_int_vs_string(t : T?) {
    ast_gc_guard() {
        var expr = make_int(42)
        t |> success(!qmatch(expr, "hello").matched, "int vs string")
    }
}

[test(description = "qmatch rejects an int pattern against a bool constant")]
def test_bool_vs_int(t : T?) {
    ast_gc_guard() {
        var expr = make_bool(true)
        t |> success(!qmatch(expr, 42).matched, "bool vs int")
    }
}

[test(description = "qmatch rejects a constant pattern against a binary operator")]
def test_op2_vs_const(t : T?) {
    ast_gc_guard() {
        var expr <- make_add_ab()
        t |> success(!qmatch(expr, 42).matched, "op2 vs const")
    }
}

[test(description = "qmatch rejects an operator pattern against a constant")]
def test_const_vs_op2(t : T?) {
    ast_gc_guard() {
        var expr = make_int(42)
        t |> success(!qmatch(expr, a + b).matched, "const vs op2")
    }
}

// ── qmatch_block ────────────────────────────────────────────────────────

def make_block_ab() : ExpressionPtr {
    // Block: { a + b; return c; }
    return qmacro($ { a + b; return c; })
}

[test(description = "qmatch_block matches a block statement for statement")]
def test_block_exact_match(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_ab()
        let r = qmatch_block(expr) $ {
            a + b
            return c
        }
        t |> success(r.matched, "exact block match should succeed")
    }
}

[test(description = "qmatch_block rejects a block whose statement differs")]
def test_block_wrong_stmt(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_ab()
        let r = qmatch_block(expr) $ {
            a - b
            return c
        }
        t |> success(!r.matched, "wrong statement should fail")
    }
}

[test(description = "qmatch_block rejects a block that has more statements than the pattern without a wildcard")]
def test_block_extra_stmt(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_ab()
        let r = qmatch_block(expr) $ {
            a + b
        }
        t |> success(!r.matched, "fewer pattern stmts without wildcard should fail")
    }
}

[test(description = "a trailing wildcard matches the remaining statements of a block")]
def test_block_wildcard_trailing(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_ab()
        let r = qmatch_block(expr) $ {
            a + b
            _wildcard()
        }
        t |> success(r.matched, "wildcard should match remaining stmts")
    }
}

[test(description = "a leading wildcard followed by a fixed statement matches a block")]
def test_block_wildcard_leading(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_ab()
        let r = qmatch_block(expr) $ {
            _wildcard()
            return c
        }
        t |> success(r.matched, "leading wildcard + fixed should match")
    }
}

// ── Wildcard edge cases ─────────────────────────────────────────────────

def make_block_1() : ExpressionPtr {
    // { a + b }
    return qmacro($ { a + b; })
}

def make_block_2() : ExpressionPtr {
    // { print(); a + b }
    return qmacro($ { print(); a + b; })
}

def make_block_3() : ExpressionPtr {
    // { print(); print(); a + b }
    return qmacro($ { print(); print(); a + b; })
}

def make_block_4() : ExpressionPtr {
    // { a + b; return c; print() }
    return qmacro($ { a + b; return c; print(); })
}

// _wildcard() before a fixed pattern — zero prefix

[test(description = "a leading wildcard matches zero statements before the fixed one")]
def test_wildcard_zero_prefix(t : T?) {
    ast_gc_guard() {
        // Pattern: _wildcard(); a + b  — actual has just a + b (wildcard matches 0)
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            _wildcard()
            a + b
        }
        t |> success(r.matched, "wildcard(0) + fixed should match single stmt")
    }
}

[test(description = "a leading wildcard skips one statement before the fixed one")]
def test_wildcard_one_prefix(t : T?) {
    ast_gc_guard() {
        // Pattern: _wildcard(); a + b  — actual: print(); a + b
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            _wildcard()
            a + b
        }
        t |> success(r.matched, "wildcard(1) + fixed should match")
    }
}

[test(description = "a leading wildcard skips two statements before the fixed one")]
def test_wildcard_two_prefix(t : T?) {
    ast_gc_guard() {
        // Pattern: _wildcard(); a + b  — actual: print(); print(); a + b
        var expr <- make_block_3()
        let r = qmatch_block(expr) $ {
            _wildcard()
            a + b
        }
        t |> success(r.matched, "wildcard(2) + fixed should match")
    }
}

// _wildcard() after a fixed pattern — consume trailing

[test(description = "a fixed statement followed by a wildcard matches a block with several trailing statements")]
def test_wildcard_trailing_multi(t : T?) {
    ast_gc_guard() {
        // Pattern: a + b; _wildcard()  — actual: a + b; return c; print()
        var expr <- make_block_4()
        let r = qmatch_block(expr) $ {
            a + b
            _wildcard()
        }
        t |> success(r.matched, "fixed + wildcard should match with trailing stmts")
    }
}

// No wildcard — must match exactly

[test(description = "a single fixed statement matches a one-statement block")]
def test_no_wildcard_exact(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            a + b
        }
        t |> success(r.matched, "exact single stmt should match")
    }
}

[test(description = "a single fixed statement does not match a two-statement block")]
def test_no_wildcard_too_few(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            a + b
        }
        t |> success(!r.matched, "single pattern on 2-stmt block without wildcard should fail")
    }
}

// _wildcard() sandwiching a fixed pattern

[test(description = "a wildcard on each side of a fixed statement matches a longer block")]
def test_wildcard_sandwich(t : T?) {
    ast_gc_guard() {
        // Pattern: _wildcard(); a + b; _wildcard()  — actual: print(); a + b; return c; print()
        var expr <- make_block_4()
        let r = qmatch_block(expr) $ {
            _wildcard()
            a + b
            _wildcard()
        }
        t |> success(r.matched, "wildcard + fixed + wildcard should match")
    }
}

// _wildcard() with no fixed — matches any block

[test(description = "a bare wildcard matches any block")]
def test_wildcard_only(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_3()
        let r = qmatch_block(expr) $ {
            _wildcard()
        }
        t |> success(r.matched, "bare wildcard should match any block")
    }
}

// Empty block

[test(description = "a bare wildcard matches an empty block")]
def test_wildcard_empty_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ {})
        let r = qmatch_block(expr) $ {
            _wildcard()
        }
        t |> success(r.matched, "wildcard should match empty block")
    }
}

// _wildcard1 — requires at least one

[test(description = "_wildcard1 before the only statement fails because it needs at least one prefix statement")]
def test_wildcard1_zero_fails(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            _wildcard1()
            a + b
        }
        t |> success(!r.matched, "wildcard1 before the only stmt should fail (needs 1+ prefix)")
    }
}

[test(description = "_wildcard1 matches exactly one prefix statement")]
def test_wildcard1_one_passes(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            _wildcard1()
            a + b
        }
        t |> success(r.matched, "wildcard1 with 1 prefix should pass")
    }
}

[test(description = "_wildcard1 scans forward over two prefix statements")]
def test_wildcard1_two_prefix(t : T?) {
    ast_gc_guard() {
        // { print(); print(); a + b } — _wildcard1 should scan forward to find a + b
        var expr <- make_block_3()
        let r = qmatch_block(expr) $ {
            _wildcard1()
            a + b
        }
        t |> success(r.matched, "wildcard1 with 2 prefix should pass (scan forward)")
    }
}

[test(description = "_wildcard1 on each side of a fixed statement matches")]
def test_wildcard1_sandwich(t : T?) {
    ast_gc_guard() {
        // { print(); print(); a + b; return c; print() } — _wildcard1 on both sides
        var expr <- make_block_4()
        let r = qmatch_block(expr) $ {
            _wildcard1()
            return c
            _wildcard1()
        }
        t |> success(r.matched, "wildcard1 sandwich should match")
    }
}

[test(description = "_wildcard1 before a fixed statement at position 0 fails")]
def test_wildcard1_sandwich_too_few_before(t : T?) {
    ast_gc_guard() {
        // { a + b; return c; print() } — a + b is at pos 0, no room for 1+ before it
        var expr <- make_block_4()
        let r = qmatch_block(expr) $ {
            _wildcard1()
            a + b
            _wildcard()
        }
        t |> success(!r.matched, "wildcard1 needs 1+ before a+b but it is at position 0")
    }
}

// _any — exactly one

[test(description = "_any before a fixed statement matches exactly one prefix statement")]
def test_any_exact(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            _any()
            a + b
        }
        t |> success(r.matched, "any + fixed on 2-stmt block should match")
    }
}

[test(description = "_any before the only statement fails because it needs exactly one")]
def test_any_zero_fails(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            _any()
            a + b
        }
        t |> success(!r.matched, "any before the only stmt should fail (needs exactly 1)")
    }
}

// _wildcard not found — fixed pattern doesn't exist in block

[test(description = "scanning for a statement the block does not contain fails")]
def test_wildcard_not_found(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_3()
        let r = qmatch_block(expr) $ {
            _wildcard()
            return c
        }
        t |> success(!r.matched, "scanning for nonexistent stmt should fail")
    }
}

// Multiple fixed patterns with wildcards between

[test(description = "two fixed statements separated by wildcards match in order")]
def test_two_fixed_with_wildcards(t : T?) {
    ast_gc_guard() {
        var expr <- make_block_4()
        let r = qmatch_block(expr) $ {
            a + b
            _wildcard()
            print()
        }
        if (!r.matched) {
            if (r.expr != null) {
                t |> success(false, "error={int(r.error)} rtti={qm_rtti(r.expr)}")
            } else {
                t |> success(false, "error={int(r.error)} expr=null")
            }
        } else {
            t |> success(true)
        }
    }
}

// ── _optional — zero or one ──────────────────────────────────────────

[test(description = "_optional before a fixed statement matches zero prefix statements")]
def test_optional_zero(t : T?) {
    ast_gc_guard() {
        // { a + b } — _optional() matches 0, then fixed matches a + b
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            _optional()
            a + b
        }
        t |> success(r.matched, "optional(0) + fixed on 1-stmt block should match")
    }
}

[test(description = "_optional before a fixed statement matches one prefix statement")]
def test_optional_one(t : T?) {
    ast_gc_guard() {
        // { print(); a + b } — _optional() matches print(), then fixed matches a + b
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            _optional()
            a + b
        }
        t |> success(r.matched, "optional(1) + fixed on 2-stmt block should match")
    }
}

[test(description = "_optional before a fixed statement fails when two statements precede it")]
def test_optional_two_fails(t : T?) {
    ast_gc_guard() {
        // { print(); print(); a + b } — _optional() can't match 2, should fail
        var expr <- make_block_3()
        let r = qmatch_block(expr) $ {
            _optional()
            a + b
        }
        t |> success(!r.matched, "optional + fixed on 3-stmt block should fail (2 before fixed)")
    }
}

[test(description = "a trailing _optional matches zero remaining statements")]
def test_optional_trailing_zero(t : T?) {
    ast_gc_guard() {
        // { a + b } — fixed matches, trailing _optional() matches 0
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            a + b
            _optional()
        }
        t |> success(r.matched, "fixed + trailing optional(0) should match")
    }
}

[test(description = "a trailing _optional matches one remaining statement")]
def test_optional_trailing_one(t : T?) {
    ast_gc_guard() {
        // { a + b; return c; print() } — fixed matches a+b, trailing _optional() matches 1? No, 2 remain
        // Use { print(); a + b } — fixed matches print(), trailing _optional() matches a+b (1)
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            print()
            _optional()
        }
        t |> success(r.matched, "fixed + trailing optional(1) should match")
    }
}

[test(description = "a trailing _optional fails when two statements remain")]
def test_optional_trailing_two_fails(t : T?) {
    ast_gc_guard() {
        // { print(); print(); a + b } — fixed matches print(), trailing _optional() has 2 remaining
        var expr <- make_block_3()
        let r = qmatch_block(expr) $ {
            print()
            _optional()
        }
        t |> success(!r.matched, "fixed + trailing optional with 2 remaining should fail")
    }
}

[test(description = "a lone trailing _optional matches an empty block")]
def test_optional_trailing_only_zero(t : T?) {
    ast_gc_guard() {
        // Empty block — trailing _optional() alone matches 0
        var expr = qmacro($ { })
        let r = qmatch_block(expr) $ {
            _optional()
        }
        t |> success(r.matched, "trailing optional on empty block should match")
    }
}

[test(description = "a lone trailing _optional matches a one-statement block")]
def test_optional_trailing_only_one(t : T?) {
    ast_gc_guard() {
        // { a + b } — _optional() alone matches 1
        var expr <- make_block_1()
        let r = qmatch_block(expr) $ {
            _optional()
        }
        t |> success(r.matched, "trailing optional on 1-stmt block should match")
    }
}

[test(description = "a lone trailing _optional fails on a two-statement block")]
def test_optional_trailing_only_two_fails(t : T?) {
    ast_gc_guard() {
        // { print(); a + b } — _optional() alone can't match 2
        var expr <- make_block_2()
        let r = qmatch_block(expr) $ {
            _optional()
        }
        t |> success(!r.matched, "trailing optional on 2-stmt block should fail")
    }
}

[test(description = "_optional between two fixed statements matches one statement between them")]
def test_optional_sandwich(t : T?) {
    ast_gc_guard() {
        // { a + b; return c; print() } — fixed, _optional(), fixed
        var expr <- make_block_4()
        let r = qmatch_block(expr) $ {
            a + b
            _optional()
            print()
        }
        t |> success(r.matched, "fixed + optional(1) + fixed should match")
    }
}

[test(description = "_optional between two fixed statements matches zero statements between them")]
def test_optional_sandwich_zero(t : T?) {
    ast_gc_guard() {
        // { a + b; print() } — 2 stmts, fixed + _optional(0) + fixed
        var expr = qmacro($ { a + b; print(); })
        let r = qmatch_block(expr) $ {
            a + b
            _optional()
            print()
        }
        t |> success(r.matched, "fixed + optional(0) + fixed should match")
    }
}

[test(description = "_optional between two fixed statements fails when two statements sit between them")]
def test_optional_sandwich_two_fails(t : T?) {
    ast_gc_guard() {
        // { a + b; foo(); bar(); print() } — 4 stmts, 2 between fixed stmts
        var expr = qmacro($ { a + b; foo(); bar(); print(); })
        let r = qmatch_block(expr) $ {
            a + b
            _optional()
            print()
        }
        t |> success(!r.matched, "fixed + optional + fixed with 2 between should fail")
    }
}
