options gen2
options indenting = 4

// Phase-4b matrix gate: matrices (OpTypeMatrix) carried in a UBO with std140 ColMajor + MatrixStride
// decorations, the linear-algebra products (OpMatrixTimesMatrix / OpMatrixTimesVector /
// OpVectorTimesScalar), and local-value swizzles (OpVectorShuffle / OpCompositeExtract). daslang
// matrices are column-major (m[i] is column i), matching SPIR-V's default — verified clean by spirv-val.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

def private check_ops(t : T?; words : array<uint>; lbl : string; ops : array<SpvOp>) {
    for (op in ops) {
        t |> success(has_op(words, op), "{lbl}: emits {op}")
    }
}

def private validate(t : T?; words : array<uint>; lbl : string) {
    let r = validate_spirv(words)
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val: {r.msg}")
    } else {
        feint("spirv-val not found locally; skipping (CI enforces)\n")
    }
}

[test]
def test_mvp_vertex(t : T?) {
    t |> run("vertex: UBO mat4x4 MVP -> MatrixTimesMatrix/Vector + VectorTimesScalar + local swizzle") <| @(t : T?) {
        var words <- mvp_vert_words()
        // a vertex entry point feeding gl_Position
        let vmodel = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(vmodel.ok && vmodel.value == uint(SpvExecutionModel.Vertex),
            "mvp_vert: EntryPoint model is Vertex")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.Position)),
            "mvp_vert: gl_Position -> BuiltIn Position")
        // the UBO carrying the two matrices: Uniform OpVariable + Block + descriptor + std140
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Uniform)),
            "mvp_vert: Uniform-storage OpVariable (the camera UBO)")
        t |> success(op_has_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Block)),
            "mvp_vert: camera struct is Block-decorated")
        // matrix std140: proj@0, view@64 (each mat4 is 16*4 = 64 bytes)
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), 0u),
            "mvp_vert: proj @ std140 offset 0")
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.Offset), 64u),
            "mvp_vert: view @ std140 offset 64")
        // matrix members carry ColMajor + MatrixStride 16 (daslang column-major storage)
        t |> success(op_has_operand_at(words, SpvOp.MemberDecorate, 2, uint(SpvDecoration.ColMajor)),
            "mvp_vert: matrix member is ColMajor")
        t |> success(op_has_operand_pair(words, SpvOp.MemberDecorate, uint(SpvDecoration.MatrixStride), 16u),
            "mvp_vert: matrix member has MatrixStride 16")
        // the matrix type itself + the linear-algebra products + local swizzles
        check_ops(t, words, "mvp_vert", [SpvOp.TypeMatrix, SpvOp.MatrixTimesMatrix, SpvOp.MatrixTimesVector,
            SpvOp.VectorTimesScalar, SpvOp.VectorShuffle, SpvOp.CompositeExtract, SpvOp.CompositeConstruct])
        validate(t, words, "mvp_vert")
        delete words
    }
}
