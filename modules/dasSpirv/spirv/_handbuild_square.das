options gen2

// PHASE-0 PROOF (temporary scaffold): hand-build the `square.comp` compute module via the
// spirv_builder primitives, assert its shape through spirv_dis, write the .spv, and dump it.
// External: spirv-val must pass; spirv-dis is eyeballed against the reference. No AST codegen
// here — this validates the builder + disassembler + section ordering + string packing.

require spirv_builder
require spirv_dis
require spirv_grammar
require daslib/fio

def build_square() : array<uint> {
    var m = make_module()

    // result-ids (allocation order; numeric value is arbitrary as long as defined-before-use)
    let id_void      = alloc_id(m)
    let id_fnvoid    = alloc_id(m)
    let id_uint      = alloc_id(m)
    let id_v3uint    = alloc_id(m)
    let id_ptr_in_v3 = alloc_id(m)
    let id_gid       = alloc_id(m)
    let id_uint0     = alloc_id(m)
    let id_ptr_in_u  = alloc_id(m)
    let id_rtarr     = alloc_id(m)
    let id_outbuf    = alloc_id(m)
    let id_ptr_sb_ob = alloc_id(m)
    let id_data      = alloc_id(m)
    let id_ptr_sb_u  = alloc_id(m)
    let id_main      = alloc_id(m)
    let id_label     = alloc_id(m)
    let id_gp        = alloc_id(m)
    let id_idx       = alloc_id(m)
    let id_sq        = alloc_id(m)
    let id_ep        = alloc_id(m)

    // capability + memory model
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Shader))
    emit(m, SEC_MEMMODEL, SpvOp.MemoryModel, uint(SpvAddressingModel.Logical), uint(SpvMemoryModel.GLSL450))

    // entry point: OpEntryPoint GLCompute %main "main" %gid
    var ep : array<uint>
    ep |> push(uint(SpvExecutionModel.GLCompute))
    ep |> push(id_main)
    var sw <- string_words("main")
    ep |> push_from(sw)
    delete sw
    ep |> push(id_gid)
    emit_n(m, SEC_ENTRY, SpvOp.EntryPoint, ep)
    delete ep

    // execution mode: LocalSize 64 1 1
    var em <- [id_main, uint(SpvExecutionMode.LocalSize), 64u, 1u, 1u]
    emit_n(m, SEC_EXECMODE, SpvOp.ExecutionMode, em)
    delete em

    // decorations
    emit(m, SEC_DECOR, SpvOp.Decorate, id_gid, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.GlobalInvocationId))
    emit(m, SEC_DECOR, SpvOp.Decorate, id_rtarr, uint(SpvDecoration.ArrayStride), 4u)
    emit(m, SEC_DECOR, SpvOp.MemberDecorate, id_outbuf, 0u, uint(SpvDecoration.Offset), 0u)
    emit(m, SEC_DECOR, SpvOp.Decorate, id_outbuf, uint(SpvDecoration.Block))
    emit(m, SEC_DECOR, SpvOp.Decorate, id_data, uint(SpvDecoration.DescriptorSet), 0u)
    emit(m, SEC_DECOR, SpvOp.Decorate, id_data, uint(SpvDecoration.Binding), 0u)

    // types / constants / global variables
    emit(m, SEC_TYPES, SpvOp.TypeVoid, id_void)
    emit(m, SEC_TYPES, SpvOp.TypeFunction, id_fnvoid, id_void)
    emit(m, SEC_TYPES, SpvOp.TypeInt, id_uint, 32u, 0u)
    emit(m, SEC_TYPES, SpvOp.TypeVector, id_v3uint, id_uint, 3u)
    emit(m, SEC_TYPES, SpvOp.TypePointer, id_ptr_in_v3, uint(SpvStorageClass.Input), id_v3uint)
    emit(m, SEC_TYPES, SpvOp.Variable, id_ptr_in_v3, id_gid, uint(SpvStorageClass.Input))
    emit(m, SEC_TYPES, SpvOp.Constant, id_uint, id_uint0, 0u)
    emit(m, SEC_TYPES, SpvOp.TypePointer, id_ptr_in_u, uint(SpvStorageClass.Input), id_uint)
    emit(m, SEC_TYPES, SpvOp.TypeRuntimeArray, id_rtarr, id_uint)
    emit(m, SEC_TYPES, SpvOp.TypeStruct, id_outbuf, id_rtarr)
    emit(m, SEC_TYPES, SpvOp.TypePointer, id_ptr_sb_ob, uint(SpvStorageClass.StorageBuffer), id_outbuf)
    emit(m, SEC_TYPES, SpvOp.Variable, id_ptr_sb_ob, id_data, uint(SpvStorageClass.StorageBuffer))
    emit(m, SEC_TYPES, SpvOp.TypePointer, id_ptr_sb_u, uint(SpvStorageClass.StorageBuffer), id_uint)

    // function body: data[gid.x] = gid.x * gid.x
    emit(m, SEC_FUNCS, SpvOp.Function, id_void, id_main, 0u, id_fnvoid)   // FunctionControl None
    emit(m, SEC_FUNCS, SpvOp.Label, id_label)
    emit(m, SEC_FUNCS, SpvOp.AccessChain, id_ptr_in_u, id_gp, id_gid, id_uint0)
    emit(m, SEC_FUNCS, SpvOp.Load, id_uint, id_idx, id_gp)
    emit(m, SEC_FUNCS, SpvOp.IMul, id_uint, id_sq, id_idx, id_idx)
    var ac <- [id_ptr_sb_u, id_ep, id_data, id_uint0, id_idx]
    emit_n(m, SEC_FUNCS, SpvOp.AccessChain, ac)
    delete ac
    emit(m, SEC_FUNCS, SpvOp.Store, id_ep, id_sq)
    emit(m, SEC_FUNCS, SpvOp.Return)
    emit(m, SEC_FUNCS, SpvOp.FunctionEnd)

    var words <- module_words(m)
    delete m
    return <- words
}

