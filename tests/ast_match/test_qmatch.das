options gen2
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public
require daslib/rtti

// ── Constant matching ───────────────────────────────────────────────────

[test(description = "qmatch matches an int constant against the same value and rejects a different one")]
def test_const_int(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        t |> success(qmatch(expr, 42).matched, "42 == 42")
        t |> success(!qmatch(expr, 99).matched, "42 != 99")
    }
}

[test(description = "qmatch matches a float constant against the same value and rejects a different one")]
def test_const_float(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(3.14)
        t |> success(qmatch(expr, 3.14).matched, "3.14 == 3.14")
        t |> success(!qmatch(expr, 2.71).matched, "3.14 != 2.71")
    }
}

[test(description = "qmatch matches a bool constant against the same value and rejects the other")]
def test_const_bool(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(true)
        t |> success(qmatch(expr, true).matched, "true == true")
        var f = qmacro(false)
        t |> success(!qmatch(f, true).matched, "false != true")
    }
}

[test(description = "qmatch matches a string constant against the same text and rejects different text")]
def test_const_string(t : T?) {
    ast_gc_guard() {
        var expr = qmacro("hello")
        t |> success(qmatch(expr, "hello").matched, "hello == hello")
        t |> success(!qmatch(expr, "world").matched, "hello != world")
    }
}

// ── RTTI mismatch ───────────────────────────────────────────────────────

[test(description = "qmatch rejects a pattern whose constant type differs from the expression's")]
def test_rtti_mismatch(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(42)
        let r = qmatch(expr, true)
        t |> success(!r.matched, "int vs bool")
        t |> equal(int(r.error), int(QMatchError.rtti_mismatch))
    }
}

[test(description = "qmatch rejects patterns whose node kind differs: int vs string, bool vs int, operator vs constant")]
def test_cross_type_mismatches(t : T?) {
    ast_gc_guard() {
        var i = qmacro(42)
        t |> success(!qmatch(i, "hello").matched, "int vs string")
        var b = qmacro(true)
        t |> success(!qmatch(b, 42).matched, "bool vs int")
        var op = qmacro(a + b)
        t |> success(!qmatch(op, 42).matched, "op2 vs const")
        t |> success(!qmatch(i, a + b).matched, "const vs op2")
    }
}

// ── Null expression ─────────────────────────────────────────────────────

[test(description = "qmatch fails without crashing on a null expression")]
def test_null(t : T?) {
    ast_gc_guard() {
        var expr : ExpressionPtr
        let r = qmatch(expr, 42)
        t |> success(!r.matched, "null fails")
        t |> equal(int(r.error), int(QMatchError.null_expression))
    }
}

// ── Operator matching ───────────────────────────────────────────────────

[test(description = "qmatch matches a binary operator and rejects a different operator or a different operand")]
def test_op2(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a + b)
        t |> success(qmatch(expr, a + b).matched, "a+b == a+b")
        t |> success(!qmatch(expr, a - b).matched, "a+b != a-b")
        var expr2 = qmacro(x + b)
        t |> success(!qmatch(expr2, a + b).matched, "x+b != a+b")
    }
}

// ── Target functions (compiled with optimizations) ──────────────────────

[export, marker(no_coverage)]
def target_simple(a, b : int) : int => a + b

[export, marker(no_coverage)]
def target_if_return(a, b, c : int) : int {
    if (a < 5) {
        return b
    } else {
        return c
    }
}

// ── Tests ───────────────────────────────────────────────────────────────

[test(description = "qmatch_function matches a simple function body against a pattern with the same parameters")]
def test_function_simple(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_simple)
        t |> success(func != null, "should find target_simple")
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a, b : int) {
            return a + b
        }
        if (!r.matched && r.expr != null) {
            t |> success(false, "error={int(r.error)} rtti={qm_rtti(r.expr)}")
        } elif (!r.matched) {
            t |> success(false, "error={int(r.error)} expr=null")
        } else {
            t |> success(true)
        }
    }
}

