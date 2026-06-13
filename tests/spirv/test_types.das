options gen2
options indenting = 4

// Unit gate for spirv_types.emit_type: correct type instructions + dedup pool correctness
// (risk #1 — type/constant dedup). Scalars dedup to one id; distinct base types stay distinct.

require dastest/testing_boost public
require spirv/spirv_builder
require spirv/spirv_types
require spirv/spirv_dis
require spirv/spirv_grammar
require daslib/ast

[test]
def test_emit_type(t : T?) {
    t |> run("emit_type: type instructions + dedup") <| @(t : T?) {
        ast_gc_guard() {
            var m = make_module()
            let u  = emit_type(m, new TypeDecl(baseType = Type.tUInt))
            let u3 = emit_type(m, new TypeDecl(baseType = Type.tUInt3))
            let f  = emit_type(m, new TypeDecl(baseType = Type.tFloat))
            let f4 = emit_type(m, new TypeDecl(baseType = Type.tFloat4))
            let u2 = emit_type(m, new TypeDecl(baseType = Type.tUInt))
            t |> equal(u, u2)                          // scalar dedup
            t |> success(u != f, "distinct scalars")
            t |> success(u3 != f4, "distinct vectors")
            var words <- module_words(m)
            t |> equal(count_op(words, SpvOp.TypeInt), 1)      // uint once (u, u3 component, u2)
            t |> equal(count_op(words, SpvOp.TypeFloat), 1)    // float once (f, f4 component)
            t |> equal(count_op(words, SpvOp.TypeVector), 2)   // uint3 + float4
            delete words
            delete m
        }
    }
}
