options gen2
options indenting = 4

// PR-G gate: a UBO whose top-level fields include a fixed-array of vec4, a nested struct, and an
// array of struct -- the three composite shapes pre-PR-G build_block_struct rejected. Asserts:
//   - Block decoration on the outer struct (UBO contract)
//   - per-member Offset cascade matching the std140 layout (palette@0 / material@48 / lights@80)
//   - ArrayStride 16 on the float4[3] (every std140 array slot rounds up to vec4)
//   - the inner NMaterial struct laid out non-Block at its members' offsets (0, 16, 20)
//   - ArrayStride 32 on the NLight[2] (vec4 + vec4)
//   - the access chains for every read shape (.material.field / .lights[i].field / .palette[i])
//   - spirv-val accepts the full module

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

def private has_member_offset(words : array<uint>; off : uint) : bool {
    return op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), off)
}

// OpDecorate <id> ArrayStride <stride>: stride lives at operand index 2 immediately after the
// decoration token at index 1. op_has_operand_pair pins the pair so a coincidental ArrayStride on
// some other id with a different stride won't satisfy the assert.
def private has_array_stride(words : array<uint>; stride : uint) : bool {
    return op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.ArrayStride), stride)
}

[test]
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
