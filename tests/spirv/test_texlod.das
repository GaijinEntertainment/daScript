options gen2
options indenting = 4

// Phase-7.2 explicit-LOD + texel fetch: textureLod -> OpImageSampleExplicitLod (with a Lod image
// operand), texelFetch -> OpImage (extract the image from the sampled image) + OpImageFetch. Both are
// derivative-free, so the fixture samples in a COMPUTE shader -- proving the stage lift (texture()'s
// implicit-LOD form is fragment-only; these are not). spirv-val'd.

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
def test_explicit_lod_and_fetch(t : T?) {
    t |> run("textureLod/texelFetch -> ExplicitLod + OpImage/ImageFetch (compute, stage-agnostic)") <| @(t : T?) {
        var words <- texlod_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.GLCompute),
            "texlod: EntryPoint model is GLCompute (sampling outside a fragment shader)")
        t |> success(has_op(words, SpvOp.ImageSampleExplicitLod), "texlod: textureLod -> OpImageSampleExplicitLod")
        t |> success(has_op(words, SpvOp.Image), "texlod: texelFetch extracts the image via OpImage")
        t |> success(has_op(words, SpvOp.ImageFetch), "texlod: texelFetch -> OpImageFetch")
        validate(t, words, "texlod")
        delete words
    }
}
