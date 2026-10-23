options gen2
options indenting = 4

require dastest/testing_boost public
require _spirv_common
require _color_mapping_shader
require spirv/spirv_dis
require spirv/spirv_grammar

[test]
def test_color_mapping_spirv(t : T?) {
    t |> run("all color mapping helpers compile as a SPIR-V fragment shader") <| @(t : T?) {
        var words <- color_mapping_words()
        t |> equal(words[1], 0x00010300u, "SPIR-V 1.3")
        let model = op_first_operand(words, SpvOp.EntryPoint)
        t |> success(model.ok && model.value == uint(SpvExecutionModel.Fragment),
            "fragment entry point")
        t |> equal(count_op_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Input)), 1,
            "runtime color input")
        t |> equal(count_op_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Output)), 4,
            "four color outputs")
        foreach_inst(words) $(opcode : uint; w : array<uint>; start, count : int) {
            if (opcode == uint(SpvOp.Variable) && count >= 3 &&
                    w[start + 2] == uint(SpvStorageClass.Output)) {
                t |> success(op_has_operand_at(words, SpvOp.Store, 0, w[start + 1]),
                    "color output is written")
            }
        }
        let validation = validate_spirv(words, "vulkan1.1")
        if (validation.ran) {
            t |> success(validation.ok, "spirv-val: {validation.msg}")
        } else {
            feint("spirv-val not found locally; skipping external validation\n")
        }
        delete words
    }
}
