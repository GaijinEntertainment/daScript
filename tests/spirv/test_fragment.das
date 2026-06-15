options gen2
options indenting = 4

// Phase-10.3 gate: fragment realism. discard() -> OpKill (a block terminator); screen-space
// derivatives dFdx/dFdy/fwidth -> OpDPdx/OpDPdy/OpFwidth; textureSize -> OpImageQuerySizeLod (which
// pulls in the ImageQuery capability); and a gl_FragDepth write -> BuiltIn FragDepth Output + the
// DepthReplacing execution mode. Each opcode/decoration is asserted positionally, and the module is
// spirv-val-clean -- the real check that OpKill terminates its block and the query/derivative results
// are well-typed.

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
def test_fragment_realism(t : T?) {
    t |> run("fragment realism: discard/derivatives/textureSize/gl_FragDepth") <| @(t : T?) {
        var words <- fragx_words()
        // a fragment entry point
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragx: EntryPoint model is Fragment")
        // discard() -> OpKill (the fragment-abandon terminator)
        t |> success(has_op(words, SpvOp.Kill), "fragx: discard() -> OpKill")
        // screen-space derivatives over the 2x2 quad
        t |> success(has_op(words, SpvOp.DPdx), "fragx: dFdx -> OpDPdx")
        t |> success(has_op(words, SpvOp.DPdy), "fragx: dFdy -> OpDPdy")
        t |> success(has_op(words, SpvOp.Fwidth), "fragx: fwidth -> OpFwidth")
        // textureSize -> OpImageQuerySizeLod + the ImageQuery capability it requires
        t |> success(has_op(words, SpvOp.ImageQuerySizeLod), "fragx: textureSize -> OpImageQuerySizeLod")
        t |> success(op_has_operand_at(words, SpvOp.Capability, 0, uint(SpvCapability.ImageQuery)),
            "fragx: textureSize pulls in OpCapability ImageQuery")
        // gl_FragDepth write -> BuiltIn FragDepth decoration + the DepthReplacing execution mode
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.FragDepth)),
            "fragx: gl_FragDepth -> BuiltIn FragDepth")
        t |> success(op_has_operand_at(words, SpvOp.ExecutionMode, 1, uint(SpvExecutionMode.DepthReplacing)),
            "fragx: gl_FragDepth write -> DepthReplacing execution mode")
        validate(t, words, "fragx")
        delete words
    }
}
