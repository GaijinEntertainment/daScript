options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7.4 separate image + sampler GPU gate fragment shader: samples a standalone texture2D + sampler
// (combined at the call via sampleTexture -> OpSampledImage). The fullscreen-quad vertex shader is reused
// from array_shaders (arr_vert). Each quadrant samples a distinct checkerboard texel, so the readback
// proves the split image/sampler descriptors bind and combine correctly on a real driver.

module separate_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

var @binding = 0 sep_image : texture2D
var @binding = 1 sep_sampler : sampler
var @in @location = 0 sf_uv : float2
var @out @location = 0 sep_color : float4
[fragment_shader(name="sep_frag_spv")]
def sep_frag {
    sep_color = sampleTexture(sep_image, sep_sampler, sf_uv)
}
