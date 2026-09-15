options gen2
options indenting = 4
options rtti
options no_coverage // shape pins: injected coverage calls sit between the fold and the probe
options auto_inline_functions = false // shape pins: splices rewrite the shapes under probe
options disable_algebraic_folding = true // the declarative rules carry the identities here

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require strings
require daslib/strings_boost
require math
require daslib/fold_rules_std
require dastest/testing_boost public

// The identity catalog, proven with the built-in algebraic arms stood down: every rewrite below is
// the daslib rule firing, not the compiler's own fold.

var g_i = 3
var g_f = 3.0
var g_b = true

[export, never_inline] def t_mul_one(x : int) : int { return x * 1 }
[export, never_inline] def t_mul_zero(x : int) : int { return x * 0 }
[export, never_inline] def t_mul_neg_one(x : int) : int { return x * -1 }
[export, never_inline] def t_add_zero(x : float) : float { return x + 0.0 }
[export, never_inline] def t_sub_self(x : int) : int { return x - x } // nolint:LINT007 - the self-operand shape is the fixture
[export, never_inline] def t_zero_sub(x : int) : int { return 0 - x }
[export, never_inline] def t_canonical(x : int) : int { return 2 * x }
[export, never_inline] def t_xor_self(x : int) : int { return x ^ x } // nolint:LINT007 - the self-operand shape is the fixture
[export, never_inline] def t_or_zero(x : int) : int { return x | 0 }
[export, never_inline] def t_and_self(x : int) : int { return x & x } // nolint:LINT007 - the self-operand shape is the fixture
[export, never_inline] def t_shift_zero(x : int) : int { return x << 0 }
[export, never_inline] def t_not_not(b : bool) : bool { return !!b }
[export, never_inline] def t_neg_neg(x : int) : int { return -(-x) }
[export, never_inline] def t_ternary_id(c : bool) : bool { return c ? true : false }
[export, never_inline] def t_ternary_not(c : bool) : bool { return c ? false : true }
[export, never_inline] def t_ternary_same(c : bool; x : int) : int { return c ? x : x } // nolint:LINT008 - the self-operand shape is the fixture
[export, never_inline] def t_and_true(b : bool) : bool { return b && true }
[export, never_inline] def t_eq_self(x : int) : bool { return x == x } // nolint:LINT007 - the self-operand shape is the fixture
[export, never_inline] def t_lt_self(x : int) : bool { return x < x } // nolint:LINT007 - the self-operand shape is the fixture
[export, never_inline] def t_pow_two(x : float) : float { return pow(x, 2.0) }
[export, never_inline] def t_abs_abs(x : float) : float { return abs(abs(x)) }
[export, never_inline] def t_abs_neg(x : float) : float { return abs(-x) }
[export, never_inline] def t_min_self(x : float) : float { return min(x, x) }

// negative controls: the gate refuses these
[export, never_inline] def t_float_eq_self(x : float) : bool { return x == x } // nolint:LINT007 - the self-operand shape is the fixture
[export, never_inline] def t_double_add_zero(x : double) : double { return x + 0.0lf }

def body_of(t : T?; nm : string) : string {
    var body = ""
    compiling_module() |> for_each_function(nm) $(func) {
        if (empty(body)) {
            body = "{describe(func.body)}"
        }
    }
    t |> success(!empty(body), "target function {nm} found")
    let clean <- [for (ln in split(body, "\n")); ln; where find(ln, "coverage(", 0) < 0]
    return join(clean, " ")
}

def has(s, sub : string) : bool {
    return find(s, sub, 0) >= 0
}

def folds_to(t : T?; nm, want : string) {
    let b = body_of(t, nm)
    t |> success(has(b, want), "{nm} expected `{want}` in: {b}")
}

[test]
def test_arithmetic_identities(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_mul_one", "return x")
        folds_to(t, "t_mul_zero", "return 0")
        folds_to(t, "t_mul_neg_one", "return -x")
        folds_to(t, "t_add_zero", "return x")
        folds_to(t, "t_sub_self", "return 0")
        folds_to(t, "t_zero_sub", "return -x")
        t |> success(!has(body_of(t, "t_mul_one"), "*"), "no multiply survives x*1")
    }
}

[test]
def test_constants_move_right(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_canonical", "x * 2")
    }
}

[test]
def test_bitwise_identities(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_xor_self", "return 0")
        folds_to(t, "t_or_zero", "return x")
        folds_to(t, "t_and_self", "return x")
        folds_to(t, "t_shift_zero", "return x")
    }
}

[test]
def test_unary_and_ternary_identities(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_not_not", "return b")
        folds_to(t, "t_neg_neg", "return x")
        folds_to(t, "t_ternary_id", "return c")
        folds_to(t, "t_ternary_not", "return !c")
        folds_to(t, "t_ternary_same", "return x")
        folds_to(t, "t_and_true", "return b")
    }
}

[test]
def test_comparison_identities(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_eq_self", "return true")
        folds_to(t, "t_lt_self", "return false")
    }
}

[test]
def test_idiom_rewrites(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_pow_two", "x * x")
        folds_to(t, "t_abs_abs", "abs(x)")
        t |> success(!has(body_of(t, "t_abs_abs"), "abs(abs"), "the inner abs is gone")
        folds_to(t, "t_abs_neg", "abs(x)")
        t |> success(!has(body_of(t, "t_abs_neg"), "-"), "the negate is gone")
        folds_to(t, "t_min_self", "return x")
    }
}

[test]
def test_gates_refuse_unsafe_rewrites(t : T?) {
    ast_gc_guard() {
        t |> success(has(body_of(t, "t_float_eq_self"), "=="),
            "float x == x must survive: NaN is not equal to itself")
        t |> success(has(body_of(t, "t_double_add_zero"), "+"),
            "double x + 0 must survive without fast_math")
    }
}

[test]
def test_folded_functions_still_compute(t : T?) {
    t |> equal(t_mul_one(g_i), 3)
    t |> equal(t_mul_zero(g_i), 0)
    t |> equal(t_mul_neg_one(g_i), -3)
    t |> equal(t_add_zero(g_f), 3.0)
    t |> equal(t_sub_self(g_i), 0)
    t |> equal(t_not_not(g_b), true)
    t |> equal(t_pow_two(g_f), 9.0)
    t |> equal(t_abs_neg(g_f), 3.0)
    t |> equal(t_ternary_not(g_b), false)
}

// ── the two halves of purity ─────────────────────────────────────────────

var g_calls = 0

def side_effecting : int {
    g_calls ++
    return 7
}

def side_effect_free(a : int) : int {
    return a + 1
}

[export, never_inline] def t_drop_pure_call(x : int) : int { return side_effect_free(x) * 0 }
[export, never_inline] def t_keep_effecting : int { return side_effecting() * 0 }
[export, never_inline] def t_repeat_small(x : float) : float { return pow(x, 2.0) }
[export, never_inline] def t_refuse_large(a : array<float>) : float { return pow(a[0] * a[1] + a[2] * a[3], 2.0) }

[test]
def test_droppable_covers_a_pure_call(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_drop_pure_call", "return 0")
        t |> success(has(body_of(t, "t_keep_effecting"), "side_effecting"),
            "a call with side effects is never dropped")
    }
}

[test]
def test_repeatable_refuses_to_duplicate_a_large_operand(t : T?) {
    ast_gc_guard() {
        folds_to(t, "t_repeat_small", "x * x")
        t |> success(has(body_of(t, "t_refuse_large"), "pow"),
            "a pure but large operand is not worth duplicating")
    }
}