[test(description = "qmatch_function matches a function body that returns through an if")]
def test_function_if_return(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_if_return)
        t |> success(func != null, "should find target_if_return")
        if (func == null) {
            return
        }
        // After optimization, if/else return may fold into ternary
        var cond : ExpressionPtr
        let r_ternary = qmatch_function(func) $(a, b, c : int) {
            return $e(cond) ? $e(cond) : $e(cond)
        }
        let r_if = qmatch_function(func) $(a, b, c : int) {
            if (a < 5) {
                return b
            } else {
                return c
            }
        }
        t |> success(r_ternary.matched || r_if.matched, "should match ternary or if-else")
    }
}

[test(description = "qmatch_function rejects a function body that differs from the pattern")]
def test_function_wrong_pattern(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_simple)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a, b : int) {
            return a - b
        }
        t |> success(!r.matched, "wrong pattern should not match")
    }
}

// The qmatch contract: capture variables are written ONLY when the whole match
// succeeds. A failing match leaves every input untouched, no matter where in the
// generated guard ladder the failure lands. Each test below fails AFTER the
// capture site would have run, per capture kind ($e $v $i $t $c $f $b $a).

// ── $e — expression capture ─────────────────────────────────────────────

[test(description = "a failed match on the root operator leaves $e captures untouched")]
def test_e_untouched_on_root_op_fail(t : T?) {
    ast_gc_guard() {
        // both children captured before the op guard compares "*" vs "+"
        var expr = qmacro(x * y)
        var a, b : ExpressionPtr
        let r = qmatch(expr, $e(a) + $e(b))
        t |> success(!r.matched, "op mismatch must fail")
        t |> success(a == null, "$e(a) untouched on failed match")
        t |> success(b == null, "$e(b) untouched on failed match")
    }
}

[test(description = "a failed match on a nested operator leaves $e captures untouched")]
def test_e_untouched_on_nested_fail(t : T?) {
    ast_gc_guard() {
        // right subtree captures b,c then fails its own op guard; a never reached
        var expr = qmacro(x * (y - z))
        var a, b, c : ExpressionPtr
        let r = qmatch(expr, $e(a) * ($e(b) + $e(c)))
        t |> success(!r.matched, "nested op mismatch must fail")
        t |> success(a == null, "$e(a) untouched")
        t |> success(b == null, "$e(b) untouched")
        t |> success(c == null, "$e(c) untouched")
    }
}

[test(description = "a failed match leaves the bindings a previous successful match wrote into the same captures")]
def test_e_ladder_keeps_previous_bindings(t : T?) {
    ast_gc_guard() {
        // the real-world hazard: a failed second qmatch must not clobber
        // bindings a previous successful match established
        var good = qmacro(x + y)
        var bad = qmacro(p * q)
        var a, b : ExpressionPtr
        let r1 = qmatch(good, $e(a) + $e(b))
        t |> success(r1.matched, "first match succeeds")
        let a0 = a
        let b0 = b
        let r2 = qmatch(bad, $e(a) + $e(b))
        t |> success(!r2.matched, "second match fails")
        t |> success(a == a0, "a still holds the first match's binding")
        t |> success(b == b0, "b still holds the first match's binding")
    }
}

// ── $v — constant value capture ─────────────────────────────────────────

[test(description = "a failure after the $v position leaves $v captures untouched")]
def test_v_untouched_on_late_fail(t : T?) {
    ast_gc_guard() {
        // both constants extract fine; the op guard then fails
        var expr = qmacro(5 * 6)
        var k = 999
        var m = 888
        let r = qmatch(expr, $v(k) + $v(m))
        t |> success(!r.matched, "op mismatch must fail")
        t |> equal(999, k, "$v(k) untouched on failed match")
        t |> equal(888, m, "$v(m) untouched on failed match")
    }
}

// ── $i — name capture ───────────────────────────────────────────────────

[test(description = "a failure after the $i position leaves $i captures untouched")]
def test_i_untouched_on_late_fail(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(x * y)
        var nm = "keep1"
        var nm2 = "keep2"
        let r = qmatch(expr, $i(nm) + $i(nm2))
        t |> success(!r.matched, "op mismatch must fail")
        t |> equal("keep1", nm, "$i(nm) untouched on failed match")
        t |> equal("keep2", nm2, "$i(nm2) untouched on failed match")
    }
}

// ── $t — type capture ───────────────────────────────────────────────────

