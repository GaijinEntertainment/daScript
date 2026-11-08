options gen2
options rtti

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── $c(var) — capture call name ─────────────────────────────────────────

[test(description = "a $c capture matches a call of any name and captures that name")]
def test_c_call_name(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(1, 2))
        var cname = ""
        let r = qmatch(expr, $c(cname)(1, 2))
        t |> success(r.matched, "should match call with any name")
        t |> equal(cname, "foo", "should capture call name")
    }
}

[test(description = "a $c capture takes the callee name of whichever call it meets")]
def test_c_call_name_different(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(bar(a, b))
        var cname = ""
        let r = qmatch(expr, $c(cname)(a, b))
        t |> success(r.matched, "should match bar call")
        t |> equal(cname, "bar", "should capture bar")
    }
}

[test(description = "a $c capture does not rescue a call whose arguments differ from the pattern")]
def test_c_args_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(1, 2))
        var cname = ""
        let r = qmatch(expr, $c(cname)(1, 3))
        t |> success(!r.matched, "wrong args should fail even with $c")
    }
}

[test(description = "a $c capture inside qmatch_block captures the name of a call statement")]
def test_c_in_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { foo(a); bar(b); })
        var cname = ""
        let r = qmatch_block(expr) $ {
            _wildcard()
            $c(cname)(b)
        }
        t |> success(r.matched, "should match bar in block")
        t |> equal(cname, "bar", "should capture bar from block")
    }
}

// ── $f(var) — capture field name ────────────────────────────────────────

[test(description = "an $f capture matches a field access of any name and captures the field name")]
def test_f_field_name(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a.foo)
        var fname = ""
        let r = qmatch(expr, a.$f(fname))
        t |> success(r.matched, "should match field access with any name")
        t |> equal(fname, "foo", "should capture field name")
    }
}

[test(description = "an $f capture takes the field name of whichever access it meets")]
def test_f_field_different(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(obj.bar)
        var fname = ""
        let r = qmatch(expr, obj.$f(fname))
        t |> success(r.matched, "should match obj.bar")
        t |> equal(fname, "bar", "should capture bar field name")
    }
}

[test(description = "an $f capture does not rescue a field access whose base value differs")]
def test_f_field_value_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a.foo)
        var fname = ""
        let r = qmatch(expr, b.$f(fname))
        t |> success(!r.matched, "wrong value should fail even with $f")
    }
}

[test(description = "an $f capture inside qmatch_block captures the field name of a statement")]
def test_f_in_block(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { a.x; b.y; })
        var fname = ""
        let r = qmatch_block(expr) $ {
            _wildcard()
            b.$f(fname)
        }
        t |> success(r.matched, "should match b.y in block")
        t |> equal(fname, "y", "should capture y from block")
    }
}

// ── $b(var) — capture statement range ───────────────────────────────────

