options gen2
require dastest/testing_boost public
require strings
require _exe_test_common

//! `for` over a const container parameter writes its lock through
//! the parameter, so the parameter is not readonly; a const pointer
//! to a scalar still is. A readonly parameter under noalias reads as
//! constant memory, and the unlock then sees the magic the lock had
//! not yet written
[test]
def test_a_const_container_parameter_is_not_readonly(t : T?) {
    let out = jit_dump(t, "modules/dasLLVM/tests/_const_container_param_root.das", [])
    t |> success(find(out, "SUM 36") >= 0,
        "the fixture iterates its const parameters three times each: {tail(out)}")
    let sig = signature_ir(function_ir(out, "`sum_for C1`Lt`i`Gt`A implementation"))
    t |> success(!empty(sig), "the dump carries sum_for: {tail(out)}")
    t |> equal(-1, find(sig, "readonly"), "a const array parameter carries no readonly:\n{sig}")
    let ptr_sig = signature_ir(function_ir(out, "`sum_ptr C1`Lt`C1`Lt`i`Gt`A`Gt`? implementation"))
    t |> success(!empty(ptr_sig), "the dump carries sum_ptr: {tail(out)}")
    t |> equal(-1, find(ptr_sig, "readonly"),
        "a const pointer to an array carries no readonly:\n{ptr_sig}")
    let scalar = jit_dump(t, "modules/dasLLVM/tests/_pointer_aliasing_root.das", [])
    let scale_sig = signature_ir(function_ir(scalar,
        "`scale 1`Lt`f`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation"))
    t |> success(find(scale_sig, "readonly") >= 0,
        "a const pointer to a scalar keeps readonly:\n{scale_sig}")
}
