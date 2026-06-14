options gen2
options indenting = 4

// Const arrays + dynamic indexing gate: the gl_VertexIndex hardcoded-triangle vertex shader lowers
// fixed-array `let`s to OpTypeArray + OpConstantComposite (vec + array constants), a Function-storage
// array OpVariable initialized via OpStore, and OpAccessChain indexing by gl_VertexIndex. The emitted
// module passes spirv-val (the real define-before-use / type oracle).

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
def test_const_array_triangle(t : T?) {
    t |> run("const arrays: positions[gl_VertexIndex] -> TypeArray + ConstantComposite + AccessChain") <| @(t : T?) {
        var words <- catri_words()
        // array type + composite constants + dynamic index + the vec4 build
        check_ops(t, words, "catri", [SpvOp.TypeArray, SpvOp.ConstantComposite,
            SpvOp.AccessChain, SpvOp.Load, SpvOp.Store, SpvOp.CompositeConstruct])
        // the length-3 array constant: an OpConstant uint with value 3 (operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Constant, 2, 3u), "catri: has the uint 3 length constant")
        // a Function-storage OpVariable for the array (storage class at operand index 2)
        t |> success(op_has_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Function)),
            "catri: array lives in a Function-storage OpVariable")
        // rasterizer builtins driving it
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.VertexIndex)),
            "catri: reads gl_VertexIndex")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.Position)),
            "catri: writes gl_Position")
        validate(t, words, "catri")
        delete words
    }
}
