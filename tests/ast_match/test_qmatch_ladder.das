options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── the ladder form: arms are tried in order and fall through ───────────

def classify(var e : ExpressionPtr) : string {
    var k : float
    qmatch(e) {
        if (pow($e(x), 2.0)) {
            return "square({describe(x)})"
        }
        if (pow($e(x), $v(k))) {
            return "pow({describe(x)},{k})"
        }
        if ($e(a) + $e(b)) {
            return "add({describe(a)},{describe(b)})"
        }
    }
    return "none"
}

[test]
def test_arms_match_in_order(t : T?) {
    ast_gc_guard() {
        t |> equal(classify(qmacro(pow(q, 2.0))), "square(q)")
        t |> equal(classify(qmacro(pow(q, 3.0))), "pow(q,3)")
        t |> equal(classify(qmacro(q + w)), "add(q,w)")
        t |> equal(classify(qmacro(q - w)), "none")
    }
}

// an arm body that does not leave the function lets the next arm try

def fallthrough_probe(var e : ExpressionPtr) : string {
    qmatch(e) {
        if ($e(a) * $e(b)) {
            if (describe(a) != "q") {
                return "mul-other"
            }
        }
        if ($e(a) * $e(b)) {
            return "mul-q({describe(b)})"
        }
    }
    return "none"
}

[test]
def test_arms_fall_through(t : T?) {
    ast_gc_guard() {
        t |> equal(fallthrough_probe(qmacro(q * w)), "mul-q(w)")
        t |> equal(fallthrough_probe(qmacro(z * w)), "mul-other")
    }
}

// ── captures are declared per arm, one kind per tag ─────────────────────

def capture_kinds(var e : ExpressionPtr) : string {
    qmatch(e) {
        if ($c(fn)($e(arg))) {
            return "call {fn} of {describe(arg)}"
        }
        if ($e(obj).$f(fld)) {
            return "field {fld} of {describe(obj)}"
        }
        if ($i(nm) + 1) {
            return "var {nm}"
        }
    }
    return "none"
}

[test]
def test_capture_tags_are_declared(t : T?) {
    ast_gc_guard() {
        t |> equal(capture_kinds(qmacro(sqrt(q))), "call sqrt of q")
        t |> equal(capture_kinds(qmacro(s.member)), "field member of s")
        t |> equal(capture_kinds(qmacro(q + 1)), "var q")
        t |> equal(capture_kinds(qmacro(q - 1)), "none")
    }
}

// the same capture name is independent per arm

def per_arm_capture(var e : ExpressionPtr) : string {
    qmatch(e) {
        if (sqrt($e(x))) {
            return "sqrt:{describe(x)}"
        }
        if (abs($e(x))) {
            return "abs:{describe(x)}"
        }
    }
    return "none"
}

[test]
def test_capture_names_are_per_arm(t : T?) {
    ast_gc_guard() {
        t |> equal(per_arm_capture(qmacro(sqrt(a))), "sqrt:a")
        t |> equal(per_arm_capture(qmacro(abs(b))), "abs:b")
    }
}

// ── the two-argument form still routes to pattern matching ─────────────

[test]
def test_block_pattern_still_matches_as_a_pattern(t : T?) {
    ast_gc_guard() {
        var expr = qmacro($ { return 1; })
        t |> success(qmatch(expr, $ { return 1; }).matched, "block pattern still matches")
        var one_arm = qmacro($ { if (a) { return 1; } })
        t |> success(qmatch(one_arm, $ { if (a) { return 1; } }).matched, "if-only block pattern")
    }
}

// ── predicate patterns: a guard that fails means the arm does not match ──

def _small(e : ExpressionPtr) : bool {
    return e is ExprConstInt && (e as ExprConstInt).value < 10
}

def _named(e : ExpressionPtr; want : string) : bool {
    return e is ExprVar && "{(e as ExprVar).name}" == want
}

def _twin(e, other : ExpressionPtr) : bool {
    return e != null && other != null && describe(e) == describe(other)
}

def predicated(var e : ExpressionPtr) : string {
    qmatch(e) {
        if ($e(a) + _small($e(k))) {
            return "small({describe(k)})"
        }
        if (_named($e(a), "q") + $e(b)) {
            return "q-plus({describe(b)})"
        }
        if ($e(a) - _twin(a)) {
            return "self-sub({describe(a)})"
        }
        if (_ * $e(b)) {
            return "mul-any({describe(b)})"
        }
    }
    return "none"
}

[test]
def test_predicate_patterns_gate_the_arm(t : T?) {
    ast_gc_guard() {
        t |> equal(predicated(qmacro(w + 3)), "small(3)")
        t |> equal(predicated(qmacro(q + 100)), "q-plus(100)")
        t |> equal(predicated(qmacro(z + 100)), "none")
        t |> equal(predicated(qmacro(w - w)), "self-sub(w)")
        t |> equal(predicated(qmacro(w - z)), "none")
        t |> equal(predicated(qmacro(z * 7)), "mul-any(7)")
    }
}

[test]
def test_anonymous_wildcard_matches_any_operand(t : T?) {
    ast_gc_guard() {
        var rhs : ExpressionPtr
        t |> success(qmatch(qmacro(a + b), _ + $e(rhs)).matched, "underscore matched the left operand")
        t |> equal(describe(rhs), "b")
    }
}

// ── a bare numeric literal matches that value in any numeric type ───────

def numeric_kind(var e : ExpressionPtr) : string {
    qmatch(e) {
        if ($e(a) * 1) {
            return "times-one"
        }
        if ($e(a) * -1) {
            return "times-minus-one"
        }
        if ($e(a) * 2) {
            return "times-two"
        }
    }
    return "none"
}

[test]
def test_bare_literals_match_across_numeric_types(t : T?) {
    ast_gc_guard() {
        t |> equal(numeric_kind(qmacro(q * 1)), "times-one")
        t |> equal(numeric_kind(qmacro(q * 1.0)), "times-one")
        t |> equal(numeric_kind(qmacro(q * 1.0lf)), "times-one")
        t |> equal(numeric_kind(qmacro(q * -1)), "times-minus-one")
        t |> equal(numeric_kind(qmacro(q * -1.0)), "times-minus-one")
        t |> equal(numeric_kind(qmacro(q * 2)), "times-two")
        t |> equal(numeric_kind(qmacro(q * 3)), "none")
    }
}

// a splat constant only exists once the optimizer folded the constructor, so the vector arm is
// checked on the comparison itself rather than through a quoted constructor call
[test]
def test_a_splat_constant_matches_the_same_scalar(t : T?) {
    ast_gc_guard() {
        var three = new ExprConstFloat3(at = LineInfo(), value = float3(1.0))
        t |> success(qm_num_equals(three, 1.0lf), "every lane is one")
        t |> success(!qm_num_equals(three, 2.0lf), "not two")
        var mixed = new ExprConstFloat3(at = LineInfo(), value = float3(1.0, 1.0, 2.0))
        t |> success(!qm_num_equals(mixed, 1.0lf), "a lane differs, so the splat does not match")
    }
}

[test]
def test_a_literal_still_refuses_another_kind(t : T?) {
    ast_gc_guard() {
        t |> success(!qmatch(qmacro(true), 1).matched, "a bool is not the number one")
        t |> success(!qmatch(qmacro(42), true).matched, "a number is not a bool")
        t |> success(!qmatch(qmacro("2"), 2).matched, "a string is not a number")
        var v : int
        t |> success(!qmatch(qmacro(3.14), $v(v)).matched, "$v stays exact about the constant class")
    }
}
