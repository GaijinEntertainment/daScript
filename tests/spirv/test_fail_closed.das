options gen2
options indenting = 4
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

// Phase-6 fail-closed gate. The AstVisitor emitter must reject every shader-illegal construct with a
// CLEAN compile error (annotation patch fails, errors non-empty, no crash, no silent empty blob) —
// never a silently-dropped node. Each fixture is a `[compute_shader]` containing exactly one
// unsupported construct; we compile it with compile_file and assert the failing-shader error text
// carries the specific diagnostic. The four constructs span the rejection surface: an unsupported
// CALL (visitExprCall), GLOBAL (classify_global), LOCAL TYPE (declare_local_var), and STATEMENT
// (the visitor's explicit preVisit rejection — the backstop-can't-see-it case).

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
    // fixtures live in a non-globbed subdir so the tests/spirv/*.das AOT glob never tries to compile
    // these intentionally-failing shaders (no_aot only skips emission, not the failing compile).
    let r = compile_issues("{get_das_root()}/tests/spirv/_fail_closed/{fixture}.das")
    t |> success(!r.ok, "{fixture}: compilation fails (fail-closed, no silent blob)")
    t |> success(find(r.issues, needle) >= 0, "{fixture}: error mentions '{needle}'")
    if (r.ok || find(r.issues, needle) < 0) {
        to_log(LOG_ERROR, "FAIL-CLOSED {fixture}: ok={r.ok}, issues=\n{r.issues}\n")
    }
}

[test]
def test_fail_closed_rejections(t : T?) {
    t |> run("emitter rejects shader-illegal constructs cleanly") <| @(t : T?) {
        check_rejects(t, "_fc_call", "cannot call 'safe_acos'")
        check_rejects(t, "_fc_global", "unsupported global 'g_scale'")
        check_rejects(t, "_fc_localtype", "type tDouble is not supported")
        check_rejects(t, "_fc_stmt", "memzero is not supported in SPIR-V shaders")
        check_rejects(t, "_fc_deadcode", "unreachable code after a block terminator")
        check_rejects(t, "_fc_wswz", "cannot assign through a swizzle")
        check_rejects(t, "_fc_wgtype", "@workgroup type must be a scalar, vector, matrix, or fixed array")
        check_rejects(t, "_fc_atomicmem", "atomic target must live in @workgroup or @ssbo memory")
        // user-defined function call rejections (Phase: shader functions)
        check_rejects(t, "_fc_recursion", "recursion is not supported")
        check_rejects(t, "_fc_mutual", "recursion is not supported")
        check_rejects(t, "_fc_badparam", "is not supported (only 32-bit scalar/vector/matrix parameters")
        check_rejects(t, "_fc_badret", "return type tDouble is not supported")
        // subpass input attachments + subpassLoad are fragment-only
        check_rejects(t, "_fc_subpass_stage", "subpassInput 'fc_subpass' is fragment-only")
        // mesh-shader output config: bad topology string (parsed in patch) + non-positive count (emit gate)
        check_rejects(t, "_fc_mesh_topology", "output_topology must be")
        check_rejects(t, "_fc_mesh_count", "max_vertices must be > 0")
        // mesh-output arrays: a per-vertex/per-primitive output outside a mesh stage, and a triangle
        // index array under a non-triangle output topology
        check_rejects(t, "_fc_mesh_vertices_stage", "is only available in a mesh shader")
        check_rejects(t, "_fc_mesh_indices_topology", "requires output_topology")
        // gl_PrimitiveID is a fragment-only Input builtin (reading it in a vertex shader is rejected)
        check_rejects(t, "_fc_primid_stage", "is only available in the fragment stage")
    }
}
