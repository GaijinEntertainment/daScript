options gen2
options indenting = 4
options rtti
options no_coverage // shape pins: injected coverage calls sit between the fold and the probe
options auto_inline_functions = false // shape pins: splices rewrite the call shapes under probe

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require strings
require daslib/strings_boost
require math
require _rules
require dastest/testing_boost public

// structural FIRED proof for the engine itself: every target below is rewritten by a fixture rule
// in _rules.das, and the assertions read the post-optimize body back.

var g_seed = 3.0

[export, never_inline]
def target_square(x : float) : float {
    return pow(x, 2.0)
}

[export, never_inline]
def target_square_double(x : double) : double {
    return pow(x, 2.0lf)
}

[export, never_inline]
def target_square_vector(x : float3) : float3 {
    return pow(x, float3(2.0))
}

[export, never_inline]
def target_operand_returned(x : float) : float {
    return pow(x, 1.0)
}

[export, never_inline]
def target_constant_result(x : float) : float {
    return pow(x, 0.0)
}

[export, never_inline]
def target_retry_chain(x : float) : float {
    return sqrt(abs(x))
}

[export, never_inline]
def target_handoff(x : float) : float {
    return cbrt(x)
}

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

[test]
def test_operator_resolution_per_type_family(t : T?) {
    ast_gc_guard() {
        t |> run("float square") @(t : T?) {
            let b = body_of(t, "target_square")
            t |> success(!has(b, "pow"), "no pow survives: {b}")
            t |> success(has(b, "x * x"), "multiply emitted: {b}")
        }
        t |> run("double square") @(t : T?) {
            let b = body_of(t, "target_square_double")
            t |> success(has(b, "x * x"), "double multiply emitted: {b}")
        }
        t |> run("float3 square") @(t : T?) {
            let b = body_of(t, "target_square_vector")
            t |> success(has(b, "x * x"), "vector multiply emitted: {b}")
        }
    }
}

[test]
def test_rule_may_return_an_existing_subtree(t : T?) {
    ast_gc_guard() {
        let b = body_of(t, "target_operand_returned")
        t |> success(!has(b, "pow"), "no pow survives: {b}")
        t |> success(has(b, "return x"), "operand returned directly: {b}")
    }
}

[test]
def test_rule_may_return_a_constant(t : T?) {
    ast_gc_guard() {
        let b = body_of(t, "target_constant_result")
        t |> success(!has(b, "pow"), "no pow survives: {b}")
        t |> success(has(b, "return 1"), "constant returned: {b}")
    }
}

[test]
def test_rewrites_retry_until_they_settle(t : T?) {
    ast_gc_guard() {
        // sqrt(abs(x)) -> sqrt(x * x) -> the inner pow rule does not apply again; one visit settles
        let b = body_of(t, "target_retry_chain")
        t |> success(!has(b, "abs"), "no abs survives: {b}")
        t |> success(has(b, "sqrt(x * x)"), "chained rewrite settled: {b}")
    }
}

[test]
def test_the_cpp_passes_see_a_das_rewrite_next_round(t : T?) {
    ast_gc_guard() {
        // the rule leaves `x * 1.0`, which only the built-in identity fold removes — and only if the
        // engine stamped the function's optimizationRound so the next round still visits it
        let b = body_of(t, "target_handoff")
        t |> success(!has(b, "cbrt"), "no cbrt survives: {b}")
        t |> success(!has(b, "*"), "the built-in fold removed the multiply: {b}")
        t |> success(has(b, "return x"), "operand returned: {b}")
    }
}

[test]
def test_rewritten_functions_still_compute(t : T?) {
    t |> equal(target_square(g_seed), 9.0)
    t |> equal(target_operand_returned(g_seed), g_seed)
    t |> equal(target_constant_result(g_seed), 1.0)
    t |> equal(target_handoff(g_seed), g_seed)
}
