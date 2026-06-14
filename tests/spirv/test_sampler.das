options gen2
options indenting = 4

// Phase-4d combined image sampler gate: a sampler2D global lowers to OpTypeImage + OpTypeSampledImage
// in UniformConstant storage with DescriptorSet/Binding; texture(tex, uv) lowers to OpLoad (the
// sampled image) + OpImageSampleImplicitLod. The blob is spirv-val'd.

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
def test_sampler_fragment(t : T?) {
    t |> run("sampler: sampler2D -> OpTypeImage/SampledImage (UniformConstant) + texture() sample") <| @(t : T?) {
        var words <- tex_frag_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "tex_frag: EntryPoint model is Fragment")
        // the image type pair
        t |> success(has_op(words, SpvOp.TypeImage), "tex_frag: emits OpTypeImage")
        t |> success(has_op(words, SpvOp.TypeSampledImage), "tex_frag: emits OpTypeSampledImage")
        // a UniformConstant-storage OpVariable (the sampler descriptor) at operand index 2
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.UniformConstant)),
            "tex_frag: has a UniformConstant-storage OpVariable")
        // descriptor decorations
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 0u),
            "tex_frag: DescriptorSet 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "tex_frag: Binding 0")
        // the sample: OpLoad the sampled image + OpImageSampleImplicitLod
        t |> success(has_op(words, SpvOp.Load), "tex_frag: OpLoad the sampled image")
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod), "tex_frag: texture() -> OpImageSampleImplicitLod")
        validate(t, words, "tex_frag")
        delete words
    }
}
