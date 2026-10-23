options gen2
options indenting = 4

// The cm2 s-tile shapes - the MoE expert-bucket geometry at both k
// steps the s stamps run - and the int8 workgroup tiles with their
// scaled s32 -> f32 fold emit as OpTypeCooperativeMatrixKHR with the
// rows and columns the kernels declare, and validate.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_dis
require spirv/spirv_grammar

//! the shape set is the claim: a fixture's four tile types at exactly these rows x cols, one MulAdd, the f16 -> f32 convert
def private check_cm2_tiles(t : T?; words : array<uint>; lbl : string; want : array<string>) {
    t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6, "{lbl}: SPIR-V header version 1.6")
    t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)), "{lbl}: OpCapability CooperativeMatrixKHR")
    var got <- spv_coopmat_shapes(words)
    t |> equal(want, got, "{lbl}: the sorted OpTypeCooperativeMatrixKHR shape set - the two operand tiles and the f16 and f32 accumulators")
    delete got
    t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "{lbl}: one OpCooperativeMatrixMulAddKHR")
    t |> success(count_op(words, SpvOp.FConvert) >= 1, "{lbl}: the f16 -> f32 accumulator convert emits OpFConvert")
    let r = validate_spirv(words, "vulkan1.3")
    if (r.ran) {
        t |> success(r.ok, "{lbl}: spirv-val (vulkan1.3): {r.msg}")
    }
}

[test]
def test_coopmat2_s_tiles(t : T?) {
    t |> run("cm2 s-stamp shapes at the 32-deep k step: A 128x32, B 32x32, acc f16 128x32, acc f32 128x32 through the name-parsed marker rail") <| @(t : T?) {
        var words <- coopmat2s_words()
        var want <- ["128x32", "128x32", "128x32", "32x32"]
        check_cm2_tiles(t, words, "s 32-deep", want)
        delete want
        delete words
    }
    t |> run("cm2 s-stamp shapes at the 64-deep k step: A 128x64, B 64x32, the same accumulators") <| @(t : T?) {
        var words <- coopmat2s64_words()
        var want <- ["128x32", "128x32", "128x64", "64x32"]
        check_cm2_tiles(t, words, "s 64-deep", want)
        delete want
        delete words
    }
}

[test]
def test_coopmat2_i8_tiles(t : T?) {
    t |> run("the int8 workgroup tiles: A s8 64x32, B s8 32x64, the s32 and f32 64x64 " +
            "accumulators, the s32 tile converted and folded under the per-element " +
            "function the emitter synthesizes over the shared scale arrays") <| @(t : T?) {
        var words <- coopmat2i8_words()
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "i8: SPIR-V header version 1.6")
        for (cap in [SpvCapability.CooperativeMatrixKHR, SpvCapability.StorageBuffer8BitAccess,
                SpvCapability.CooperativeMatrixTensorAddressingNV,
                SpvCapability.CooperativeMatrixPerElementOperationsNV]) {
            t |> success(op_has_operand(words, SpvOp.Capability, uint(cap)),
                "i8: OpCapability {cap}")
        }
        var got <- spv_coopmat_shapes(words)
        var want <- ["32x64", "64x32", "64x64", "64x64"]
        t |> equal(want, got, "i8: the sorted OpTypeCooperativeMatrixKHR shape set - " +
            "the two int8 operand tiles and the two accumulators")
        delete got
        delete want
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "i8: one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.ConvertSToF) == 1,
            "i8: the s32 tile converts once (OpConvertSToF)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixPerElementOpNV) == 1,
            "i8: one OpCooperativeMatrixPerElementOpNV")
        t |> success(count_op(words, SpvOp.FConvert) == 0, "i8: no float width convert")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "i8: spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (no CI lane runs spirv-val)\n")
        }
        delete words
    }
}
