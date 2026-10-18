expect 30341
options gen2
require spirv/spirv_shader
require spirv/spirv_builtins public
var @binding = 0 tex : sampler2DArray
var @ssbo @binding = 1 dst : array<float4>
[compute_shader(local_size_x=1, name="invalid_grad_spv")]
def invalid_grad {
    dst[0] = textureGrad(tex, float3(0.5), float3(0.1), float3(0.1))
}
