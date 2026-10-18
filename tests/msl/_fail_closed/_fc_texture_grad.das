expect 50501
options gen2
require metal/msl_shader
require daslib/shader_lingua_franca
class FcTextureGrad {
    @ssbo @binding = 0 dst : array<float4>
    [metal_kernel(name="fc_texture_grad_msl")]
    def kern {
        var sampler : sampler2D
        dst[0] = textureGrad(sampler, float2(0.5), float2(0.1, 0.0), float2(0.0, 0.1))
    }
}
