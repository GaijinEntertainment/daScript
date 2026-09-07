options gen2
options indenting = 4

// The h256 flash-attention tiles: coopmatWgA_f16_64x256, coopmatWgB_f16_256x32, coopmatWgB_f16_32x256
// and the 64x256 accumulators parse out of their names into OpTypeCooperativeMatrixKHR with the
// declared rows and columns, and the online-softmax kernel that multiplies through them validates.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2fa256_words
require spirv/spirv_dis             // count_op, op_has_operand
require spirv/spirv_grammar         // SpvOp, SpvCapability

[test]
def test_coopmat2_fa256_tiles(t : T?) {
    t |> run("cm2 h256 flash tiles: A 64x256, B 256x32, B 32x256, acc 64x256 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2fa256_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        // acc f32 64x256, A f16 64x256, acc f32 64x32, B f16 256x32, A f16 64x32, B f16 32x32, B f16 32x256 - one type each
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 7,
            "seven OpTypeCooperativeMatrixKHR (the h256 loop's shapes)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 3, "three OpCooperativeMatrixMulAddKHR (S, rowsum, O)")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}
