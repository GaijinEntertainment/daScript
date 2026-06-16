options gen2
options indenting = 4

// Phase-7.1b storage image: an image2D global lowers to a UniformConstant OpTypeImage with a KNOWN
// Rgba8 format and sampled=2 (read-write) + DescriptorSet/Binding; imageLoad -> OpImageRead,
// imageStore -> OpImageWrite. A known format means no without-format capability is required. spirv-val'd.

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
def test_storage_image_compute(t : T?) {
    t |> run("storage image: image2D -> read-write OpTypeImage (Rgba8) + imageLoad/imageStore") <| @(t : T?) {
        var words <- storimg_words()
        let fmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(fmodel.ok && fmodel.value == uint(SpvExecutionModel.GLCompute),
            "storimg: EntryPoint model is GLCompute")
        t |> success(has_op(words, SpvOp.TypeImage), "storimg: emits OpTypeImage")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ms, sampled, format].
        // sampled=2 (read-write) at index 6, format=Rgba8 (known, no without-format cap) at index 7.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 2u),
            "storimg: OpTypeImage sampled=2 (read-write storage image)")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 7, uint(SpvImageFormat.Rgba8)),
            "storimg: OpTypeImage format is Rgba8 (known format)")
        // UniformConstant-storage OpVariable (the storage-image descriptor) at operand index 2
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.UniformConstant)),
            "storimg: has a UniformConstant-storage OpVariable")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.DescriptorSet), 0u),
            "storimg: DescriptorSet 0")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u),
            "storimg: Binding 0")
        t |> success(has_op(words, SpvOp.ImageRead), "storimg: imageLoad -> OpImageRead")
        t |> success(has_op(words, SpvOp.ImageWrite), "storimg: imageStore -> OpImageWrite")
        validate(t, words, "storimg")
        delete words
    }
}

[test]
def test_image_size_query(t : T?) {
    t |> run("imageSize(storage image) -> OpImageQuerySize + ImageQuery capability") <| @(t : T?) {
        var words <- imgquery_words()
        t |> success(has_op(words, SpvOp.ImageQuerySize), "imgquery: imageSize -> OpImageQuerySize")
        // the no-LOD query: must NOT emit the LOD variant (that one is textureSize's, Sampled=1 images)
        t |> success(!has_op(words, SpvOp.ImageQuerySizeLod),
            "imgquery: storage-image query is the no-LOD OpImageQuerySize, not OpImageQuerySizeLod")
        // OpImageQuerySize takes (result_type, result_id, image) -- the image operand is the loaded
        // storage image, so an OpLoad of the UniformConstant image must precede it
        t |> success(has_op(words, SpvOp.Load), "imgquery: loads the storage image before the query")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ImageQuery)),
            "imgquery: imageSize pulls in OpCapability ImageQuery")
        validate(t, words, "imgquery")
        delete words
    }
}

[test]
def test_module_scope_const(t : T?) {
    t |> run("module-scope `let` constants fold to OpConstant / OpConstantComposite") <| @(t : T?) {
        var words <- modconst_words()
        // Compilation succeeding at all is the fold proof: a non-folded module-scope const falls through
        // to the fail-closed `unsupported global` error (see test_fail_closed.das for the `var` rejection).
        // Here we pin the positive structure -- the folded scalar consts become OpConstant, the const-
        // vector becomes OpConstantComposite, referenced straight by id (no OpVariable / OpLoad).
        t |> success(has_op(words, SpvOp.Constant), "modconst: scalar module consts -> OpConstant")
        t |> success(has_op(words, SpvOp.ConstantComposite),
            "modconst: const-vector module const -> OpConstantComposite")
        validate(t, words, "modconst")
        delete words
    }
}