[test(description = "a trailing $b wildcard captures the remaining statements of a block")]
def test_b_wildcard_trailing(t : T?) {
    ast_gc_guard() {
        // Capture all trailing statements
        var expr = qmacro($ { foo(); bar(); baz(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            foo()
            _wildcard($b(stmts))
        }
        t |> success(r.matched, "should match with trailing $b capture")
        t |> equal(length(stmts), 2, "should capture 2 trailing stmts")
    }
}

[test(description = "a leading $b wildcard captures the statements before the fixed tail")]
def test_b_wildcard_leading(t : T?) {
    ast_gc_guard() {
        // Capture all leading statements before a fixed match
        var expr = qmacro($ { foo(); bar(); baz(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            _wildcard($b(stmts))
            baz()
        }
        t |> success(r.matched, "should match with leading $b capture")
        t |> equal(length(stmts), 2, "should capture 2 leading stmts")
    }
}

[test(description = "a $b wildcard matches and captures nothing when no statements remain")]
def test_b_wildcard_empty(t : T?) {
    ast_gc_guard() {
        // Wildcard captures zero statements
        var expr = qmacro($ { foo(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            _wildcard($b(stmts))
            foo()
        }
        t |> success(r.matched, "should match with empty $b")
        t |> equal(length(stmts), 0, "should capture 0 stmts")
    }
}

[test(description = "a $b wildcard between two fixed statements captures exactly the middle statements")]
def test_b_wildcard_sandwich(t : T?) {
    ast_gc_guard() {
        // Capture middle statements between two fixed
        var expr = qmacro($ { foo(); a(); b(); bar(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            foo()
            _wildcard($b(stmts))
            bar()
        }
        t |> success(r.matched, "should match sandwich")
        t |> equal(length(stmts), 2, "should capture 2 middle stmts")
    }
}

[test(description = "a trailing $b _wildcard1 captures the remaining statements")]
def test_b_wildcard1_trailing(t : T?) {
    ast_gc_guard() {
        // _wildcard1 with $b — must have at least 1
        var expr = qmacro($ { foo(); bar(); baz(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            foo()
            _wildcard1($b(stmts))
        }
        t |> success(r.matched, "should match with _wildcard1 trailing $b")
        t |> equal(length(stmts), 2, "should capture 2 trailing stmts")
    }
}

[test(description = "a $b _any pattern captures exactly one statement")]
def test_b_any_capture(t : T?) {
    ast_gc_guard() {
        // _any with $b — captures exactly one statement
        var expr = qmacro($ { foo(); bar(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            _any($b(stmts))
            bar()
        }
        t |> success(r.matched, "should match with _any $b")
        t |> equal(length(stmts), 1, "should capture exactly 1 stmt")
    }
}

[test(description = "a wildcard without a $b tag still matches and captures nothing")]
def test_b_no_tag_still_works(t : T?) {
    ast_gc_guard() {
        // Wildcards without $b still work as before
        var expr = qmacro($ { foo(); bar(); baz(); })
        let r = qmatch_block(expr) $ {
            _wildcard()
            baz()
        }
        t |> success(r.matched, "wildcard without $b should still work")
    }
}

[test(description = "$e captures bind the left and right operands of a binary operator")]
def test_e_op2_children(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a + b)
        var left, right : ExpressionPtr
        let r = qmatch(expr, $e(left) + $e(right))
        t |> success(r.matched, "should capture left and right")
        if (r.matched) {
            t |> equal(left.__rtti, "ExprVar")
            t |> equal(right.__rtti, "ExprVar")
        }
    }
}

[test(description = "a $e capture binds an arbitrary whole expression")]
def test_e_whole_expression(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a + b)
        var captured : ExpressionPtr
        let r = qmatch(expr, $e(captured))
        t |> success(r.matched, "$e on any expression should match")
        if (r.matched) {
            t |> equal(captured.__rtti, "ExprOp2")
        }
    }
}

[test(description = "an op2 pattern with $e operands rejects a non-operator expression")]
def test_e_rtti_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        var captured : ExpressionPtr
        let r = qmatch(expr, $e(captured) + $e(captured))
        t |> success(!r.matched, "int should not match op2 with $e")
        t |> equal(int(r.error), int(QMatchError.rtti_mismatch))
    }
}

[test(description = "a $e capture is a clone, not the pointer of the matched expression")]
def test_e_clone_independence(t : T?) {
    ast_gc_guard() {
        // Captured clone should be independent from original
        var expr = qmacro(42)
        var captured : ExpressionPtr
        let r = qmatch(expr, $e(captured))
        t |> success(r.matched, "capture should succeed")
        if (r.matched) {
            // Both should be ExprConstInt with value 42
            t |> equal(captured.__rtti, "ExprConstInt")
            // Pointers should differ (it's a clone)
            t |> success(captured != expr, "clone should be different pointer")
        }
    }
}

[test(description = "$e captures work next to a fixed operand in one pattern")]
def test_e_mixed_with_fixed(t : T?) {
    ast_gc_guard() {
        // Pattern: $e(x) + 5  — capture left, fixed right
        var expr = qmacro(a + 5)
        var left : ExpressionPtr
        let r = qmatch(expr, $e(left) + 5)
        t |> success(r.matched, "should match $e + fixed")
        if (r.matched) {
            t |> equal(left.__rtti, "ExprVar")
        }
    }
}

[test(description = "a $e capture inside qmatch_block captures a return value expression")]
def test_e_in_block(t : T?) {
    ast_gc_guard() {
        // $e capture inside qmatch_block
        var expr = qmacro($ { return a + b; })
        var captured : ExpressionPtr
        let r = qmatch_block(expr) $ {
            return $e(captured)
        }
        t |> success(r.matched, "should capture return value in block")
        if (r.matched) {
            t |> equal(captured.__rtti, "ExprOp2")
        }
    }
}

[test(description = "an $i capture extracts the name of a variable expression")]
def test_i_var(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(my_variable)
        var name : string
        let r = qmatch(expr, $i(name))
        t |> success(r.matched, "should extract var name")
        t |> equal(name, "my_variable")
    }
}

[test(description = "$i captures extract both variable names of a binary operator")]
def test_i_from_op2(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a + b)
        var left_name, right_name : string
        let r = qmatch(expr, $i(left_name) + $i(right_name))
        t |> success(r.matched, "should extract both var names from op2")
        t |> equal(left_name, "a")
        t |> equal(right_name, "b")
    }
}

[test(description = "an $i capture matches any expression, a constant included")]
def test_i_on_const(t : T?) {
    ast_gc_guard() {
        // $i on a constant expression — should still match (returns "")
        var expr = qmacro(42)
        var name : string
        let r = qmatch(expr, $i(name))
        t |> success(r.matched, "$i should match any expression")
        t |> equal(name, "")
    }
}

[test(description = "an $i capture works next to a fixed variable in one pattern")]
def test_i_mixed_with_fixed(t : T?) {
    ast_gc_guard() {
        // Pattern: $i(name) + b  — capture left name, fixed right
        var expr = qmacro(foo + b)
        var name : string
        let r = qmatch(expr, $i(name) + b)
        t |> success(r.matched, "should match $i + fixed var")
        t |> equal(name, "foo")
    }
}

// ── $i in let declarations ────────────────────────────────────────────────

[test(description = "an $i capture extracts the name declared by a let statement")]
def test_i_let_name(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { var x = 5; })
        var name : string
        let r = qmatch_block(expr) $ {
            var $i(name) = 5
        }
        t |> success(r.matched, "should capture let var name")
        t |> equal(name, "x")
    }
}

[test(description = "a fixed let name in the pattern matches the same declared name")]
def test_i_let_name_fixed(t : T?) {
    ast_gc_guard() {
        // Fixed name match — var x should match var x
        var expr = qmacro($ { var x = 5; })
        let r = qmatch_block(expr) $ {
            var x = 5
        }
        t |> success(r.matched, "fixed let name should match")
    }
}

[test(description = "a fixed let name in the pattern rejects a different declared name")]
def test_i_let_name_wrong(t : T?) {
    ast_gc_guard() {
        // Fixed name mismatch — var x should not match var y
        var expr = qmacro($ { var x = 5; })
        let r = qmatch_block(expr) $ {
            var y = 5
        }
        t |> success(!r.matched, "wrong let name should fail")
    }
}

[test(description = "an $i capture extracts the let name when the declaration has an explicit type")]
def test_i_let_with_type(t : T?) {
    ast_gc_guard() {
        // Match let with type annotation
        var expr = qmacro($ { var x : int = 5; })
        var name : string
        let r = qmatch_block(expr) $ {
            var $i(name) : int = 5
        }
        t |> success(r.matched, "should capture let name with type")
        t |> equal(name, "x")
    }
}

// ── $i in for loops ──────────────────────────────────────────────────────

[test(description = "a fixed for-loop iterator name matches the same iterator")]
def test_i_for_iterator(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (x in arr) {
                print()
            }
        }
        t |> success(r.matched, "for iterator name should match")
    }
}

