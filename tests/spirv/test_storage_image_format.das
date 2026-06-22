options gen2
options indenting = 4

// storage-image @format qualifier (Hi-Z depth-pyramid prereq): a bare image2D stays Rgba8 (back-compat
// default), while an @format = "r32f" image2D lowers to OpTypeImage format R32f. The two distinct
// formats produce two distinct OpTypeImage; imageLoad/imageStore are format-agnostic (the format lives
// on OpTypeImage), so the same ops serve both. spirv-val'd.

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
def test_storage_image_format(t : T?) {
    t |> run("storage-image @format: default image2D stays Rgba8, @format=r32f -> OpTypeImage R32f") <| @(t : T?) {
        var words <- imgfmt_words()
        // two storage images of different formats -> two distinct OpTypeImage (deduped on the format operand)
        t |> success(count_op(words, SpvOp.TypeImage) == 2,
            "imgfmt: two distinct-format storage images -> 2 OpTypeImage")
        // OpTypeImage operands: [result_id, sampled_type, dim, depth, arrayed, ms, sampled, format].
        // format at operand index 7: the @format=r32f image carries R32f; the bare one keeps Rgba8.
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 7, uint(SpvImageFormat.R32f)),
            "imgfmt: @format=r32f image2D -> OpTypeImage format R32f")
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 7, uint(SpvImageFormat.Rgba8)),
            "imgfmt: bare image2D stays Rgba8 (back-compat default)")
        // the format change leaves the read-write storage classification (sampled=2 at index 6) intact
        t |> success(op_has_operand_at(words, SpvOp.TypeImage, 6, 2u),
            "imgfmt: storage images stay sampled=2 (read-write)")
        t |> success(has_op(words, SpvOp.ImageRead), "imgfmt: imageLoad -> OpImageRead")
        t |> success(has_op(words, SpvOp.ImageWrite), "imgfmt: imageStore -> OpImageWrite")
        validate(t, words, "imgfmt")
        delete words
    }
}
