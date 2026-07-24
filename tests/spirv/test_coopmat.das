options gen2
options indenting = 4

// Cooperative-matrix gate (SPV_KHR_cooperative_matrix — Tensor-Core MMA). The coopmat_kernel fixture is
// an f16 x f16 -> f32 GEMM tile: it loads a 16x16 A (row-major) and B (column-major) from f16 SSBOs into
// subgroup tiles, does one OpCooperativeMatrixMulAddKHR into a zero-initialized (OpConstantNull) f32
// accumulator, and stores the 16x16 result column-major. This pins the four KHR coopmat opcodes, the
// CooperativeMatrixKHR capability + SPV_KHR_cooperative_matrix extension, and the SPIR-V 1.6 version
// floor; spirv-val (vulkan1.3) is the independent witness that the whole shape is valid.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat_words
require spirv/spirv_dis             // count_op, op_has_operand
require spirv/spirv_grammar         // SpvOp, SpvCapability, SPV_VERSION_1_6

[test]
def test_coopmat(t : T?) {
    t |> run("cooperative matrix f16xf16->f32 GEMM tile: KHR coopmat opcodes + capability + 1.6 floor") <| @(t : T?) {
        var words <- coopmat_words()
        // capability + extension + the SPIR-V 1.6 version floor coopmat forces
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR (6022)")
        t |> success(count_op(words, SpvOp.Extension) >= 1,
            "an OpExtension is declared (SPV_KHR_cooperative_matrix)")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "SPIR-V header version floored to 1.6")
        // three distinct tile types (A f16/use0, B f16/use1, accumulator f32/use2), two loads, one
        // multiply-accumulate, one store
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 3,
            "three OpTypeCooperativeMatrixKHR (A, B, accumulator)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadKHR) == 2,
            "two OpCooperativeMatrixLoadKHR (A, B tiles)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreKHR) == 1,
            "one OpCooperativeMatrixStoreKHR")
        // the accumulator is zero-initialized (a bare `var acc : coopmatAcc...`) -> OpConstantNull
        t |> success(count_op(words, SpvOp.ConstantNull) >= 1,
            "OpConstantNull zero-inits the accumulator tile")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("cooperative matrix INT8 tile: s8xs8->s32 (16x16x32) + the signed-components MulAdd mask") <| @(t : T?) {
        var words <- coopmat_i8_words()
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR (6022)")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.StorageBuffer8BitAccess)),
            "OpCapability StorageBuffer8BitAccess (the int8 SSBO tiles)")
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 3,
            "three OpTypeCooperativeMatrixKHR (A s8 16x32, B s8 32x16, accumulator s32 16x16)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadKHR) == 2, "two loads")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1, "one MulAdd")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreKHR) == 1, "one store")
        // the integer MulAdd carries the CooperativeMatrixOperands mask at operand index 5:
        // A|B|C|Result signed = 0x1|0x2|0x4|0x8 = 0xF (float MulAdd omits it)
        t |> success(count_op_operand_at(words, SpvOp.CooperativeMatrixMulAddKHR, 5, 0xFu) == 1,
            "the MulAdd declares all operands signed (mask 0xF)")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("cooperative matrix f16 ACCUMULATOR + coopmatConvert widening (the mul_mm f16acc form)") <| @(t : T?) {
        var words <- coopmat_f16acc_words()
        t |> success(count_op(words, SpvOp.TypeCooperativeMatrixKHR) == 4,
            "four OpTypeCooperativeMatrixKHR (A, B, f16 accumulator, f32 widened)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR (f16 accumulate, no operands mask)")
        t |> success(count_op(words, SpvOp.FConvert) == 1,
            "one OpFConvert — the componentwise f16->f32 tile widening")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreKHR) == 1,
            "one OpCooperativeMatrixStoreKHR (the f32 tile)")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
    t |> run("cooperative matrix from @workgroup shared memory (the tiled-GEMM staging path)") <| @(t : T?) {
        var words <- coopmat_sm_words()
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadKHR) == 2, "two coopmat loads (from shared)")
        // the shared tiles are Workgroup-storage OpVariables; the loads chain a Workgroup pointer
        t |> success(op_has_operand(words, SpvOp.Variable, uint(SpvStorageClass.Workgroup)),
            "a @workgroup (Workgroup-storage) OpVariable is declared")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        } else {
            feint("spirv-val not found locally; skipping (CI enforces)\n")
        }
        delete words
    }
}
