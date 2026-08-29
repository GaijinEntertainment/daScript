options gen2
options indenting = 4

// The cm2 s-tile shapes: coopmatWgB_f16_64x32, coopmatWgAcc_f16_128x32 and coopmatWgAcc_f32_128x32 -
// the MoE expert-bucket geometry - parse out of their names into OpTypeCooperativeMatrixKHR with the
// declared rows and columns, and the kernel that multiplies through them validates.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2s_words
require spirv/spirv_dis             // count_op, op_has_operand
require spirv/spirv_grammar         // SpvOp, SpvCapability

[test]
def test_coopmat2_s_tiles(t : T?) {
    t |> run("cm2 s-tile shapes: B 64x32, acc f16 128x32, acc f32 128x32 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2s_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        // A f16 128x64, B f16 64x32, acc f16 128x32, acc f32 128x32 - one type each
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 4,
            "four OpTypeCooperativeMatrixKHR (A, B, f16 acc, f32 acc)")
        // the rows and columns are OpConstant literals baked into the types, never runtime operands
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.FConvert) >= 1, "the f16 -> f32 accumulator convert emits OpFConvert")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}
