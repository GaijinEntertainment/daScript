options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── Generic functions ────────────────────────────────────────────────────

[sideeffects]
def generic_add(a, b : auto) : auto => a + b

[sideeffects]
def generic_identity(x : auto) : auto => x

[sideeffects]
def generic_branch(a : auto; threshold : int) : auto {
    if (a > threshold) {
        return a
    } else {
        return threshold
    }
}

// ── Target functions that call generics ──────────────────────────────────

[export, marker(no_coverage)]
def target_add_int(a, b : int) : int => generic_add(a, b)

[export, marker(no_coverage)]
def target_add_float(a, b : float) : float => generic_add(a, b)

[export, marker(no_coverage)]
def target_identity(x : int) : int => generic_identity(x)

[export, marker(no_coverage)]
def target_branch(a : int; threshold : int) : int => generic_branch(a, threshold)

// ── Tests ────────────────────────────────────────────────────────────────

[test(description = "qmatch_function matches a function whose body calls a generic instantiated for int")]
def test_add_int_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_int)
        t |> success(func != null, "should find target_add_int")
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a, b : int) {
            return generic_add(a, b)
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

[test(description = "qmatch_function matches a function whose body calls a generic instantiated for float")]
def test_add_float_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_float)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a, b : float) {
            return generic_add(a, b)
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

[test(description = "qmatch_function matches a function whose body calls a generic identity")]
def test_identity_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_identity)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : int) {
            return generic_identity(x)
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

[test(description = "qmatch_function rejects a call pattern naming a different generic")]
def test_wrong_generic_name(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_int)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a, b : int) {
            return generic_identity(a)
        }
        t |> success(!r.matched, "wrong generic name should not match")
    }
}

[test(description = "qmatch_function rejects an inlined-arithmetic pattern against a body that calls the generic")]
def test_wrong_pattern(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_int)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a, b : int) {
            return a + b
        }
        // target_add_int calls generic_add(a,b), not a+b directly
        t |> success(!r.matched, "inlined body should not match call pattern")
    }
}

[test(description = "a $c capture binds the generic's name and $i captures bind its argument names")]
def test_capture_names(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_int)
        if (func == null) {
            return
        }
        var call_name, lhs_name, rhs_name : string
        let r = qmatch_function(func) $(a, b : int) {
            return $c(call_name)($i(lhs_name), $i(rhs_name))
        }
        if (r.matched) {
            t |> success(true)
            t |> equal(call_name, "generic_add")
            t |> equal(lhs_name, "a")
            t |> equal(rhs_name, "b")
        } else {
            t |> success(false, "should match call with captures")
        }
    }
}

[test(description = "a function calling a branching generic matches either the call or the folded ternary form")]
def test_branch_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_branch)
        t |> success(func != null, "should find target_branch")
        if (func == null) {
            return
        }
        // After optimization, may fold to ternary or stay as call
        var cond : ExpressionPtr
        let r_call = qmatch_function(func) $(a : int; threshold : int) {
            return generic_branch(a, threshold)
        }
        let r_ternary = qmatch_function(func) $(a : int; threshold : int) {
            return $e(cond) ? $e(cond) : $e(cond)
        }
        t |> success(r_call.matched || r_ternary.matched, "should match call or ternary")
    }
}

[test(description = "qmatch_function checks the instantiated parameter types of a generic caller")]
def test_add_arg_types(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_int)
        if (func == null) {
            return
        }
        // Verify arg types are int (not auto)
        let r = qmatch_function(func) $(a : int; b : int) : int {
            return generic_add(a, b)
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

[test(description = "a pattern with float parameter types does not match the int instantiation")]
def test_float_arg_types_dont_match_int(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_add_int)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(a : float; b : float) : float {
            return generic_add(a, b)
        }
        t |> success(!r.matched, "int function should not match float arg types")
    }
}
