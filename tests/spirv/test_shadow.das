options gen2
options indenting = 4

// Phase-10.4 gate: depth-compare (shadow) sampling. textureCompare(sampler2DShadow, uv, ref) lowers to
// OpImageSampleDrefImplicitLod (the reference value as the Dref operand, a scalar result). The sampler's
// OpTypeImage carries the Depth=1 flag that distinguishes a comparison sampler from an ordinary one.
// Each is asserted positionally, and the module is spirv-val-clean -- the real check that the Dref
// operand count and the Depth-flagged image type are well-formed.

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
def test_depth_compare(t : T?) {
    t |> run("depth-compare sampling: sampler2DShadow + textureCompare -> OpImageSampleDrefImplicitLod") <| @(t : T?) {
        var words <- fragshadow_words()
        // a fragment entry point (implicit-LOD Dref sampling is fragment-only)
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragshadow: EntryPoint model is Fragment")
        // textureCompare -> the depth-compare sample op
        t |> success(has_op(words, SpvOp.ImageSampleDrefImplicitLod),
            "fragshadow: textureCompare -> OpImageSampleDrefImplicitLod")
        // the sampler's OpTypeImage marks Depth=1 (comparison) and Dim=2D. OpTypeImage operands:
        // result-id(0), Sampled Type(1), Dim(2), Depth(3), Arrayed(4), MS(5), Sampled(6), Format(7).
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 3, 1u),
            "fragshadow: sampler2DShadow OpTypeImage has Depth=1")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._2D)),
            "fragshadow: sampler2DShadow OpTypeImage Dim=2D")
        validate(t, words, "fragshadow")
        delete words
    }
}
