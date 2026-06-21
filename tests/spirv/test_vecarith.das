options gen2
options indenting = 4

// Phase-8.2 vector arithmetic + folded const-vector operand. Component-wise +/-/ lower to
// OpFAdd/OpFSub/OpFDiv on the vector type; unary - to OpFNegate; a float2(...) literal operand folds
// to OpConstantComposite. (This is the exact shape Phase 7 deferred.) The blob is spirv-val'd.

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
def test_vector_arithmetic(t : T?) {
    t |> run("vector arithmetic: component-wise +/-/ + unary - + folded const operand") <| @(t : T?) {
        var words <- vecarith_words()
        t |> success(has_op(words, SpvOp.FAdd), "vecarith: va + float2(..) -> component-wise OpFAdd")
        t |> success(has_op(words, SpvOp.FSub), "vecarith: a - b -> component-wise OpFSub")
        t |> success(has_op(words, SpvOp.FDiv), "vecarith: c / b -> component-wise OpFDiv")
        t |> success(has_op(words, SpvOp.FNegate), "vecarith: -d -> component-wise OpFNegate")
        t |> success(has_op(words, SpvOp.ConstantComposite), "vecarith: float2(0.5,0.5) -> OpConstantComposite")
        validate(t, words, "vecarith")
        delete words
    }
}

[test]
def test_vector_times_scalar(t : T?) {
    t |> run("vec *= scalar -> OpVectorTimesScalar (const + runtime), not a shape-mismatched OpFMul") <| @(t : T?) {
        var words <- vts_words()
        // Both `v *= 0.5` (const scalar -- the historical crash) and `w *= vts_in.x` (runtime scalar)
        // must lower to OpVectorTimesScalar; a component-wise OpFMul(vec, scalar) is illegal SPIR-V.
        t |> success(has_op(words, SpvOp.VectorTimesScalar), "vts: vec *= scalar -> OpVectorTimesScalar")
        let nvts = count_op(words, SpvOp.VectorTimesScalar)
        t |> success(nvts == 2, "vts: two VectorTimesScalar (const + runtime scalar) (got {nvts})")
        validate(t, words, "vts")
        delete words
    }
}