[test(description = "a fixed for-loop iterator name rejects a different iterator")]
def test_i_for_wrong_iterator(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { print(); }; })
        let r = qmatch_block(expr) $ {
            for (y in arr) {
                print()
            }
        }
        t |> success(!r.matched, "wrong for iterator name should fail")
    }
}

// ── $i capture in for loops ───────────────────────────────────────────────

[test(description = "an $i capture extracts the for-loop iterator name")]
def test_i_for_capture(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { for (x in arr) { print(); }; })
        var iter_name : string
        let r = qmatch_block(expr) $ {
            for ($i(iter_name) in arr) {
                print()
            }
        }
        t |> success(r.matched, "should capture for iterator name")
        t |> equal(iter_name, "x")
    }
}

// ── $v int ──────────────────────────────────────────────────────────────

[test(description = "a $v int capture extracts an int constant")]
def test_v_int(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        var val : int
        let r = qmatch(expr, $v(val))
        t |> success(r.matched, "should extract int")
        t |> equal(val, 42)
    }
}

[test(description = "a $v int capture rejects a float constant")]
def test_v_int_wrong_type(t : T?) {
    ast_gc_guard() {
        // $v(int) on a float expression should fail
        var expr = qmacro(3.14)
        var val : int
        let r = qmatch(expr, $v(val))
        t |> success(!r.matched, "float expr should not match int $v")
        t |> equal(int(r.error), int(QMatchError.const_type_mismatch))
    }
}

