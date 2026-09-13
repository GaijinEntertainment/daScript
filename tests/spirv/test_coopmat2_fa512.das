options gen2
options indenting = 4

// The h512 flash-attention tiles: coopmatWgA_f16_64x512, coopmatWgB_f16_512x32, coopmatWgB_f16_32x512 and
// the 64x512 accumulators - f32 and the f16 O beside it - parse out of their names into
// OpTypeCooperativeMatrixKHR, the f16-accumulator MulAdd and the f16-to-f32 convert lower, and the
// online-softmax kernel that multiplies through them validates.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2fa512_words
require spirv/spirv_dis             // count_op, op_has_operand
require spirv/spirv_grammar         // SpvOp, SpvCapability

[test]
def test_coopmat2_fa512_tiles(t : T?) {
    t |> run("cm2 h512 flash tiles: A 64x512, B 512x32, B 32x512, acc 64x512 in f32 and f16 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2fa512_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        // acc f32 64x512, A f16 64x512, acc f32 64x32, B f16 512x32, A f16 64x32, B f16 32x32, B f16 32x512, acc f16 64x512 - one type each
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 8,
            "eight OpTypeCooperativeMatrixKHR (the h512 loop's shapes, the f16 O beside the f32)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 4, "four OpCooperativeMatrixMulAddKHR (S, rowsum, O in f32, O in f16)")
        t |> success(count_op(words, SpvOp.FConvert) >= 2, "the f16 O widens to f32 through OpFConvert beside the Q narrow")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "SPIR-V header version 1.6")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}