[test(description = "a failure after the $t position leaves the $t capture untouched")]
def test_t_untouched_on_late_fail(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(type<int>) - x)
        var td : TypeDeclPtr
        var z : ExpressionPtr
        let r = qmatch(expr, foo(type<$t(td)>) + $e(z))
        t |> success(!r.matched, "op mismatch must fail")
        t |> success(td == null, "$t(td) untouched on failed match")
        t |> success(z == null, "$e(z) untouched on failed match")
    }
}

// ── $c — call name capture ──────────────────────────────────────────────

[test(description = "a failure after the $c position leaves the $c capture untouched")]
def test_c_untouched_on_late_fail(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(foo(1) - x)
        var cn = "keep"
        var z : ExpressionPtr
        let r = qmatch(expr, $c(cn)(1) + $e(z))
        t |> success(!r.matched, "op mismatch must fail")
        t |> equal("keep", cn, "$c(cn) untouched on failed match")
        t |> success(z == null, "$e(z) untouched on failed match")
    }
}

// ── $f — field name capture ─────────────────────────────────────────────

[test(description = "a failure after the $f position leaves the $f capture untouched")]
def test_f_untouched_on_late_fail(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(a.foo - x)
        var fn = "keep"
        var z : ExpressionPtr
        let r = qmatch(expr, a.$f(fn) + $e(z))
        t |> success(!r.matched, "op mismatch must fail")
        t |> equal("keep", fn, "$f(fn) untouched on failed match")
        t |> success(z == null, "$e(z) untouched on failed match")
    }
}

// ── $b — statement range capture ────────────────────────────────────────

[test(description = "a failed statement match leaves the $b capture untouched")]
def test_b_untouched_on_late_fail(t : T?) {
    ast_gc_guard() {
        // _any($b) extracts its statement, then the fixed baz() fails on bar()
        var expr = qmacro($ { foo(); bar(); })
        var stmts : array<ExpressionPtr>
        let r = qmatch_block(expr) $ {
            _any($b(stmts))
            baz()
        }
        t |> success(!r.matched, "fixed statement mismatch must fail")
        t |> equal(0, length(stmts), "$b(stmts) untouched on failed match")
    }
}

// ── $a — remaining-args capture ─────────────────────────────────────────

[export, marker(no_coverage)]
def nbof_target(a, b, c : int) : int => a + b + c

[test(description = "a failed function-body match leaves the $a argument-rest capture untouched")]
def test_a_untouched_on_body_fail(t : T?) {
    ast_gc_guard() {
        // args phase matches and captures rest; the body pattern then fails
        var func = find_module_function_via_rtti(compiling_module(), @@nbof_target)
        if (func == null) {
            return
        }
        var rest : array<VariablePtr>
        let r = qmatch_function(func) $(a : int; $a(rest)) {
            return a * 111
        }
        t |> success(!r.matched, "body mismatch must fail")
        t |> equal(0, length(rest), "$a(rest) untouched on failed match")
    }
}

// ── success still binds, including through the scan and direct paths ────

[test(description = "a successful scan match still binds captures after deferred binding")]
def test_scan_path_still_binds_on_success(t : T?) {
    ast_gc_guard() {
        // the wildcard scan runs the element matcher in an inner closure -
        // captures must still commit when the overall match succeeds
        var expr = qmacro($ { foo(); q + 1; })
        var cap : ExpressionPtr
        let r = qmatch_block(expr) $ {
            _wildcard()
            $e(cap) + 1
        }
        t |> success(r.matched, "scan should find q + 1")
        t |> success(cap != null, "$e(cap) bound through the scan path")
        t |> success(cap != null && cap is ExprVar, "cap is the scanned q")
    }
}

[test(description = "a successful direct match still binds captures after deferred binding")]
def test_direct_path_still_binds_on_success(t : T?) {
    ast_gc_guard() {
        // single-statement block pattern against a non-block source takes the
        // direct-match early-success path - commits must ride it too
        var expr = qmacro(x + 1)
        var cap : ExpressionPtr
        let r = qmatch_block(expr) $ {
            $e(cap) + 1
        }
        t |> success(r.matched, "direct path should match")
        t |> success(cap != null, "$e(cap) bound through the direct path")
    }
}
