options gen2

// PHASE-1 increment (temporary scaffold): rebuild the square module's types via the cached,
// deduplicated type/constant helpers, proving the dedup pools produce a valid module. Function
// body is still hand-driven (no AST codegen yet — that is spirv_emit). Validates the #1 risk
// (type/constant dedup correctness) before the AST visitor is built on top.

require spirv_builder
require spirv_dis
require spirv_grammar
require fio

[export]
def main {
    var out_path = "square_typed.spv"
    let args <- get_command_line_arguments()
    if (length(args) > 0) {
        out_path = args[length(args) - 1]
    }

    var m = make_module()

    // ----- types + globals via cached helpers (dependency order) -----
    let t_void  = type_void(m)
    let t_fn    = type_function_void(m, t_void)
    let t_uint  = type_uint(m)
    let t_v3u   = type_vector(m, t_uint, 3)
    let t_p_in_v3 = type_pointer(m, SpvStorageClass.Input, t_v3u)

    let id_gid = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Variable, t_p_in_v3, id_gid, uint(SpvStorageClass.Input))
    emit(m, SEC_DECOR, SpvOp.Decorate, id_gid, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.GlobalInvocationId))

    let c0      = const_uint(m, 0u)
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

    // ----- dedup proof: re-requesting an existing type/const returns the SAME id -----
    var dedup_ok = true
    dedup_ok = dedup_ok && type_uint(m) == t_uint
    dedup_ok = dedup_ok && type_void(m) == t_void
    dedup_ok = dedup_ok && type_pointer(m, SpvStorageClass.Input, t_uint) == t_p_in_u
    dedup_ok = dedup_ok && type_pointer(m, SpvStorageClass.StorageBuffer, t_uint) == t_p_sb_u
    dedup_ok = dedup_ok && const_uint(m, 0u) == c0
    let bound_after = m.next_id   // must NOT have grown from the re-requests above

    // ----- capability / memory model / entry / exec mode -----
    let id_main  = alloc_id(m)
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
    let bound_final = m.next_id
    delete m

    // ----- shape + dedup assertions -----
    var ok = dedup_ok
    ok = ok && bound_after == bound_final - 6u    // only id_main + 5 body ids allocated after dedup checks
    ok = ok && words[0] == SPV_MAGIC
    ok = ok && count_op(words, SpvOp.TypeInt) == 1        // uint emitted once despite many requests
    ok = ok && count_op(words, SpvOp.TypePointer) == 4    // in_v3, in_uint, sb_outbuf, sb_uint — each once
    ok = ok && count_op(words, SpvOp.Constant) == 1       // const 0 once
    ok = ok && count_op(words, SpvOp.Variable) == 2
    ok = ok && has_op(words, SpvOp.IMul)

    print("{dump(words)}")
    print("---\n")
    print("dedup + shape assertions: {ok ? "PASS" : "FAIL"}\n")
    print("id bound: {bound_final}, words: {length(words)}\n")

    fopen(out_path, "wb") <| $(f) {
        if (f != null) {
            fwrite(f, words)
        }
    }
    print("wrote {out_path}\n")
    delete words
}
