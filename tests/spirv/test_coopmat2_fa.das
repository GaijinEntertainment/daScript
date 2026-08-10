options gen2
options indenting = 4

// Cooperative-matrix v2 flash-attention rail (SPV_NV_cooperative_matrix2 reductions / conversions /
// per-element ops). The coopmat2fa_kernel fixture is llama.cpp's flash_attn_cm2 inner loop: online
// softmax over KV tiles with OpCooperativeMatrixReduceNV (row reduce + the column-RESIZING smear),
// OpCooperativeMatrixPerElementOpNV (no-extra exp, a matrix extra for the running max, scalar extras
// for the on-the-fly causal mask), use-changing OpFConvert (f32 accumulator -> f16 A — the
// ConversionsNV shape glslang emits for coopmat<f16,...,A>(acc)), componentwise tile arithmetic,
// OpMatrixTimesScalar, and single-scalar OpCompositeConstruct fills. Callback functions are BY-VALUE
// scalar OpFunctions — the operand shapes are pinned against llama.cpp's compiled
// flash_attn_f32_f16_cm2.spv; spirv-val (vulkan1.3) is the independent witness.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2fa_words
require spirv/spirv_dis             // count_op, op_has_operand, count_op_operand_at
require spirv/spirv_grammar         // SpvOp, SpvCapability, SPV_VERSION_1_6

[test]
def test_coopmat2_fa(t : T?) {
    t |> run("cm2 flash-attention: reduce + per-element callbacks, use conversion, tile algebra") <| @(t : T?) {
        var words <- coopmat2fa_words()
        // the three opt-in cm2 capabilities ride the base cm2 set
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixReductionsNV)),
            "OpCapability CooperativeMatrixReductionsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixPerElementOperationsNV)),
            "OpCapability CooperativeMatrixPerElementOperationsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixConversionsNV)),
            "OpCapability CooperativeMatrixConversionsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV)),
            "OpCapability CooperativeMatrixTensorAddressingNV")
        // reduces: rowmax + the two smears, all Row (mask literal 1), each with a combine-fn id
        t |> success(count_op(words, SpvOp.CooperativeMatrixReduceNV) == 3,
            "three OpCooperativeMatrixReduceNV (rowmax + eM smear + L smear)")
        t |> success(count_op_operand_at(words, SpvOp.CooperativeMatrixReduceNV, 3, 1u) == 3,
            "every reduce carries the Row mask literal")
        // per-element ops: mask (2 scalar extras), pick_max (1 matrix extra), exp x2, recip
        t |> success(count_op(words, SpvOp.CooperativeMatrixPerElementOpNV) == 5,
            "five OpCooperativeMatrixPerElementOpNV (mask, running max, exp x2, recip)")
        // callbacks: main + 6 by-value scalar OpFunctions (max, smear, exp, pick_max, mask, recip)
        t |> success(count_op(words, SpvOp.Function) == 7,
            "seven OpFunction (main + six callbacks)")
        t |> success(count_op(words, SpvOp.FunctionParameter) == 2 + 2 + 3 + 4 + 5 + 3,
            "19 by-value OpFunctionParameter across the six callbacks")
        // tile algebra: 3 MulAdds (S, rowsum, O), componentwise mul x3 / add / sub x2, Q pre-scale
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 3,
            "three OpCooperativeMatrixMulAddKHR (Q@K^T, P@One, P@V)")
        t |> success(count_op(words, SpvOp.MatrixTimesScalar) == 1,
            "one OpMatrixTimesScalar (the Q pre-scale)")
        // the use-changing conversions: Q f32 ACC -> f16 A and P f32 ACC -> f16 A
        t |> success(count_op(words, SpvOp.FConvert) >= 2,
            "OpFConvert covers the two ACC -> A use conversions")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}
