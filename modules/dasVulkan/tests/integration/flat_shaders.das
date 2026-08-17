options gen2
options _comment_hygiene = true
options indenting = 4

// Fullscreen solid-white shaders for the batch-creator / boost-pipeline tests: the
// single-triangle fullscreen trick covers every pixel, and a constant white fragment
// makes color-blend state (CONSTANT_COLOR blend factors) exactly predictable per pixel.

module flat_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

[vertex_shader(name="flat_vert_spv")]
def flat_vert {
    let positions = fixed_array(float2(-1.0, -1.0), float2(3.0, -1.0), float2(-1.0, 3.0))
    gl_Position = float4(positions[gl_VertexIndex], 0.0, 1.0)
}

var @out @location = 0 frag_color : float4
[fragment_shader(name="flat_frag_spv")]
def flat_frag {
    frag_color = float4(1.0, 1.0, 1.0, 1.0)
}
