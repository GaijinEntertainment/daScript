expect 30302
options gen2
require spirv/spirv_shader
require spirv/spirv_builtins public
var @binding = 0 tex : sampler2D
var @ssbo @binding = 1 dst : array<float4>
[compute_shader(local_size_x=1, name="invalid_grad_spv")]
def invalid_grad {
    dst[0] = textureGrad(tex, float2(0.5), 0.1, 0.1)
}
