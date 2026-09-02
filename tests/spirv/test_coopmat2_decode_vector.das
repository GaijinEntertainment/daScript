options gen2
options indenting = 4

// The four-wide decode twin (SPV_NV_cooperative_matrix_decode_vector): the tenth
// coopmatLoadTensorDecode argument. `true` synthesizes the twin over the scalar body (four
// OpFunctionCalls composed into the vector); a [spirv_decode] method returning half4 is a hand-laid
// twin. Both forms keep the scalar DecodeFunc on the load beside the DecodeVectorFunc, so the mask
// word carries both bits and two operands follow it. The extension adds no opcode, so the census
// cannot see it: the assertions here are on the mask word, the operand count and the function set.

require dastest/testing_boost public
require _spirv_common               // validate_spirv, coopmat2_words, Cm2BlockQ8, cm2_decode_q8
require spirv/spirv_shader
require spirv/spirv_builtins
require spirv/spirv_dis             // count_op, op_has_operand, count_op_operand_at, strip_decode_vector
require spirv/spirv_builder         // string_words
require spirv/spirv_grammar         // SpvOp, SpvCapability, SpvTensorAddressingOperands

var @ssbo @binding = 0 dv_a : array<Cm2BlockQ8>
var @ssbo @binding = 1 dv_b : array<float16>
var @ssbo @binding = 2 dv_c : array<float>
var @ssbo @binding = 3 dv_meta : array<uint>

// the scalar q8 GEMM of _spirv_common, with the twin synthesized over cm2_decode_q8
[compute_shader(local_size_x=128, name="coopmat2_dv_synth_spv"), marker(no_coverage)]
def coopmat2_dv_synth_kernel {
    let m = dv_meta[0u]
    let n = dv_meta[1u]
    let kk = dv_meta[2u]
    let ti = gl_WorkGroupID.x
    let tj = gl_WorkGroupID.y
    var tla : tensorLayout2DPad
    tensorLayoutCreate(tla)
    tensorLayoutSetBlockSize(tla, 1u, 32u)
    tensorLayoutSetDimension(tla, m, kk)
    tensorLayoutSetStride(tla, kk / 32u, 1u)
    var tlb : tensorLayout2DPad
    tensorLayoutCreate(tlb)
    tensorLayoutSetDimension(tlb, n, kk)
    tensorLayoutSetStride(tlb, kk, 1u)
    var tlo : tensorLayout2D
    tensorLayoutCreate(tlo)
    tensorLayoutSetDimension(tlo, m, n)
    tensorLayoutSetStride(tlo, n, 1u)
    var tv : tensorView2Dt
    tensorViewCreate(tv)
    var a : coopmatWgA_f16_64x16
    var b : coopmatWgB_f16_16x32
    var acc : coopmatWgAcc_f16_64x32
    var k = 0u
    while (k < kk) {
        coopmatLoadTensorDecode(a, dv_a, 0u, tla, ti * 64u, 64u, k, 16u, @@cm2_decode_q8, true)
        coopmatLoadTensor(b, dv_b, 0u, tlb, tj * 32u, 32u, k, 16u, tv)
        acc = coopmatMulAdd(a, b, acc)
        k += 16u
    }
    coopmatClamp(acc, -65504.0, 65504.0)
    var accw : coopmatWgAcc_f32_64x32
    coopmatConvert(accw, acc)
    coopmatStoreTensor(accw, dv_c, 0u, tlo, ti * 64u, 64u, tj * 32u, 32u)
}

struct DvClsArgs {
    d : uint
    n : uint
}

// the hand-laid twin as a class method: four consecutive quants are two u16 lanes of the block
class DvDecodeCls {
    @ssbo @binding = 0 wq : array<Cm2BlockQ8>
    @ssbo @binding = 1 xb : array<float16>
    @ssbo @binding = 2 yc : array<float>
    @push_constant pa : DvClsArgs

    [spirv_decode, unused_argument(bc)]
    def decode_q8(blk : Cm2BlockQ8; bc, cib : uint2) : float16 {
        let s = uint(blk.qs[int((cib.y & 30u) >> 1u)])
        let sh = 24 - int(cib.y & 1u) * 8
        let q = (int(s) << sh) >> 24
        return blk.d * float16(float(q))
    }

    [spirv_decode, unused_argument(bc)]
    def decode_q8_v4(blk : Cm2BlockQ8; bc, cib : uint2) : half4 {
        let i0 = int((cib.y & 28u) >> 1u)
        let s0 = int(uint(blk.qs[i0]))
        let s1 = int(uint(blk.qs[i0 + 1]))
        let d = float(blk.d)
        return half4(float4(float((s0 << 24) >> 24) * d, float((s0 << 16) >> 24) * d,
            float((s1 << 24) >> 24) * d, float((s1 << 16) >> 24) * d))
    }

