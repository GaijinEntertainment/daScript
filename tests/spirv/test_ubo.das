options gen2
options indenting = 4

// Phase-4a UBO gate: a @uniform struct lowered to a Uniform-storage, Block-decorated OpTypeStruct
// with std140 member offsets, read through OpAccessChain by field index. Asserts the descriptor
// decorations (DescriptorSet/Binding), the Block decoration, and EVERY std140 member offset (the
// two packing edge cases — scalar after vec2, scalar after vec3 — are the whole point). Every
// emitted module passes spirv-val.

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

// MemberDecorate operands are [struct_id, member, decoration, value]; Offset is at idx 2 with the
// byte offset adjacent at idx 3, so op_has_operand_pair(Offset, off) is an exact layout assertion.
def private has_member_offset(words : array<uint>; off : uint) : bool {
    return op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), off)
}

[test]
def test_ubo_fragment(t : T?) {
    t |> run("uniform: @uniform struct -> Uniform/Block + std140 offsets + OpAccessChain member reads") <| @(t : T?) {
        var words <- ubo_frag_words()
        // a fragment entry point
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "ubo_frag: EntryPoint model is Fragment")
        // a Uniform-storage OpVariable (storage class at operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Uniform)),
            "ubo_frag: has a Uniform-storage OpVariable")
        // descriptor decorations (decoration adjacent to its value)
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 0u),
            "ubo_frag: DescriptorSet 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "ubo_frag: Binding 0")
        // Block decoration on the interface struct (decoration at operand index 1: [target, Block])
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)),
            "ubo_frag: struct is Block-decorated")
        // the FULL std140 layout: color@0, uv@16, flags@24, scale@28, dir@32, bias@44
        t |> success(has_member_offset(words, 0u), "ubo_frag: color @ std140 offset 0")
        t |> success(has_member_offset(words, 16u), "ubo_frag: uv @ std140 offset 16")
        t |> success(has_member_offset(words, 24u), "ubo_frag: flags @ std140 offset 24 (packs after vec2)")
        t |> success(has_member_offset(words, 28u), "ubo_frag: scale @ std140 offset 28")
        t |> success(has_member_offset(words, 32u), "ubo_frag: dir @ std140 offset 32 (vec3 aligns to 16)")
        t |> success(has_member_offset(words, 44u), "ubo_frag: bias @ std140 offset 44 (packs after vec3)")
        // member reads: OpAccessChain into the block + OpLoad, feeding CompositeConstruct
        t |> success(has_op(words, SpvOp.AccessChain), "ubo_frag: member access via OpAccessChain")
        t |> success(has_op(words, SpvOp.Load), "ubo_frag: member load via OpLoad")
        t |> success(has_op(words, SpvOp.CompositeConstruct), "ubo_frag: float4(member, ...) -> CompositeConstruct")
        validate(t, words, "ubo_frag")
        delete words
    }
}