[test(description = "a $v int capture rejects a non-constant expression")]
def test_v_int_on_non_const(t : T?) {
    ast_gc_guard() {
        // $v(int) on a non-constant expression should fail
        var expr = qmacro(a + b)
        var val : int
        let r = qmatch(expr, $v(val))
        t |> success(!r.matched, "non-const expr should not match $v")
    }
}

// ── $v float ────────────────────────────────────────────────────────────

[test(description = "a $v float capture extracts a float constant")]
def test_v_float(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(3.14)
        var val : float
        let r = qmatch(expr, $v(val))
        t |> success(r.matched, "should extract float")
        t |> success(val > 3.13 && val < 3.15, "val should be ~3.14")
    }
}

[test(description = "a $v float capture rejects an int constant")]
def test_v_float_wrong_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        var val : float
        let r = qmatch(expr, $v(val))
        t |> success(!r.matched, "int expr should not match float $v")
        t |> equal(int(r.error), int(QMatchError.const_type_mismatch))
    }
}

// ── $v bool ─────────────────────────────────────────────────────────────

[test(description = "a $v bool capture extracts true")]
def test_v_bool(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(true)
        var val : bool
        let r = qmatch(expr, $v(val))
        t |> success(r.matched, "should extract bool")
        t |> equal(val, true)
    }
}

[test(description = "a $v bool capture extracts false")]
def test_v_bool_false(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(false)
        var val : bool
        let r = qmatch(expr, $v(val))
        t |> success(r.matched, "should extract false")
        t |> equal(val, false)
    }
}

[test(description = "a $v bool capture rejects an int constant")]
def test_v_bool_wrong_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        var val : bool
        let r = qmatch(expr, $v(val))
        t |> success(!r.matched, "int expr should not match bool $v")
        t |> equal(int(r.error), int(QMatchError.const_type_mismatch))
    }
}

// ── $v string ───────────────────────────────────────────────────────────

[test(description = "a $v string capture extracts a string constant")]
def test_v_string(t : T?) {
    ast_gc_guard() {
        var expr = qmacro("hello world")
        var val : string
        let r = qmatch(expr, $v(val))
        t |> success(r.matched, "should extract string")
        t |> equal(val, "hello world")
    }
}

[test(description = "a $v string capture rejects an int constant")]
def test_v_string_wrong_type(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        var val : string
        let r = qmatch(expr, $v(val))
        t |> success(!r.matched, "int expr should not match string $v")
        t |> equal(int(r.error), int(QMatchError.const_type_mismatch))
    }
}

// ── $v in compound patterns ─────────────────────────────────────────────

[test(description = "a $v capture works as one operand of a binary operator next to a fixed operand")]
def test_v_in_op2(t : T?) {
    ast_gc_guard() {
        // Pattern: a + $v(val)  — extract the constant from the right side
        var expr = qmacro(a + 5)
        var val : int
        let r = qmatch(expr, a + $v(val))
        t |> success(r.matched, "should match a + $v")
        t |> equal(val, 5)
    }
}

[test(description = "a $v capture inside qmatch_block extracts a return constant")]
def test_v_in_block(t : T?) {
    ast_gc_guard() {
        // $v inside qmatch_block
        var expr = qmacro($ { return 42; })
        var val : int
        let r = qmatch_block(expr) $ {
            return $v(val)
        }
        t |> success(r.matched, "should extract return value in block")
        t |> equal(val, 42)
    }
}