[export]
def main {
    var out_path = "square_handbuilt.spv"
    let args <- get_command_line_arguments()
    if (length(args) > 0) {
        out_path = args[length(args) - 1]
    }

    var words <- build_square()

    // ----- assert shape via the disassembler -----
    var ok = true
    ok = ok && words[0] == SPV_MAGIC
    ok = ok && words[1] == SPV_VERSION_1_3
    ok = ok && has_op(words, SpvOp.Capability)
    ok = ok && has_op(words, SpvOp.MemoryModel)
    ok = ok && has_op(words, SpvOp.EntryPoint)
    ok = ok && has_op(words, SpvOp.ExecutionMode)
    ok = ok && count_op(words, SpvOp.Decorate) == 5
    ok = ok && count_op(words, SpvOp.MemberDecorate) == 1
    ok = ok && has_op(words, SpvOp.TypeRuntimeArray)
    ok = ok && has_op(words, SpvOp.TypeStruct)
    ok = ok && count_op(words, SpvOp.Variable) == 2
    ok = ok && count_op(words, SpvOp.AccessChain) == 2
    ok = ok && has_op(words, SpvOp.Load)
    ok = ok && has_op(words, SpvOp.IMul)
    ok = ok && has_op(words, SpvOp.Store)
    ok = ok && has_op(words, SpvOp.Return)
    ok = ok && op_has_operand(words, SpvOp.ExecutionMode, 64u)                              // LocalSize x
    ok = ok && op_has_operand(words, SpvOp.Decorate, uint(SpvBuiltIn.GlobalInvocationId))

    print("{dump(words)}")
    print("---\n")
    print("shape assertions: {ok ? "PASS" : "FAIL"}\n")
    print("word count: {length(words)} (bytes: {length(words) * 4})\n")

    // ----- write the .spv for external spirv-val / spirv-dis -----
    fopen(out_path, "wb") <| $(f) {
        if (f != null) {
            fwrite(f, words)
        }
    }
    print("wrote {out_path}\n")
    delete words
}
