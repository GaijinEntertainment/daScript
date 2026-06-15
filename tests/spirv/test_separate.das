options gen2
options indenting = 4

// Phase-7.4 separate image + sampler: a texture2D global lowers to a standalone OpTypeImage, a sampler
// global to a standalone OpTypeSampler -- two independent UniformConstant descriptors. sampleTexture
// combines them at the call site (OpSampledImage) before OpImageSampleImplicitLod. spirv-val'd.

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
def test_separate_image_sampler(t : T?) {
    t |> run("texture2D + sampler -> standalone OpTypeImage/OpTypeSampler + OpSampledImage combine") <| @(t : T?) {
        var words <- septex_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "septex: EntryPoint model is Fragment")
        t |> success(has_op(words, SpvOp.TypeImage), "septex: a standalone OpTypeImage (texture2D)")
        t |> success(has_op(words, SpvOp.TypeSampler), "septex: a standalone OpTypeSampler (sampler)")
        t |> success(has_op(words, SpvOp.SampledImage), "septex: sampleTexture combines via OpSampledImage")
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod), "septex: sample -> OpImageSampleImplicitLod")
        // two UniformConstant descriptors at bindings 0 (image) and 1 (sampler)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.UniformConstant)),
            "septex: UniformConstant OpVariables")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "septex: image at Binding 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 1u),
            "septex: sampler at Binding 1")
        validate(t, words, "septex")
        delete words
    }
}
