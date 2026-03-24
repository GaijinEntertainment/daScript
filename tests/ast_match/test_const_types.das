options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/rtti
require daslib/ast_match
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// ── Target functions with various constant types ─────────────────────────

[export]
def target_int64(x : int64) : int64 {
    return x + 100l
}

[export]
def target_uint(x : uint) : uint {
    return x + 0xFFu
}

[export]
def target_double(x : double) : double {
    return x + 3.14lf
}

[export]
def target_uint64(x : uint64) : uint64 {
    return x + 1000ul
}

[export]
def target_null_check(x : int?) : bool {
    return x == null
}

[export]
def target_range() : range {
    return range(1, 10)
}

[export]
def target_enum(x : Type) : Type {
    return x
}

[export]
def target_float2() : float2 {
    return float2(1.0, 2.0)
}

[export]
def target_int2() : int2 {
    return int2(3, 4)
}

// ── Tests ────────────────────────────────────────────────────────────────

[test]
def test_int64_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_int64)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
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

[test]
def test_int64_mismatch(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_int64)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return x + 200l
    }
    t |> success(!r.matched, "wrong int64 value should not match")
}

[test]
def test_uint_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_uint)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
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

[test]
def test_uint_mismatch(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_uint)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return x + 0x00u
    }
    t |> success(!r.matched, "wrong uint value should not match")
}

[test]
def test_double_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_double)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
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

[test]
def test_double_mismatch(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_double)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return x + 2.71lf
    }
    t |> success(!r.matched, "wrong double value should not match")
}

[test]
def test_uint64_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_uint64)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
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

[test]
def test_null_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_null_check)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
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

[test]
def test_range_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_range)
    if (get_ptr(func) == null) {
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

[test]
def test_range_mismatch(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_range)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return range(1, 20)
    }
    t |> success(!r.matched, "wrong range should not match")
}

[test]
def test_float2_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_float2)
    if (get_ptr(func) == null) {
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

[test]
def test_float2_mismatch(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_float2)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return float2(1.0, 9.0)
    }
    t |> success(!r.matched, "wrong float2 should not match")
}

[test]
def test_int2_match(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_int2)
    if (get_ptr(func) == null) {
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

[test]
def test_int2_mismatch(t : T?) {
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_int2)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return int2(3, 99)
    }
    t |> success(!r.matched, "wrong int2 should not match")
}

[test]
def test_identity_enum(t : T?) {
    // enum identity — just body match, no constant folding
    var inscope func <- find_module_function_via_rtti(compiling_module(), @@target_enum)
    if (get_ptr(func) == null) {
        return
    }
    let r = qmatch_function(func) $ {
        return x
    }
    t |> success(r.matched, "identity should match")
}
