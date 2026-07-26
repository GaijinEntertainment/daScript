options gen2
options indenting = 4

// Cooperative-matrix v2 gate (SPV_NV_cooperative_matrix2 + SPV_NV_tensor_addressing). The
// coopmat2_kernel fixture is the q8_0 decode-in-load GEMM shape: workgroup-scope tiles, tensor
// layout/view objects, per-K-tile OpTensorLayoutSliceNV, an A load through a [spirv_decode]
// function (a real OpFunction taking a PhysicalStorageBuffer block pointer — Aliased-decorated,
// Aligned loads), a B load through the (1,0) transpose view, the ±65504 f16 clamp loop, and a
// tensor store. Pins the NV capability set, the PhysicalStorageBuffer64 addressing model, and the
// exact operand shapes; spirv-val (vulkan1.3) is the independent witness the whole shape is valid.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2_words
require spirv/spirv_dis             // count_op, op_has_operand, count_op_operand_at
require spirv/spirv_grammar         // SpvOp, SpvCapability, SpvAddressingModel, SPV_VERSION_1_6

[test]
def test_coopmat2(t : T?) {
    t |> run("cm2 q8 decode-in-load GEMM: tensor addressing + decode function + wg-scope tiles") <| @(t : T?) {
        var words <- coopmat2_words()
        // the glslang cm2 capability set + both extensions
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR)),
            "OpCapability CooperativeMatrixKHR")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.PhysicalStorageBufferAddresses)),
            "OpCapability PhysicalStorageBufferAddresses")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV)),
            "OpCapability CooperativeMatrixTensorAddressingNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixBlockLoadsNV)),
            "OpCapability CooperativeMatrixBlockLoadsNV")
        t |> success(op_has_operand(words, SpvOp.Capability, uint(SpvCapability.TensorAddressingNV)),
            "OpCapability TensorAddressingNV")
        // PhysicalStorageBuffer64 Vulkan memory model + the 1.6 floor
        t |> success(op_has_operand_at(words, SpvOp.MemoryModel, 0, uint(SpvAddressingModel.PhysicalStorageBuffer64)),
            "OpMemoryModel PhysicalStorageBuffer64")
        t |> success(length(words) >= 2 && words[1] == SPV_VERSION_1_6,
            "SPIR-V header version floored to 1.6")
        // tensor objects: 2 layout types (clamp Constant + clamp Undefined), 1 transpose view type,
        // 3 created layouts + 1 created view, the 3 setters, and one slice per load/store
        t |> success(count_op(words, SpvOp.TypeTensorLayoutNV) == 2,
            "two OpTypeTensorLayoutNV (zero-pad inputs, undefined-clamp store)")
        t |> success(count_op(words, SpvOp.TypeTensorViewNV) == 1,
            "one OpTypeTensorViewNV (the transpose view)")
        t |> success(count_op(words, SpvOp.CreateTensorLayoutNV) == 3,
            "three OpCreateTensorLayoutNV (A, B, out)")
        t |> success(count_op(words, SpvOp.CreateTensorViewNV) == 1,
            "one OpCreateTensorViewNV")
        t |> success(count_op(words, SpvOp.TensorLayoutSetDimensionNV) == 3,
            "three OpTensorLayoutSetDimensionNV")
        t |> success(count_op(words, SpvOp.TensorLayoutSetStrideNV) == 3,
            "three OpTensorLayoutSetStrideNV")
        t |> success(count_op(words, SpvOp.TensorLayoutSetBlockSizeNV) == 1,
            "one OpTensorLayoutSetBlockSizeNV (the 1x32 q8_0 block)")
        t |> success(count_op(words, SpvOp.TensorLayoutSliceNV) == 3,
            "three OpTensorLayoutSliceNV (A load, B load, store)")
        // tensor-addressed loads/stores + the wg-scope MMA
        t |> success(count_op(words, SpvOp.CooperativeMatrixLoadTensorNV) == 2,
            "two OpCooperativeMatrixLoadTensorNV (DecodeFunc A, TensorView B)")
        t |> success(count_op(words, SpvOp.CooperativeMatrixStoreTensorNV) == 1,
            "one OpCooperativeMatrixStoreTensorNV")
        t |> success(count_op(words, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR")
        t |> success(count_op(words, SpvOp.FConvert) >= 1,
            "OpFConvert widens the f16 accumulator for the f32 store")
        // the clamp loop: Length + the emitter's only OpPhi + FClamp through GLSL.std.450
        t |> success(count_op(words, SpvOp.CooperativeMatrixLengthKHR) == 1,
            "one OpCooperativeMatrixLengthKHR")
        t |> success(count_op(words, SpvOp.Phi) == 1,
            "one OpPhi (the coopmatClamp element loop)")
        // the decode function: a second OpFunction with exactly 3 OpFunctionParameters, its
        // PhysicalStorageBuffer pointer decorated Aliased, and Aligned loads inside
        t |> success(count_op(words, SpvOp.Function) == 2,
            "two OpFunction (main + the decode function)")
        t |> success(count_op(words, SpvOp.FunctionParameter) == 3,
            "three OpFunctionParameter (block pointer + two uint[2] coords)")
        t |> success(count_op_operand_at(words, SpvOp.Decorate, 1, uint(SpvDecoration.Aliased)) == 1,
            "the decode function's PSB pointer parameter is decorated Aliased")
        t |> success(count_op_operand_at(words, SpvOp.Load, 3, uint(SpvMemoryAccess.Aligned)) >= 2,
            "the decode function's PSB loads carry the Aligned memory operand")
        let r = validate_spirv(words, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
        delete words
    }
}
