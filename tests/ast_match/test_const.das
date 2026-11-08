options gen2
options rtti
options _cyclomatic_complexity = 0  // qmatch pattern blocks expand into matcher code; the count is not the test's own

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/ast_match
require dastest/testing_boost public
require daslib/rtti

[export, marker(no_coverage)]
def target_range13() : range => range(13)

[test(description = "qmatch_function matches a compiled function body whose constant-folded range(13) is the pattern's one-value range, binding the lower bound to 0")]
def test_range_one_val_folded(t : T?) {
    ast_gc_guard() {
        // Compiled: range(13) folds to ExprConstRange(0, 13)
        var func = find_module_function_via_rtti(compiling_module(), @@target_range13)
        if (func == null) { t |> success(false, "not found"); return ; }
        var x_val : int
        let r = qmatch_function(func) $ {
            return range($v(x_val), 13)
        }
        t |> success(r.matched, "should match folded range(13): error={int(r.error)}")
        if (r.matched) {
            t |> success(x_val == 0, "x should be 0, got {x_val}")
        }
    }
}

[test(description = "a one-value range pattern matches the raw range(13) call and binds the lower bound to 0")]
def test_range_one_val_raw(t : T?) {
    ast_gc_guard() {
        // Raw qmacro: range(13) stays as ExprCall with 1 arg
        // One-value expansion: range(13) = range(0, 13)
        var expr = qmacro(range(13))
        var x_val : int
        let r = qmatch(expr, range($v(x_val), 13))
        t |> success(r.matched, "raw range(13) should match: error={int(r.error)}")
        if (r.matched) {
            t |> success(x_val == 0, "x should be 0, got {x_val}")
        }
    }
}

[test(description = "a two-value range pattern matches the raw range(5, 13) call and binds the lower bound to 5")]
def test_range_two_val_raw(t : T?) {
    ast_gc_guard() {
        // Two-value form stays as ExprCall with 2 args — should match directly
        var expr = qmacro(range(5, 13))
        var x_val : int
        let r = qmatch(expr, range($v(x_val), 13))
        t |> success(r.matched, "raw range(5, 13) should match: error={int(r.error)}")
        if (r.matched) {
            t |> success(x_val == 5, "x should be 5, got {x_val}")
        }
    }
}

[test(description = "a range(13) call does not match a range pattern whose second bound is fixed to a different value")]
def test_range_one_val_mismatch(t : T?) {
    ast_gc_guard() {
        // range(13) expanded is range(0, 13) — should not match range($v(x), 20)
        var expr = qmacro(range(13))
        var x_val : int
        let r = qmatch(expr, range($v(x_val), 20))
        t |> success(!r.matched, "range(13) should not match range(_, 20)")
    }
}

[test(description = "a one-value urange pattern matches the raw urange(10u) call and binds the lower bound to 0u")]
def test_urange_one_val_raw(t : T?) {
    ast_gc_guard() {
        var expr = qmacro(urange(10u))
        var x_val : uint
        let r = qmatch(expr, urange($v(x_val), 10u))
        t |> success(r.matched, "raw urange(10u) should match: error={int(r.error)}")
        if (r.matched) {
            t |> success(x_val == 0u, "x should be 0u, got {x_val}")
        }
    }
}

[test(description = "a float2 pattern matches the raw float2(3.0) and binds both components to 3.0")]
def test_float2_one_val_raw(t : T?) {
    ast_gc_guard() {
        // float2(3.0) = float2(3.0, 3.0) — broadcast
        var expr = qmacro(float2(3.0))
        var x_val, y_val : float
        let r = qmatch(expr, float2($v(x_val), $v(y_val)))
        t |> success(r.matched, "raw float2(3.0) should match: error={int(r.error)}")
        if (r.matched) {
            t |> success(x_val == 3.0, "x should be 3.0, got {x_val}")
            t |> success(y_val == 3.0, "y should be 3.0, got {y_val}")
        }
    }
}

// ── Target functions with various constant types ─────────────────────────

[export, marker(no_coverage)]
def target_int64(x : int64) : int64 => x + 100l

[export, marker(no_coverage)]
def target_uint(x : uint) : uint => x + 0xFFu

[export, marker(no_coverage)]
def target_double(x : double) : double => x + 3.14lf

[export, marker(no_coverage)]
def target_uint64(x : uint64) : uint64 => x + 1000ul

[export, marker(no_coverage)]
def target_null_check(x : int?) : bool => x == null

[export, marker(no_coverage)]
def target_range() : range => range(1, 10)

[export, marker(no_coverage)]
def target_enum(x : Type) : Type => x

[export, marker(no_coverage)]
def target_float2() : float2 => float2(1.0, 2.0)

[export, marker(no_coverage)]
def target_int2() : int2 => int2(3, 4)

// ── Tests ────────────────────────────────────────────────────────────────

[test(description = "qmatch_function matches a function body that adds an int64 literal")]
def test_int64_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_int64)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : int64) {
            return x + 100l
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

[test(description = "qmatch_function rejects a function body whose int64 literal differs from the pattern's")]
def test_int64_mismatch(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_int64)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : int64) {
            return x + 200l
        }
        t |> success(!r.matched, "wrong int64 value should not match")
    }
}

