options gen2
options indenting = 4

// The cm2 s-tile shapes - the MoE expert-bucket geometry at both k steps the s stamps run: A 128x32 with
// B 32x32 (the grid-codebook formats' stamps) and A 128x64 with B 64x32 (every other format's), the 128x32
// accumulators on both - emit as OpTypeCooperativeMatrixKHR and validate.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2s_words, coopmat2s64_words
require spirv/spirv_dis             // count_op, op_has_operand
require spirv/spirv_grammar         // SpvOp, SpvCapability

def private check_s_tiles(t : T?; var words : array<uint>; depth : int) {
    t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "SPIR-V header version 1.6 ({depth}-deep)")
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
        "OpCapability CooperativeMatrixKHR ({depth}-deep)")
    // A f16 128xdepth, B f16 depthx32, acc f16 128x32, acc f32 128x32 - one type each
    t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 4,
        "four OpTypeCooperativeMatrixKHR (A, B, f16 acc, f32 acc) ({depth}-deep)")
    // the rows and columns are OpConstant literals baked into the types, never runtime operands
    t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "one OpCooperativeMatrixMulAddKHR ({depth}-deep)")
    t |> success(count_op(words, SpvOp.FConvert) >= 1, "the f16 -> f32 accumulator convert emits OpFConvert ({depth}-deep)")
    let r = validate_spirv(words, "vulkan1.3")
    if (r.ran) {
        t |> success(r.ok, "spirv-val (vulkan1.3, {depth}-deep): {r.msg}")
    }
    delete words
}

[test]
def test_coopmat2_s_tiles(t : T?) {
    t |> run("cm2 s-tile shapes: A 128x32, B 32x32, acc f16 128x32, acc f32 128x32 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2s_words()
        check_s_tiles(t, words, 32)
    }
    t |> run("cm2 s-tile shapes at the 64-deep k step: A 128x64, B 64x32, the same accumulators") <| @(t : T?) {
        var words <- coopmat2s64_words()
        check_s_tiles(t, words, 64)
    }
}
