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
def test_fail_closed_rejections(t : T?) {   // nolint:STYLE038 — flat one-check-per-fixture list
    t |> run("emitter rejects shader-illegal constructs cleanly") <| @(t : T?) {
        check_rejects(t, "_fc_call", "cannot call 'safe_acos'")
        check_rejects(t, "_fc_global", "unsupported global 'g_scale'")
        check_rejects(t, "_fc_image_format", "unsupported @format")
        check_rejects(t, "_fc_local_struct_nested", "struct/array members of a local struct value are not yet supported")
        // the same nested shape on the MEMORY path: a `var` struct local's undecorated OpTypeStruct
        // has no recursive plain-struct builder, so the field is refused rather than guessed at
        check_rejects(t, "_fc_local_struct_var_nested", "nested-struct fields are not yet supported")
        check_rejects(t, "_fc_localtype", "type tInt64 is not supported")
        check_rejects(t, "_fc_stmt", "memzero is not supported in SPIR-V shaders")
        check_rejects(t, "_fc_deadcode", "unreachable code after a block terminator")
        // a write-swizzle chains a component pointer off its base, so the base needs an OpVariable; a
        // swizzle of a swizzle has only a loaded value, and there is nothing to store through
        check_rejects(t, "_fc_wswz", "write-swizzle target must be addressable")
        check_rejects(t, "_fc_wgtype", "@workgroup type must be a scalar, vector, matrix, or fixed array")
        check_rejects(t, "_fc_atomicmem", "atomic target must live in @workgroup or @ssbo memory")
        // atomicAdd is the only float-capable atomic; the rest of the family declares no float
        // overload, so a float atomicMax is rejected at the call rather than lowered to OpAtomicSMax
        check_rejects(t, "_fc_atomicfloat", "no matching functions or generics: atomicMax")
        // a buffer image has no mips and SPIR-V forbids a Lod operand on Dim=Buffer, so textureBuffer
        // declares only the two-argument texelFetch: the sampler-shaped 3-arg call is rejected at the call
        check_rejects(t, "_fc_texel_buffer_lod", "no matching functions or generics: texelFetch")
        // @ssbo array<array<T>> is a descriptor array of buffers; a third array level has no
        // descriptor meaning and no byte layout, so it is rejected rather than given a made-up stride
        check_rejects(t, "_fc_ssbo_array_nested", "element type tArray is not supported")
        // a matrix column read is an OpCompositeExtract, whose index is a literal, and a matrix value
        // has no OpVariable to chain a dynamic index through -- so m[i] needs a compile-time i
        check_rejects(t, "_fc_matrix_dyn_col", "indexing a matrix with a non-constant column index")
        // user-defined function call rejections (Phase: shader functions)
        check_rejects(t, "_fc_recursion", "recursion is not supported")
        check_rejects(t, "_fc_mutual", "recursion is not supported")
        check_rejects(t, "_fc_badparam", "parameter 'x' of type tInt64 is not supported")
        check_rejects(t, "_fc_badret", "return type tInt64 is not supported")
        // a 4-parameter [spirv_decode] is legal only as a kernel-class method (the leading self is
        // erased from the emitted signature); a free function with 4 params is rejected outright
        check_rejects(t, "_fc_decode4", "must be a class method (leading self)")
        // subpass input attachments + subpassLoad are fragment-only
        check_rejects(t, "_fc_subpass_stage", "subpassInput 'fc_subpass' is fragment-only")
        // mesh-shader output config: bad topology string (parsed in patch) + non-positive count (emit gate)
        check_rejects(t, "_fc_mesh_topology", "output_topology must be")
        check_rejects(t, "_fc_mesh_count", "max_vertices must be > 0")
        // mesh-output arrays: a per-vertex/per-primitive output outside a mesh stage, and a triangle
        // index array under a non-triangle output topology
        check_rejects(t, "_fc_mesh_vertices_stage", "is only available in a mesh shader")
        check_rejects(t, "_fc_mesh_indices_topology", "requires output_topology")
        // gl_PrimitiveID is a fragment Input builtin (+ closest-hit); reading it in a vertex shader is rejected
        check_rejects(t, "_fc_primid_stage", "is not available in the vertex stage")
        // a mesh @out array<T> with an unsupported element type (int64) fails closed, not crash emit_type
        check_rejects(t, "_fc_mesh_out_badtype", "not a supported varying type")
        // @task_payload outside a task/mesh stage, and a payload with a nested-struct field
        check_rejects(t, "_fc_payload_stage", "is only valid in a task or mesh shader")
        check_rejects(t, "_fc_payload_nested", "nested-struct fields are not yet supported")
        // ray tracing: builtins / the traceRayEXT intrinsic / the TLAS descriptor / the RT storage
        // classes are all RT-pipeline-stage-gated, and the trace payload must be a payload GLOBAL
        check_rejects(t, "_fc_rt_builtin_stage", "is not available in the compute stage")
        check_rejects(t, "_fc_rt_trace_stage", "traceRayEXT is only valid in a raygen / miss / closest-hit shader")
        check_rejects(t, "_fc_rt_accel_stage", "accelerationStructureEXT 'fc_rta_tlas' is only valid in a raygen / miss / closest-hit shader")
        check_rejects(t, "_fc_ray_query_committed", "the committed argument must be a bool literal")
        check_rejects(t, "_fc_rt_payload_stage", "@ray_payload 'fc_rtp_payload' is only valid in a raygen / miss / closest-hit shader")
        check_rejects(t, "_fc_rt_incoming_stage", "@incoming_ray_payload 'fc_rti_prd' is only valid in a miss or closest-hit shader")
        check_rejects(t, "_fc_rt_hitattr_stage", "@hit_attribute 'fc_rth_bary' is only valid in a closest-hit shader")
        check_rejects(t, "_fc_rt_trace_payload", "the payload argument must be a @ray_payload / @incoming_ray_payload global")
        // subgroup: a clustered reduction's ClusterSize must be a constant power of two, and the
        // workgroup-scoped builtins are illegal outside a workgroup-dispatched stage
        check_rejects(t, "_fc_subgroup_cluster", "cluster_size must be a compile-time constant power of two")
        check_rejects(t, "_fc_subgroup_id_stage", "is not available in the fragment stage")
        // spec constants: 32-bit scalar only, literal default required, unique SpecIds (incl. the
        // local_size_*_id axes), and read-only in the shader body
        check_rejects(t, "_fc_spec_type", "must be a 32-bit scalar")
        check_rejects(t, "_fc_spec_noinit", "needs a literal scalar initializer")
        check_rejects(t, "_fc_spec_dupid", "duplicate @constant_id 8")
        check_rejects(t, "_fc_spec_write", "are read-only in a shader")
        check_rejects(t, "_fc_spec_axis_dup", "duplicates another SpecId")
        // gl_WorkGroupSize is the declared LocalSize: only the workgroup-dispatched stages have one
        check_rejects(t, "_fc_wgsize_stage", "builtin 'gl_WorkGroupSize' is not available in the fragment stage")
        // cooperative matrices are subgroup-scoped -> compute-only; a coopmat op in a fragment shader is rejected
        check_rejects(t, "_fc_coopmat_stage", "coopmatLoad is only valid in a compute shader")
        // a storage image's access qualifier is binding: no read of a @writeonly, no write to a @readonly
        check_rejects(t, "_fc_image_writeonly_load", "is @writeonly (NonReadable)")
        check_rejects(t, "_fc_image_readonly_store", "is @readonly (NonWritable)")
    }
}

[test]
def test_fail_closed_kernel_class(t : T?) {
    t |> run("the [spirv_kernel] class frontend rejects bad classes cleanly") <| @(t : T?) {
        // the annotation demands a class method, and the member scan fail-closes on unannotated /
        // mixed / mis-bound members and buffer-less classes
        check_rejects(t, "_fc_kernel_free", "must annotate a class method")
        check_rejects(t, "_fc_kernel_plain_member", "is none of @ssbo/@uniform/@push_constant/@workgroup/@private_global")
        check_rejects(t, "_fc_kernel_mixed", "mixes resource kinds")
        check_rejects(t, "_fc_kernel_set", "a kernel class is one descriptor set")
        check_rejects(t, "_fc_kernel_nobinding", "has no @binding")
        check_rejects(t, "_fc_kernel_wg_binding", "take no binding")
        check_rejects(t, "_fc_kernel_no_ssbo", "has no @ssbo members")
        check_rejects(t, "_fc_kernel_method_rec", "method call cycle")
        check_rejects(t, "_fc_kernel_method_abstract", "has no resolvable body")
    }
}
