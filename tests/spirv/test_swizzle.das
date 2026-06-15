options gen2
options indenting = 4

// Phase-10.2 gate: multi-component swizzle of a bare global (the Phase-8 out-of-scope note). A
// single-component global swizzle stays an lvalue (OpAccessChain to the component); a multi-component
// one loads the whole global and OpVectorShuffle's the lanes. Asserts both opcodes are present (shuffle
// for .xy/.xyz/.yx, access-chain for the single-component .w) and that the module is spirv-val-clean --
// the real check that the loaded-then-shuffled lanes form a valid vector result.

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
def test_global_swizzle(t : T?) {
    t |> run("swizzle: multi-component global -> load + OpVectorShuffle; single-component -> OpAccessChain") <| @(t : T?) {
        var words <- gswizzle_words()
        // a fragment entry point
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "gswizzle: EntryPoint model is Fragment")
        // multi-component global swizzles (gl_FragCoord.xy, sw_in.xyz, sw_in.yx) -> OpVectorShuffle
        t |> success(has_op(words, SpvOp.VectorShuffle), "gswizzle: multi-component swizzle -> OpVectorShuffle")
        // the single-component global swizzle (sw_in.w) -> OpAccessChain to the component
        t |> success(has_op(words, SpvOp.AccessChain), "gswizzle: single-component global swizzle -> OpAccessChain")
        // and the loaded lanes feed a float4 constructor
        t |> success(has_op(words, SpvOp.CompositeConstruct), "gswizzle: result via OpCompositeConstruct")
        validate(t, words, "gswizzle")
        delete words
    }
}
