options gen2
require dastest/testing_boost public
require strings
require _exe_test_common

//! a pointer difference lowers to i_das_ptr_diff(a, b, stride),
//! which the interpreter and the AOT inline; the dump shows the
//! JIT's form
[test]
def test_a_pointer_difference_is_arithmetic_not_a_call(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_ptr_diff_root.das", [])
    t |> success(find(out, "DIST 5 -5") >= 0, "the fixture printed both distances: {tail(out)}")
    t |> success(find(out, "DIST64 -5") >= 0,
        "the unnarrowed backward distance divides signed: {tail(out)}")
    let ir = function_ir(out,
        "`distance C1`Lt`S`Lt`S`Gt``Gt`? C1`Lt`S`Lt`S`Gt``Gt`? implementation")
    t |> success(!empty(ir), "the dump carries distance: {tail(out)}")
    t |> equal(-1, find(ir, "i_das_ptr_diff"),
        "the difference is inline arithmetic, not a call through the i_das_ptr_diff slot:\n{ir}")
    t |> success(find(ir, "sub i64") >= 0, "the difference subtracts the two addresses:\n{ir}")
}
