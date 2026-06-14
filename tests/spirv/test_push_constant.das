options gen2
options indenting = 4

// Phase-4c push-constant gate: a @push_constant struct lowers to a PushConstant-storage,
// Block-decorated OpTypeStruct with std140 member offsets, read via OpAccessChain — and crucially
// with NO DescriptorSet/Binding decorations (push constants are not descriptors). Every blob is
// spirv-val'd.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_push_constant_fragment(t : T?) {
    t |> run("push_constant: @push_constant struct -> PushConstant/Block, no descriptor, member reads") <| @(t : T?) {
        var words <- pc_frag_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "pc_frag: EntryPoint model is Fragment")
        // a PushConstant-storage OpVariable (storage class at operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.PushConstant)),
            "pc_frag: has a PushConstant-storage OpVariable")
        // Block-decorated, with std140 offsets (tint@0, gamma@16)
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)),
            "pc_frag: struct is Block-decorated")
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), 0u),
            "pc_frag: tint @ offset 0")
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), 16u),
            "pc_frag: gamma @ offset 16")
        // push constants are NOT descriptors: no DescriptorSet / Binding decoration anywhere
        t |> success(!op_has_operand(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet)),
            "pc_frag: no DescriptorSet decoration")
        t |> success(!op_has_operand(words, SpvOp.Decorate, uint(SpvDecoration.Binding)),
            "pc_frag: no Binding decoration")
        // member access + the scalar-broadcast multiply
        t |> success(has_op(words, SpvOp.AccessChain), "pc_frag: member access via OpAccessChain")
        t |> success(has_op(words, SpvOp.VectorTimesScalar), "pc_frag: tint * gamma -> VectorTimesScalar")
        validate(t, words, "pc_frag")
        delete words
    }
}
