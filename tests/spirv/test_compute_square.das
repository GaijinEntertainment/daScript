options gen2
options indenting = 4

// End-to-end Phase-1 gate: the real `[compute_shader] square` lowered by the AST emitter,
// captured into `square_spv`. Asserts the exact opcode shape, then validates the blob with
// spirv-val (the structured-CFG / define-before-use oracle opcode-presence can't catch).

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

[test]
def test_square_opcodes(t : T?) {
    t |> run("compute square: opcode shape") <| @(t : T?) {
        var words <- square_words()
        t |> success(length(words) > 5, "has words")
        t |> equal(words[0], SPV_MAGIC)
        t |> equal(count_op(words, SpvOp.EntryPoint), 1)
        t |> equal(count_op(words, SpvOp.ExecutionMode), 1)
        t |> equal(count_op(words, SpvOp.Variable), 2)        // gid (Input) + data (StorageBuffer)
        t |> equal(count_op(words, SpvOp.AccessChain), 2)     // gid.x + data[i]
        t |> equal(count_op(words, SpvOp.Load), 1)            // load gid.x
        t |> equal(count_op(words, SpvOp.IMul), 1)            // i * i
        t |> equal(count_op(words, SpvOp.Store), 1)           // data[i] = ...
        t |> success(has_op(words, SpvOp.Return), "has Return")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.BuiltIn), uint(SpvBuiltIn.GlobalInvocationId)), "BuiltIn = GlobalInvocationId decoration")
        t |> success(op_has_operand_pair(words, SpvOp.ExecutionMode, uint(SpvExecutionMode.LocalSize), 64u), "ExecutionMode LocalSize x = 64")
        t |> success(op_has_operand_pair(words, SpvOp.Decorate, uint(SpvDecoration.Binding), 0u), "Binding = 0 decoration")
        delete words
    }
}

[test]
def test_square_validates(t : T?) {
    t |> run("compute square: spirv-val clean") <| @(t : T?) {
        var words <- square_words()
        let r = validate_spirv(words)
        if (r.ran) {
            t |> success(r.ok, "spirv-val: {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
