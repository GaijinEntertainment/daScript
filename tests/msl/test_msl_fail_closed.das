options gen2
options indenting = 4
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

// Phase-2 fail-closed gate (mirrors tests/spirv/test_fail_closed.das): the emitter must reject
// every kernel-illegal construct with a CLEAN compile error (annotation fixup fails, errors
// non-empty, no crash, no silent wrong kernel). Each fixture holds exactly one unsupported
// construct; we compile it with compile_file and assert the error text carries the specific
// diagnostic. The fixtures span the rejection surface: a WRITTEN @uniform (write-set scan),
// an ARRAY @uniform (member scan), a VALUE return in the kernel (statement emit), a BOUND
// @workgroup, and a dynamic-array @workgroup; the user-function surface: a RECURSIVE helper,
// an un-lowerable helper PARAMETER type, and a @workgroup member bound to a thread-space `&`
// parameter; the Phase-5 simdgroup_matrix surface: `.data` access in a kernel, an @ssbo of
// tiles, a non-member load source, and a threadgroup store.

require dastest/testing_boost public
require daslib/ast
require strings

def private compile_issues(file_path : string) : tuple<ok : bool; issues : string> {
    var okFlag = false
    var issuesText = ""
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(file_path, access, unsafe(addr(mg)), cop) $(ok, _program, issues) {
                okFlag = ok
                issuesText := issues
            }
        }
    }
    return (ok = okFlag, issues = issuesText)
}

def private check_rejects(t : T?; fixture, needle : string) {
    // fixtures live in a non-globbed subdir so the tests/msl/*.das AOT glob never tries to compile
    // these intentionally-failing kernels
    let r = compile_issues("{get_das_root()}/tests/msl/_fail_closed/{fixture}.das")
    t |> success(!r.ok, "{fixture}: compilation fails (fail-closed, no silent kernel)")
    t |> success(find(r.issues, needle) >= 0, "{fixture}: error mentions '{needle}'")
    if (r.ok || find(r.issues, needle) < 0) {
        to_log(LOG_ERROR, "FAIL-CLOSED {fixture}: ok={r.ok}, issues=\n{r.issues}\n")
    }
}

[test]
def test_fail_closed_rejections(t : T?) {
    t |> run("emitter rejects kernel-illegal constructs cleanly") <| @(t : T?) {
        check_rejects(t, "_fc_uniform_written", "@uniform member `n` is written")
        check_rejects(t, "_fc_uniform_array", "@uniform member `xs` must be a scalar or vector value")
        check_rejects(t, "_fc_return_value", "kernels return void")
        check_rejects(t, "_fc_recursion", "is recursive — MSL forbids recursion")
        check_rejects(t, "_fc_ufn_param", "type has no MSL form")
        check_rejects(t, "_fc_workgroup_ref", "address spaces differ")
        check_rejects(t, "_fc_wg_binding", "threadgroup memory is not a bindable buffer")
        check_rejects(t, "_fc_wg_type", "@workgroup member `tile` must be a scalar, vector, or single-dimension fixed array")
        check_rejects(t, "_fc_lattice_wide", "is CPU-only (8/16-lane lattice forms do not exist in shaders")
        check_rejects(t, "_fc_lattice_wide_local", "is CPU-only (8/16-lane lattice forms do not exist in shaders")
        check_rejects(t, "_fc_packed_partial", "component or swizzle writes into a packed 3-lane buffer element")
        check_rejects(t, "_fc_sgmat_field", "field access `.data` is not a kernel-class member access")
        check_rejects(t, "_fc_sgmat_member", "element type metal_builtins::simdgroup_float8x8 has no MSL form")
        check_rejects(t, "_fc_sgmat_src", "simdgroup_load source must be an @ssbo or @workgroup member")
        check_rejects(t, "_fc_sgmat_store_tg", "simdgroup_store targets an @ssbo buffer")
    }
}
