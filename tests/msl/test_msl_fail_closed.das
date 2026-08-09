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
// tiles, a non-member load source, and a threadgroup store; the flatten surface: recursive
// methods in BOTH positions (the value one is refused while binding arguments, the statement
// one only by the scan — emit lowers a splice by recursion), the value-position body shape, a
// discarded value return, a non-tail `return`, the state-member kinds (bound, initialized
// pointer, unbound pointer, dual-root pointer), and the splice argument binder (writable scalar
// and writable pointer in value position, an un-lowerable parameter type, a twice-named
// parameter, an untraceable pointer space, a non-pointer argument, a non-aliasable `&`
// argument).

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
        check_rejects(t, "_fc_uniform_array", "array members are @ssbo buffers")
        check_rejects(t, "_fc_return_value", "kernels return void")
        check_rejects(t, "_fc_recursion", "is recursive - MSL forbids recursion")
        check_rejects(t, "_fc_ufn_param", "type has no MSL form")
        check_rejects(t, "_fc_workgroup_ref", "address spaces differ")
        check_rejects(t, "_fc_ptr_space_tg", "is `threadgroup` but the argument points into device memory")
        check_rejects(t, "_fc_ptr_space_dev", "is `device` but the argument points into threadgroup memory")
        check_rejects(t, "_fc_ptr_untraceable", "has no traceable address space")
        check_rejects(t, "_fc_ustruct_field", "only int/uint/float fields lower")
        check_rejects(t, "_fc_ustruct_nested", "only int/uint/float fields lower")
        check_rejects(t, "_fc_method_recursion", "method `mrec` is recursive")
        check_rejects(t, "_fc_method_abstract", "has no resolvable body")
        check_rejects(t, "_fc_wg_binding", "threadgroup memory is not a bindable buffer")
        check_rejects(t, "_fc_wg_type", "@workgroup member `tile` must be a scalar, vector, or single-dimension fixed array")
        check_rejects(t, "_fc_lattice_wide", "is CPU-only (8/16-lane lattice forms do not exist in shaders")
        check_rejects(t, "_fc_lattice_wide_local", "is CPU-only (8/16-lane lattice forms do not exist in shaders")
        check_rejects(t, "_fc_packed_partial", "component or swizzle writes into a packed 3-lane buffer element")
        check_rejects(t, "_fc_sgmat_field", "field access `.data` is not a kernel-class member access")
        check_rejects(t, "_fc_sgmat_member", "element type metal_builtins::simdgroup_float8x8 has no MSL form")
        check_rejects(t, "_fc_sgmat_src", "simdgroup_load source must be an @ssbo or @workgroup member")
        check_rejects(t, "_fc_sgmat_store_tg", "simdgroup_store targets an @ssbo buffer")
        // the flatten (method-splice) surface
        check_rejects(t, "_fc_method_recursion_stmt", "method `mrec` is recursive")
        check_rejects(t, "_fc_method_value_body", "used in value position must be a single `return <expr>` body")
        check_rejects(t, "_fc_method_discard", "returns a value its statement call discards")
        check_rejects(t, "_fc_method_nontail", "contains `return` but the call is not in kernel tail position")
        check_rejects(t, "_fc_state_binding", "plain members are thread-locals, not bindable buffers")
        check_rejects(t, "_fc_state_ptr_init", "pointer state member `cur` takes no initializer")
        check_rejects(t, "_fc_state_ptr_unbound", "is never assigned from a kernel buffer")
        check_rejects(t, "_fc_state_ptr_dual", "one root buffer per pointer member")
        check_rejects(t, "_fc_splice_writable_value", "writable parameter `v` cannot splice in value position")
        check_rejects(t, "_fc_splice_param_type", "parameter `p` type has no MSL form")
        check_rejects(t, "_fc_splice_param_multi", "names parameter `k` more than once")
        check_rejects(t, "_fc_splice_ptr_value", "writable parameter `p` cannot splice in value position")
        check_rejects(t, "_fc_splice_ptr_trace", "pointer parameter `p` has no traceable address space")
        check_rejects(t, "_fc_splice_ptr_arg", "buffer/pointer parameter `p` needs a member, pointer local, or addr() argument")
        check_rejects(t, "_fc_splice_ref_arg", "`&` parameter `acc` needs an lvalue argument")
    }
}
