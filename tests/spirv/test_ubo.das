options gen2

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

// Phase-4a UBO gate: a @uniform struct lowered to a Uniform-storage, Block-decorated OpTypeStruct
// with std140 member offsets, read through OpAccessChain by field index. Asserts the descriptor
// decorations (DescriptorSet/Binding), the Block decoration, and EVERY std140 member offset (the
// two packing edge cases — scalar after vec2, scalar after vec3 — are the whole point). Every
// emitted module passes spirv-val.
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
def private has_member_offset(words : array<uint>; off : uint) : bool =>
    op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), off)

[test(description = "A @uniform struct becomes a Uniform Block with std140 offsets and OpAccessChain member reads.")]
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

// OpDecorate <id> ArrayStride <stride>: stride lives at operand index 2 immediately after the
// decoration token at index 1. op_has_operand_pair pins the pair so a coincidental ArrayStride on
// some other id with a different stride won't satisfy the assert.
def private has_array_stride(words : array<uint>; stride : uint) : bool =>
    op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), stride)

[test(description = "A uniform block with an array of vec4, a nested struct and an array of structs gets the right ArrayStrides, inner Offsets and access chains.")]
def test_ubo_nested(t : T?) {
    t |> run("uniform: array<vec4> + nested struct + array<struct> -> ArrayStride + inner Offsets + access chains") <| @(t : T?) {
        var words <- ubo_nested_words()
        // a fragment entry point
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "ubo_nested: EntryPoint model is Fragment")
        // outer struct is a Uniform-storage OpVariable + Block-decorated + descriptor decorations
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Uniform)),
            "ubo_nested: Uniform-storage OpVariable")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)),
            "ubo_nested: outer struct is Block-decorated")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 0u),
            "ubo_nested: DescriptorSet 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "ubo_nested: Binding 0")
        // top-level field offsets: palette@0, material@48 (after 3*16=48), lights@80 (after material
        // laid out at 24, rounded to align 16 -> 32; 48+32=80).
        t |> success(has_member_offset(words, 0u),  "ubo_nested: palette @ 0")
        t |> success(has_member_offset(words, 48u), "ubo_nested: material @ 48 (palette = 3*vec4 = 48 bytes)")
        t |> success(has_member_offset(words, 80u), "ubo_nested: lights @ 80 (material laid out + padded to 32)")
        // NMaterial inner offsets (non-Block laid out at the same offsets as if it were a Block):
        // base_color@0, roughness@16, metallic@20.
        t |> success(has_member_offset(words, 16u), "ubo_nested: NMaterial.roughness @ inner offset 16")
        t |> success(has_member_offset(words, 20u), "ubo_nested: NMaterial.metallic @ inner offset 20")
        // NLight inner offsets: pos@0, color@16. (pos@0 collides with palette@0; color@16 collides
        // with material@16-after-base. Both 16 hits already covered above; explicit-check pos@0 not
        // distinguishable from palette@0 -- relying on the spirv-val gate to catch missing inner
        // offsets is fine because spirv-val FAILS on a Block member without Offset.)
        // ArrayStride: the inner two arrays.
        t |> success(has_array_stride(words, 16u), "ubo_nested: float4[3] ArrayStride 16 (std140 vec4)")
        t |> success(has_array_stride(words, 32u), "ubo_nested: NLight[2] ArrayStride 32 (vec4 + vec4)")
        // access chains: every read shape lands as OpAccessChain. The block has at least 6 of them
        // (one per read site), but assert presence + a lower bound to catch silent regression where
        // one of the new branches misses the AccessChain emission.
        t |> success(has_op(words, SpvOp.AccessChain), "ubo_nested: member access via OpAccessChain")
        let nchain = count_op(words, SpvOp.AccessChain)
        t |> success(nchain >= 6, "ubo_nested: at least 6 OpAccessChain (palette, material.base_color, material.roughness, lights[i], .pos, .color) (got {nchain})")
        validate(t, words, "ubo_nested")
        delete words
    }
}

[test(description = "A @push_constant struct becomes a PushConstant Block with no descriptor and working member reads.")]
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
