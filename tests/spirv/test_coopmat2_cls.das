options gen2
options indenting = 4

// The cm2 decode-METHOD gate: [spirv_decode] on a kernel-class method, referenced as
// `self.<method>` from coopmatLoadTensorDecode. The das-level self is erased — the emitted decode
// function keeps the rigid 3-param SPV_NV_cooperative_matrix2 shape — while the body reads CLASS
// MEMBERS: a separate scale plane (the two-plane fmt-0 q8 layout: qs words + f16 scales), the push
// constants, and an @workgroup scalar staged by the kernel body. Also pins the l-tile GEMM shapes
// (A f16 128x64, B f16 64x256, acc 128x256 — llama.cpp's cm2 quant "l" geometry) through the
// name-parsed wg marker rail. spirv-val (vulkan1.3) is the independent witness.

require dastest/testing_boost public
require _spirv_common               // validate_spirv
require spirv/spirv_shader
require spirv/spirv_dis             // count_op, op_has_operand, count_op_operand_at
require spirv/spirv_grammar         // SpvOp, SpvDecoration, SpvMemoryAccess, SpvStorageClass

struct Cm2ClsArgs {
    d : uint
    n : uint
    blk0 : uint
}

// the two-plane fmt-0 block: 8 words = 32 packed int8 quants, 32 bytes — the scale is NOT here
struct Cm2BlockQ8P {
    qs : uint[8]
}

class Cm2DecodeCls {
    @ssbo @binding = 0 wq : array<Cm2BlockQ8P>
    @ssbo @binding = 1 wsh : array<float16>
    @ssbo @binding = 2 xb : array<float16>
    @ssbo @binding = 3 yc : array<float>
    @push_constant pa : Cm2ClsArgs
    @workgroup blk0_sh : uint

    // the member reads are the form's whole point: wsh (a second SSBO the rigid decode signature
    // cannot carry), pa (push constants), blk0_sh (@workgroup staging by the kernel body)
    [spirv_decode, unused_argument(bc)]
    def decode_q8(blk : Cm2BlockQ8P; bc, cib : uint2) : float16 {
        let w = blk.qs[int(cib.y >> 2u)]
        let q = (int(w) << ((3 - int(cib.y & 3u)) * 8)) >> 24
        return wsh[blk0_sh + bc.x * (pa.n >> 5u) + bc.y] * float16(float(q))
    }

    [spirv_kernel(local_size_x = 256, name = "cm2_cls_spv")]
    def run {
        if (gl_LocalInvocationID.x == 0u) {
            blk0_sh = pa.blk0
        }
        barrier()
        var tla : tensorLayout2DPad
        tensorLayoutCreate(tla)
        tensorLayoutSetBlockSize(tla, 1u, 32u)
        tensorLayoutSetDimension(tla, pa.d, pa.n)
        tensorLayoutSetStride(tla, pa.n >> 5u, 1u)
        var tlb : tensorLayout2DPad
        tensorLayoutCreate(tlb)
        tensorLayoutSetDimension(tlb, 256u, pa.n)
        tensorLayoutSetStride(tlb, pa.n, 1u)
        var tlo : tensorLayout2D
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, 256u, pa.d)
        tensorLayoutSetStride(tlo, pa.d, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var a : coopmatWgA_f16_128x64
        var b : coopmatWgB_f16_64x256
        var acc : coopmatWgAcc_f16_128x256
        let ir = gl_WorkGroupID.x
        var k = 0u
        while (k < pa.n) {
            coopmatLoadTensorDecode(a, wq, 0u, tla, ir * 128u, 128u, k, 64u, self.decode_q8)
            coopmatLoadTensor(b, xb, 0u, tlb, 0u, 256u, k, 64u, tv)
            acc = coopmatMulAdd(a, b, acc)
            k += 64u
        }
        coopmatClamp(acc, -65504.0, 65504.0)
        var accw : coopmatWgAcc_f32_128x256
        coopmatConvert(accw, acc)
        coopmatStoreTensor(accw, yc, 0u, tlo, 0u, 256u, ir * 128u, 128u, tv)
    }
}

[test]
def test_coopmat2_cls(t : T?) {
    t |> run("cm2 decode method: self-erased signature + member access + l-tile shapes") <| @(t : T?) {
        // the decode method emits as the SAME rigid shape the module-level form does: a second
        // OpFunction with exactly 3 parameters — the das-level self is erased
        t |> success(count_op(cm2_cls_spv, SpvOp.Function) == 2,
            "two OpFunction (main + the decode method)")
        t |> success(count_op(cm2_cls_spv, SpvOp.FunctionParameter) == 3,
            "three OpFunctionParameter (self erased: block pointer + two uint[2] coords)")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Decorate, 1, uint(SpvDecoration.Aliased)) == 1,
            "the decode method's PSB pointer parameter is decorated Aliased")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Load, 3, uint(SpvMemoryAccess.Aligned)) >= 1,
            "the decode method's PSB loads carry the Aligned memory operand")
        // member access from the decode body: the members classify as module-scope OpVariables in
        // their storage classes (storage class = OpVariable operand 2)
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Variable, 2, uint(SpvStorageClass.StorageBuffer)) == 4,
            "four StorageBuffer OpVariable (wq, wsh, xb, yc)")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Variable, 2, uint(SpvStorageClass.PushConstant)) == 1,
            "one PushConstant OpVariable (pa)")
        t |> success(count_op_operand_at(cm2_cls_spv, SpvOp.Variable, 2, uint(SpvStorageClass.Workgroup)) == 1,
            "one Workgroup OpVariable (blk0_sh)")
        // the l-tile flexible-dimension types: A 128x64, B 64x256, acc f16 + f32 128x256
        t |> success(count_op(cm2_cls_spv, SpvOp.TypeCooperativeMatrixKHR) == 4,
            "four OpTypeCooperativeMatrixKHR (A, B, f16 acc, f32 acc)")
        // the GEMM shape: decode load + view load, one MMA, one clamped tensor store
        t |> success(count_op(cm2_cls_spv, SpvOp.CooperativeMatrixLoadTensorNV) == 2,
            "two OpCooperativeMatrixLoadTensorNV (DecodeFunc A, TensorView B)")
        t |> success(count_op(cm2_cls_spv, SpvOp.CooperativeMatrixStoreTensorNV) == 1,
            "one OpCooperativeMatrixStoreTensorNV")
        t |> success(count_op(cm2_cls_spv, SpvOp.CooperativeMatrixMulAddKHR) == 1,
            "one OpCooperativeMatrixMulAddKHR")
        let r = validate_spirv(cm2_cls_spv, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
    }
}
