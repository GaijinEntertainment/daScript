options gen2
module _texture_grad_shaders shared public
require spirv/spirv_shader
require spirv/spirv_builtins public

var @binding = 0 grad_2d : sampler2D
var @binding = 1 grad_3d : sampler3D
var @binding = 2 grad_cube : samplerCube
var @binding = 3 grad_array : sampler2DArray
var @ssbo @binding = 4 grad_out : array<float4>

[compute_shader(local_size_x=1, name="texture_grad_spv")]
def texture_grad_compute {
    grad_out[0] = textureGrad(grad_2d, float2(0.5), float2(0.25, 0.0), float2(0.0, 0.125))
    grad_out[1] = textureGrad(grad_3d, float3(0.5), float3(0.25, 0.0, 0.0), float3(0.0, 0.125, 0.0))
    grad_out[2] = textureGrad(grad_cube, float3(0.0, 0.0, 1.0), float3(0.25, 0.0, 0.0), float3(0.0, 0.125, 0.0))
    grad_out[3] = textureGrad(grad_array, float3(0.5, 0.5, 2.0), float2(0.25, 0.0), float2(0.0, 0.125))
}