    [spirv_kernel(local_size_x = 256, name = "coopmat2_dv_hand_spv")]
    def run {
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
            coopmatLoadTensorDecode(a, wq, 0u, tla, ir * 128u, 128u, k, 64u, self.decode_q8, self.decode_q8_v4)
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

def private both_bits : uint => uint(SpvTensorAddressingOperands.DecodeFunc) | uint(SpvTensorAddressingOperands.DecodeVectorFunc)

[test]
def test_coopmat2_decode_vector_synth(t : T?) {
    t |> run("synthesized twin: capability + extension, both mask bits, four calls of the scalar body") <| @(t : T?) {
        let base <- coopmat2_words()
        t |> success(op_has_operand(coopmat2_dv_synth_spv, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "OpCapability CooperativeMatrixDecodeVectorNV")
        t |> success(!op_has_operand(base, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "the scalar-only load declares no such capability")
        t |> success(count_op(coopmat2_dv_synth_spv, SpvOp.Extension) == count_op(base, SpvOp.Extension) + 1,
            "one OpExtension more than the scalar-only kernel (SPV_NV_cooperative_matrix_decode_vector)")
        t |> success(count_op_operand_at(coopmat2_dv_synth_spv, SpvOp.CooperativeMatrixLoadTensorNV, 6, both_bits()) == 1,
            "the decode load's addressing mask carries DecodeFunc | DecodeVectorFunc")
        t |> success(count_op_operand_at(base, SpvOp.CooperativeMatrixLoadTensorNV, 6, uint(SpvTensorAddressingOperands.DecodeFunc)) == 1,
            "the scalar-only load's mask carries DecodeFunc alone")
        t |> success(count_op(coopmat2_dv_synth_spv, SpvOp.Function) == 3,
            "three OpFunction (main, the scalar decode, the synthesized twin)")
        t |> success(count_op(coopmat2_dv_synth_spv, SpvOp.FunctionCall) == 4,
            "the twin calls the scalar body four times")
        t |> success(count_op(coopmat2_dv_synth_spv, SpvOp.FunctionParameter) == 6,
            "six OpFunctionParameter (the rigid three, twice)")
        t |> success(count_op(coopmat2_dv_synth_spv, SpvOp.IAdd) >= 3,
            "the twin steps coordInBlock.y by 1, 2, 3")
        let r = validate_spirv(coopmat2_dv_synth_spv, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
    }
}

[test]
def test_coopmat2_decode_vector_strip(t : T?) {
    t |> run("strip_decode_vector: the device-without-the-feature fallback leaves a scalar-only module") <| @(t : T?) {
        var stripped := coopmat2_dv_synth_spv
        t |> success(strip_decode_vector(stripped), "a module naming the twin reports the strip")
        t |> success(!op_has_operand(stripped, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "the capability is gone")
        t |> success(count_op(stripped, SpvOp.Extension) == count_op(coopmat2_dv_synth_spv, SpvOp.Extension) - 1,
            "the extension declaration is gone")
        t |> success(count_op_operand_at(stripped, SpvOp.CooperativeMatrixLoadTensorNV, 6, uint(SpvTensorAddressingOperands.DecodeFunc)) == 1,
            "the load's mask carries DecodeFunc alone")
        t |> success(length(stripped) == length(coopmat2_dv_synth_spv) - 2 - (1 + length(string_words("SPV_NV_cooperative_matrix_decode_vector"))) - 1,
            "exactly the capability, the extension and one operand word left the module")
        t |> success(count_op(stripped, SpvOp.Function) == 3,
            "the twin's OpFunction stays, unreferenced")
        var again := stripped
        t |> success(!strip_decode_vector(again), "a scalar-only module reports nothing to strip")
        t |> success(length(again) == length(stripped), "and is left as it was")
        let r = validate_spirv(stripped, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3) on the stripped module: {r.msg}")
        }
        delete again
        delete stripped
    }
}

[test]
def test_coopmat2_decode_vector_hand(t : T?) {
    t |> run("hand-laid twin as a class method: half4 return, both mask bits, no synthesized calls") <| @(t : T?) {
        t |> success(op_has_operand(coopmat2_dv_hand_spv, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixDecodeVectorNV)),
            "OpCapability CooperativeMatrixDecodeVectorNV")
        t |> success(count_op_operand_at(coopmat2_dv_hand_spv, SpvOp.CooperativeMatrixLoadTensorNV, 6, both_bits()) == 1,
            "the decode load's addressing mask carries DecodeFunc | DecodeVectorFunc")
        t |> success(count_op(coopmat2_dv_hand_spv, SpvOp.Function) == 3,
            "three OpFunction (main, the scalar method, the hand-laid twin)")
        t |> success(count_op(coopmat2_dv_hand_spv, SpvOp.FunctionCall) == 0,
            "no OpFunctionCall: the hand-laid twin has its own body")
        t |> success(count_op(coopmat2_dv_hand_spv, SpvOp.FunctionParameter) == 6,
            "six OpFunctionParameter (self erased from both methods)")
        let r = validate_spirv(coopmat2_dv_hand_spv, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
    }
}
