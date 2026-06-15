options gen2
options indenting = 4

// Phase-7.3 sampler dimensionalities: samplerCube / sampler3D / sampler2DArray lower to OpTypeImage
// with the matching Dim (Cube / 3D / 2D) and Arrayed flag; all sample via texture() (OpImageSample-
// ImplicitLod). No new opcodes vs Phase-7.2 -- only different OpTypeImage operands. spirv-val'd.

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
def test_sampler_dims(t : T?) {
    t |> run("samplerCube/3D/2DArray -> OpTypeImage Dim Cube/3D + Arrayed") <| @(t : T?) {
        var words <- dims_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.Fragment),
            "dims: EntryPoint model is Fragment")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ...]. Dim at index 2,
        // Arrayed at index 4. Three image types are emitted; assert each distinguishing operand is present.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim.Cube)),
            "dims: a samplerCube image (Dim Cube)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 2, uint(SpvDim._3D)),
            "dims: a sampler3D image (Dim 3D)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 4, 1u),
            "dims: a sampler2DArray image (Arrayed = 1)")
        t |> success(has_op(words, SpvOp.ImageSampleImplicitLod), "dims: texture() -> OpImageSampleImplicitLod")
        validate(t, words, "dims")
        delete words
    }
}
