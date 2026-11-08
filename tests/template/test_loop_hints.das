options gen2
options indenting = 4
options rtti

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require dastest/testing_boost public

// The append_vectorize / append_unroll helpers attach per-loop hints to a
// quoted `for` node by mutating its AnnotationArgumentList directly (there is
// no `$v` splice point for annotations). Each entry must be byte-identical to
// what the parser emits, so the JIT's append_loop_hint_operand reads it the
// same whether the loop was hand-written or macro-tuned.

[test(description = "append_vectorize with a width adds the vectorize and vectorize_width annotations to a loop")]
def test_vectorize_width(t : T?) {
    ast_gc_guard() {
        var loop = qmacro_expr(${ for (j in range64(4l)) { let x = j; } }) as ExprFor
        loop |> append_vectorize(8)
        t |> equal(loop.annotations |> length, 2, "vectorize + vectorize_width")
        t |> equal(string(loop.annotations[0].name), "vectorize")
        t |> equal(loop.annotations[0].basicType, Type.tBool)      // parser-identical flag
        t |> equal(loop.annotations[0].bValue, true)
        t |> equal(string(loop.annotations[1].name), "vectorize_width")
        t |> equal(loop.annotations[1].basicType, Type.tInt)
        t |> equal(loop.annotations[1].iValue, 8)
    }
}

[test(description = "append_unroll with a count adds one unroll annotation to a loop")]
def test_unroll_count(t : T?) {
    ast_gc_guard() {
        var loop = qmacro_expr(${ for (j in range64(4l)) { let x = j; } }) as ExprFor
        loop |> append_unroll(2)
        t |> equal(loop.annotations |> length, 1)
        t |> equal(string(loop.annotations[0].name), "unroll_count")
        t |> equal(loop.annotations[0].basicType, Type.tInt)
        t |> equal(loop.annotations[0].iValue, 2)
    }
}

[test(description = "append_vectorize and append_unroll without arguments add only their flag annotations")]
def test_bare_flags(t : T?) {
    ast_gc_guard() {
        var loop = qmacro_expr(${ for (j in range64(4l)) { let x = j; } }) as ExprFor
        loop |> append_vectorize()   // width 0 -> [vectorize] flag only
        loop |> append_unroll()      // count 0 -> [unroll] flag
        t |> equal(loop.annotations |> length, 2)
        t |> equal(string(loop.annotations[0].name), "vectorize")
        t |> equal(string(loop.annotations[1].name), "unroll")
        t |> equal(loop.annotations[1].basicType, Type.tBool)
    }
}
