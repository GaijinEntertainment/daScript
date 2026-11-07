options gen2
require dastest/testing_boost public
require strings
require _exe_test_common

//! flips to true when the emitter gives LLVM alias information for
//! typed pointers (#4446): no !tbaa and no parameter noalias exist
//! yet
let private TYPED_POINTER_ALIAS_INFO = false

//! a float store cannot change what an int? points at, so the bound
//! and the addend of the fixture's loop are read outside it
[test]
def test_loop_invariant_reads_through_pointers_leave_the_loop(t : T?) {
    if (!TYPED_POINTER_ALIAS_INFO) {
        t |> skip("the JIT emits no alias information for typed pointers yet (#4446)")
        return
    }
    let out = jit_dump(t, "modules/dasLLVM/tests/_pointer_aliasing_root.das", [])
    t |> success(find(out, "SCALE 5.25 5.25") >= 0, "the fixture printed its result: {tail(out)}")
    let ir = function_ir(out,
        "`scale 1`Lt`f`Gt`? C1`Lt`i`Gt`? C1`Lt`i`Gt`? implementation")
    t |> success(!empty(ir), "the dump carries scale: {tail(out)}")
    let body = named_block_ir(ir, "while_body")
    t |> success(!empty(body), "scale keeps a while_body block:\n{ir}")
    t |> success(find(body, "store float") >= 0, "the loop body stores the float:\n{body}")
    t |> equal(-1, find(body, "load i32"),
        "the bound and the addend are read outside the loop, not after each float store:\n{body}")
}
