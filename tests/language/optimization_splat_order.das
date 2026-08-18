options gen2
options disable_auto_inline

require dastest/testing_boost public

// The splat-collapse fold `floatN(s) * v -> v * s` SWAPS the operands. When both carry
// side effects that swap reorders them, so the fold must decline. Auto-inline is off
// file-wide because the inliner would hoist a() and b() into locals before the fold and
// hide the reorder (which is exactly how it stayed invisible in optimization_folding).

var g_order = 0

def a() : float {
    g_order = g_order * 10 + 1
    return 2.0
}

def b() : float3 {
    g_order = g_order * 10 + 2
    return float3(1.0, 2.0, 3.0)
}

// one-sided impurity: the swap is still observable when only ONE operand has effects,
// because a pure operand can READ what the impure one writes - each gate clause alone
var g_scalar = 1.0
var g_vec3 = float3(1.0, 1.0, 1.0)

def bump_scalar_ret_vec() : float3 {
    g_scalar = 10.0
    return float3(1.0, 1.0, 1.0)
}

def bump_vec_ret_scalar() : float {
    g_vec3 = float3(10.0, 10.0, 10.0)
    return 1.0
}

[test]
def test_splat_swap_declines_when_only_right_is_impure(t : T?) {
    g_scalar = 1.0
    t |> equal(float3(g_scalar) * bump_scalar_ret_vec(), float3(1.0, 1.0, 1.0), "splat argument read before the right operand writes it")
}

[test]
def test_splat_swap_declines_when_only_left_is_impure(t : T?) {
    g_vec3 = float3(1.0, 1.0, 1.0)
    t |> equal(float3(bump_vec_ret_scalar()) * g_vec3, float3(10.0, 10.0, 10.0), "splat argument writes before the right operand is read")
}

[test]
def test_splat_swap_keeps_evaluation_order(t : T?) {
    g_order = 0
    let r = float3(a()) * b()
    t |> equal(r, float3(2.0, 4.0, 6.0))
    t |> equal(g_order, 12, "left operand evaluated first")
    g_order = 0
    let r2 = b() * float3(a())
    t |> equal(r2, float3(2.0, 4.0, 6.0))
    t |> equal(g_order, 21, "left operand evaluated first")
}
