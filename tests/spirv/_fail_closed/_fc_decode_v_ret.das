// Fail-closed fixture: a hand-laid four-wide decode twin must return half4 / float4 - a scalar
// result in the tenth coopmatLoadTensorDecode slot is rejected at registration. `expect 50501`
// keeps lint off the intentionally-failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct FcBlk {
    qs : uint[8]
}

var @ssbo @binding = 0 wq : array<FcBlk>
var @ssbo @binding = 1 yc : array<float>
var @ssbo @binding = 2 meta : array<uint>

[spirv_decode, unused_argument(blk, bc, cib)]
def fc_decode(blk : FcBlk; bc, cib : uint2) : float16 {
    return float16(0.0)
}

[spirv_decode, unused_argument(blk, bc, cib)]
def fc_decode_v(blk : FcBlk; bc, cib : uint2) : float16 {
    return float16(1.0)
}

[compute_shader(local_size_x=64, name="fc_decode_v_ret_spv")]
def fc_decode_v_ret {
    var tla : tensorLayout2DPad
    tensorLayoutCreate(tla)
    tensorLayoutSetBlockSize(tla, 1u, 32u)
    tensorLayoutSetDimension(tla, meta[0u], meta[1u])
    tensorLayoutSetStride(tla, meta[1u] >> 5u, 1u)
    var tlo : tensorLayout2D
    tensorLayoutCreate(tlo)
    tensorLayoutSetDimension(tlo, meta[0u], meta[1u])
    tensorLayoutSetStride(tlo, meta[1u], 1u)
    var a : coopmatWgA_f16_64x16
    coopmatLoadTensorDecode(a, wq, 0u, tla, 0u, 64u, 0u, 16u, @@fc_decode, @@fc_decode_v)
    var b : coopmatWgB_f16_16x16
    var acc : coopmatWgAcc_f16_64x16
    acc = coopmatMulAdd(a, b, acc)
    var accw : coopmatWgAcc_f32_64x16
    coopmatConvert(accw, acc)
    coopmatStoreTensor(accw, yc, 0u, tlo, 0u, 64u, 0u, 16u)
}
