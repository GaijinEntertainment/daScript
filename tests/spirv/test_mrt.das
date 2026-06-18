options gen2
options indenting = 4

// Multiple Render Targets: lock in that the dasSpirv emitter accepts multiple `@out @location = N`
// globals in a fragment shader and emits each as its own Output OpVariable with a distinct Location
// decoration. This is the G-buffer shape tutorial 10 (deferred shading) will use; the per-global
// Location model is already what the emitter does, so this test exists to *guard* the behavior
// against a future regression -- not to add new functionality.

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
def test_mrt_three_outputs(t : T?) {
    t |> run("MRT: fragment shader with 3 @location outputs") <| @(t : T?) {
        var words <- mrt_words()
        // Entry point is a fragment shader.
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "mrt: EntryPoint model is Fragment")
        // Exactly 4 Location decorations: 1 @in (mrt_uv at 0) + 3 @out (position/normal/albedo at 0/1/2).
        let n_loc = count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Location))
        t |> success(n_loc == 4, "mrt: 4 Location decorations (1 input + 3 outputs), got {n_loc}")
        // Each of locations 0, 1, 2 is emitted. (Location 0 is emitted twice -- input + output -- and
        // op_has_operand_pair only checks for presence, not count.)
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 0u),
            "mrt: Location 0 decoration present")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 1u),
            "mrt: Location 1 decoration present")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Location), 2u),
            "mrt: Location 2 decoration present")
        // At least 4 OpVariable (1 in + 3 out -- there may be more for builtins / internals).
        let n_var = count_op(words, SpvOp.Variable)
        t |> success(n_var >= 4, "mrt: at least 4 OpVariable (1 input + 3 outputs), got {n_var}")
        // At least 3 OpStore -- one write per output global. The shader body has exactly 3 stores.
        let n_store = count_op(words, SpvOp.Store)
        t |> success(n_store >= 3, "mrt: at least 3 OpStore writes, got {n_store}")
        // spirv-val (CI gate) catches structural CFG / type errors the opcode-counting asserts can't.
        validate(t, words, "mrt")
        delete words
    }
}