[test(description = "qmatch_function matches a function body that adds a uint literal")]
def test_uint_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_uint)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : uint) {
            return x + 0xFFu
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

[test(description = "qmatch_function rejects a function body whose uint literal differs from the pattern's")]
def test_uint_mismatch(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_uint)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : uint) {
            return x + 0x00u
        }
        t |> success(!r.matched, "wrong uint value should not match")
    }
}

[test(description = "qmatch_function matches a function body that adds a double literal")]
def test_double_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_double)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : double) {
            return x + 3.14lf
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

[test(description = "qmatch_function rejects a function body whose double literal differs from the pattern's")]
def test_double_mismatch(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_double)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : double) {
            return x + 2.71lf
        }
        t |> success(!r.matched, "wrong double value should not match")
    }
}

[test(description = "qmatch_function matches a function body that adds a uint64 literal")]
def test_uint64_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_uint64)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : uint64) {
            return x + 1000ul
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

[test(description = "qmatch_function matches a function body that compares a pointer parameter with null")]
def test_null_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_null_check)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : int?) {
            return x == null
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

[test(description = "qmatch_function matches a function body returning a two-bound range literal")]
def test_range_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_range)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $ {
            return range(1, 10)
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

[test(description = "qmatch_function rejects a range literal whose bounds differ from the pattern's")]
def test_range_mismatch(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_range)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $ {
            return range(1, 20)
        }
        t |> success(!r.matched, "wrong range should not match")
    }
}

[test(description = "qmatch_function matches a function body returning a float2 literal")]
def test_float2_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_float2)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $ {
            return float2(1.0, 2.0)
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

[test(description = "qmatch_function rejects a float2 literal whose components differ from the pattern's")]
def test_float2_mismatch(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_float2)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $ {
            return float2(1.0, 9.0)
        }
        t |> success(!r.matched, "wrong float2 should not match")
    }
}

[test(description = "qmatch_function matches a function body returning an int2 literal")]
def test_int2_match(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_int2)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $ {
            return int2(3, 4)
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

[test(description = "qmatch_function rejects an int2 literal whose components differ from the pattern's")]
def test_int2_mismatch(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_int2)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $ {
            return int2(3, 99)
        }
        t |> success(!r.matched, "wrong int2 should not match")
    }
}

[test(description = "qmatch_function matches an enum-typed identity body with no constant folding involved")]
def test_identity_enum(t : T?) {
    ast_gc_guard() {
        // enum identity — just body match, no constant folding
        var func = find_module_function_via_rtti(compiling_module(), @@target_enum)
        if (func == null) {
            return
        }
        let r = qmatch_function(func) $(x : Type) {
            return x
        }
        t |> success(r.matched, "identity should match")
    }
}

// ── Tag capture in constant constructors ─────────────────────────────────

[export, marker(no_coverage)]
def target_float4() : float4 => float4(1.0, 2.0, 3.0, 4.0)

[test(description = "$v captures bind the components of a float2 literal in a function body")]
def test_float2_capture_v(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_float2)
        if (func == null) {
            return
        }
        var x_val : float
        let r = qmatch_function(func) $ {
            return float2($v(x_val), 2.0)
        }
        t |> success(r.matched, "should match with $v capture")
        t |> success(x_val == 1.0, "captured x should be 1.0, got {x_val}")
    }
}

[test(description = "a float2 pattern with a mismatching fixed component fails even next to a $v capture")]
def test_float2_capture_v_wrong_fixed(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_float2)
        if (func == null) {
            return
        }
        var x_val : float
        let r = qmatch_function(func) $ {
            return float2($v(x_val), 9.0)
        }
        t |> success(!r.matched, "fixed component mismatch should fail")
    }
}

[test(description = "a $v capture on the second component of an int2 literal binds that component")]
def test_int2_capture_v(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_int2)
        if (func == null) {
            return
        }
        var y_val : int
        let r = qmatch_function(func) $ {
            return int2(3, $v(y_val))
        }
        t |> success(r.matched, "should match with $v on second component")
        t |> success(y_val == 4, "captured y should be 4, got {y_val}")
    }
}

[test(description = "$v captures on the first and fourth components of a float4 literal bind both")]
def test_float4_capture_v(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_float4)
        if (func == null) {
            return
        }
        var x_val, w_val : float
        let r = qmatch_function(func) $ {
            return float4($v(x_val), 2.0, 3.0, $v(w_val))
        }
        t |> success(r.matched, "should match with $v on first and fourth")
        t |> success(x_val == 1.0, "captured x should be 1.0, got {x_val}")
        t |> success(w_val == 4.0, "captured w should be 4.0, got {w_val}")
    }
}

[test(description = "$v captures bind both bounds of a range literal")]
def test_range_capture_v(t : T?) {
    ast_gc_guard() {
        var func = find_module_function_via_rtti(compiling_module(), @@target_range)
        if (func == null) {
            return
        }
        var lo, hi : int
        let r = qmatch_function(func) $ {
            return range($v(lo), $v(hi))
        }
        t |> success(r.matched, "should match with $v capture on both range components")
        t |> success(lo == 1, "lo should be 1, got {lo}")
        t |> success(hi == 10, "hi should be 10, got {hi}")
    }
}
