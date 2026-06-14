options gen2
options indenting = 4

// Builder gate: hand-build the square module via the cached/deduplicated type+constant helpers,
// assert dedup (re-requests return the same id and don't grow the id-bound), assert the section
// shape, and validate the blob with spirv-val. This is the AST-independent foundation the emitter
// is built on (risk #1 type/const dedup + risk #2 SSBO decoration completeness).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_builder
require spirv/spirv_dis
require spirv/spirv_grammar

[test]
def test_builder_dedup_and_validate(t : T?) {
    t |> run("builder: dedup + hand-built square validates") <| @(t : T?) {
        var m = make_module()

        // ----- types + globals via cached helpers (dependency order) -----
        let t_void = type_void(m)
        let t_fn   = type_function_void(m, t_void)
        let t_uint = type_uint(m)
        let t_v3u  = type_vector(m, t_uint, 3)
        let t_p_in_v3 = type_pointer(m, SpvStorageClass.Input, t_v3u)
        let id_gid = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, t_p_in_v3, id_gid, uint(SpvStorageClass.Input))
        emit(m, SEC_DECOR, SpvOp.Decorate, id_gid, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.GlobalInvocationId))

        let c0 = const_uint(m, 0u)
        let t_p_in_u = type_pointer(m, SpvStorageClass.Input, t_uint)
        let t_rtarr = type_runtime_array(m, t_uint, 4)
        var members <- [t_rtarr]
        var offsets <- [0]
        let t_outbuf = type_struct_block(m, members, offsets)
        delete members
        delete offsets
        let t_p_sb_ob = type_pointer(m, SpvStorageClass.StorageBuffer, t_outbuf)
        let id_data = alloc_id(m)
        emit(m, SEC_TYPES, SpvOp.Variable, t_p_sb_ob, id_data, uint(SpvStorageClass.StorageBuffer))
        emit(m, SEC_DECOR, SpvOp.Decorate, id_data, uint(SpvDecoration.DescriptorSet), 0u)
        emit(m, SEC_DECOR, SpvOp.Decorate, id_data, uint(SpvDecoration.Binding), 0u)
        let t_p_sb_u = type_pointer(m, SpvStorageClass.StorageBuffer, t_uint)

        // ----- dedup proof: re-request returns the SAME id, id-bound does not grow -----
        let bound_before = m.next_id
        t |> equal(type_uint(m), t_uint)
        t |> equal(type_void(m), t_void)
        t |> equal(type_pointer(m, SpvStorageClass.Input, t_uint), t_p_in_u)
        t |> equal(type_pointer(m, SpvStorageClass.StorageBuffer, t_uint), t_p_sb_u)
        t |> equal(const_uint(m, 0u), c0)
        t |> equal(m.next_id, bound_before)

        // ----- preamble + entry + exec mode -----
        let id_main = alloc_id(m)
        emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
        emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))
        var ep : array<uint>
        ep |> push(uint(SpvExecutionModel.GLCompute))
        ep |> push(id_main)
        var sw <- string_words("main")
        ep |> push_from(sw)
        delete sw
        ep |> push(id_gid)
        emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
        delete ep
        var em <- [id_main, uint(SpvExecutionMode.LocalSize), 64u, 1u, 1u]
        emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
        delete em

        // ----- function body: data[gid.x] = gid.x * gid.x -----
        let id_label = alloc_id(m)
        let id_gp    = alloc_id(m)
        let id_idx   = alloc_id(m)
        let id_sq    = alloc_id(m)
        let id_ep    = alloc_id(m)
        emit(m, SEC_FUNCS, SpvOp.Function, t_void, id_main, 0u, t_fn)
        emit(m, SEC_FUNCS, SpvOp.Label, id_label)
        emit(m, SEC_FUNCS, SpvOp.AccessChain, t_p_in_u, id_gp, id_gid, c0)
        emit(m, SEC_FUNCS, SpvOp.Load, t_uint, id_idx, id_gp)
        emit(m, SEC_FUNCS, SpvOp.IMul, t_uint, id_sq, id_idx, id_idx)
        var ac <- [t_p_sb_u, id_ep, id_data, c0, id_idx]
        emit_n(m, SEC_FUNCS, SpvOp.AccessChain, ac)
        delete ac
        emit(m, SEC_FUNCS, SpvOp.Store, id_ep, id_sq)
        emit(m, SEC_FUNCS, SpvOp.Return)
        emit(m, SEC_FUNCS, SpvOp.FunctionEnd)

        var words <- module_words(m)
        delete m

        // ----- shape (dedup visible in the instruction counts) -----
        t |> equal(words[0], SPV_MAGIC)
        t |> equal(count_op(words, SpvOp.TypeInt), 1)         // uint emitted once
        t |> equal(count_op(words, SpvOp.TypePointer), 4)     // in_v3, in_uint, sb_outbuf, sb_uint
        t |> equal(count_op(words, SpvOp.Constant), 1)        // const 0 once
        t |> equal(count_op(words, SpvOp.Variable), 2)

        // ----- the real oracle -----
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
