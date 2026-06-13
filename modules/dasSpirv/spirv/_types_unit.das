options gen2

// PHASE-1 unit scaffold: exercise spirv_types.emit_type — correct type instructions + dedup.

require spirv_builder
require spirv_types
require spirv_dis
require spirv_grammar
require daslib/ast

[export]
def main {
    var ok = true
    ast_gc_guard() {
        var m = make_module()
        let u   = emit_type(m, new TypeDecl(baseType = Type.tUInt))
        let u3  = emit_type(m, new TypeDecl(baseType = Type.tUInt3))
        let f   = emit_type(m, new TypeDecl(baseType = Type.tFloat))
        let f4  = emit_type(m, new TypeDecl(baseType = Type.tFloat4))
        let u_again = emit_type(m, new TypeDecl(baseType = Type.tUInt))

        ok = ok && u == u_again                 // scalar dedup
        ok = ok && u != f                        // distinct scalars
        ok = ok && u3 != f4                       // distinct vector types
        var words <- module_words(m)
        ok = ok && count_op(words, SpvOp.TypeInt) == 1     // uint emitted once (u, u3 comp, u_again)
        ok = ok && count_op(words, SpvOp.TypeFloat) == 1   // float once (f, f4 comp)
        ok = ok && count_op(words, SpvOp.TypeVector) == 2   // uint3 + float4
        print("{dump(words)}")
        delete words
        delete m
    }
    print("---\nspirv_types emit_type: {ok ? "PASS" : "FAIL"}\n")
}

def u4_or(x : uint) : uint {
    return x
}
