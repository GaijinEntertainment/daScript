options gen2
options indenting = 4

//! the head-96 flash-tile shapes: A 64x96 x B 96x32 into a 64x32
//! accumulator (the Q K^T step), A 64x32 x B 32x96 into a 64x96 one
//! (the P V step), with the f32 -> f16 convert of the 64x96 tile;
//! spirv-val (vulkan1.3) is the independent witness

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_shader
require spirv/spirv_dis
require spirv/spirv_grammar

struct Cm2H96Args {
    n : uint
}

class Cm2H96Cls {
    @ssbo @binding = 0 qa : array<float16>
    @ssbo @binding = 1 kb : array<float16>
    @ssbo @binding = 2 vb : array<float16>
    @ssbo @binding = 3 yc : array<float16>
    @push_constant pa : Cm2H96Args

    [spirv_kernel(local_size_x = 256, name = "cm2_h96_spv")]
    def run {
        var tlq : tensorLayout2D
        tensorLayoutCreate(tlq)
        tensorLayoutSetDimension(tlq, 64u, 96u)
        tensorLayoutSetStride(tlq, 96u, 1u)
        var tlk : tensorLayout2D
        tensorLayoutCreate(tlk)
        tensorLayoutSetDimension(tlk, 96u, pa.n)
        tensorLayoutSetStride(tlk, pa.n, 1u)
        var tlv : tensorLayout2D
        tensorLayoutCreate(tlv)
        tensorLayoutSetDimension(tlv, pa.n, 96u)
        tensorLayoutSetStride(tlv, 96u, 1u)
        var tlo : tensorLayout2D
        tensorLayoutCreate(tlo)
        tensorLayoutSetDimension(tlo, 64u, 96u)
        tensorLayoutSetStride(tlo, 96u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
        var q : coopmatWgA_f16_64x96
        coopmatLoadTensor(q, qa, 0u, tlq, 0u, 64u, 0u, 96u, tv)
        var o : coopmatWgAcc_f32_64x96
        var kc = 0u
        while (kc < pa.n) {
            var s : coopmatWgAcc_f32_64x32
            var k : coopmatWgB_f16_96x32
            coopmatLoadTensor(k, kb, 0u, tlk, 0u, 96u, kc, 32u, tv)
            s = coopmatMulAdd(q, k, s)
            var p : coopmatWgA_f16_64x32
            coopmatConvert(p, s)
            var v : coopmatWgB_f16_32x96
            coopmatLoadTensor(v, vb, 0u, tlv, kc, 32u, 0u, 96u, tv)
            o = coopmatMulAdd(p, v, o)
            kc += 32u
        }
        coopmatClamp(o, -65504.0, 65504.0)
        var oh : coopmatWgAcc_f16_64x96
        coopmatConvert(oh, o)
        coopmatStoreTensor(oh, yc, 0u, tlo, 0u, 64u, 0u, 96u, tv)
    }
}

[test]
def test_coopmat2_h96(t : T?) {
    t |> run("cm2 head-96 flash shapes: 64x96 x 96x32 and 64x32 x 32x96") <| @(t : T?) {
        //! A 64x96, B 96x32, acc f32 64x32, A 64x32, B 32x96, acc f32 64x96,
        //! acc f16 64x96
        t |> success(count_op(cm2_h96_spv, SpvOp.TypeCooperativeMatrixKHR) == 7,
            "seven OpTypeCooperativeMatrixKHR (two A, two B, two f32 accumulators, the f16 one)")
        t |> success(count_op(cm2_h96_spv, SpvOp.CooperativeMatrixMulAddKHR) == 2,
            "two OpCooperativeMatrixMulAddKHR (Q K^T, then P V)")
        t |> success(count_op(cm2_h96_spv, SpvOp.CooperativeMatrixLoadTensorNV) == 3,
            "three OpCooperativeMatrixLoadTensorNV (Q, K, V)")
        t |> success(count_op(cm2_h96_spv, SpvOp.CooperativeMatrixStoreTensorNV) == 1,
            "one OpCooperativeMatrixStoreTensorNV")
        let r = validate_spirv(cm2_h96_spv, "vulkan1.3")
        if (r.ran) {
            t |> success(r.ok, "spirv-val (vulkan1.3): {r.msg}")
        }
    }
}
