options gen2
options indenting = 4

// The cm2 e-column shapes - the expert schedule's m column at the 32-deep k step - emit as
// OpTypeCooperativeMatrixKHR and validate.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

[test]
def test_coopmat2_e_tiles(t : T?) {
    t |> run("cm2 e-column shapes: A 128x32, B 32x128, acc f16 128x128, acc f32 128x128 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2e_words()
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "SPIR-V header version 1.6")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 4,
            "four OpTypeCooperativeMatrixKHR (A, B, f16 acc, f32 acc)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.FConvert) >= 1, "the f16 -> f32 accumulator convert emits OpFConvert")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}
