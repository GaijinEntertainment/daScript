options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7.3 sampler2DArray GPU gate shaders: a fullscreen quad whose fragment picks an array layer from
// its screen quadrant and samples a 2D-array texture. With each layer a distinct solid color and NEAREST
// filtering, every quadrant renders exactly its layer's color -- the real-driver proof that the array
// Arrayed image + the layer coordinate (.z) select the right layer.

module array_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

// fullscreen quad (two triangles) from gl_VertexIndex, passing a [0,1]^2 uv
var @out @location = 0 av_uv : float2
[vertex_shader(name="arr_vert_spv")]
def arr_vert {
    let pos = fixed_array(
        float2(-1.0, -1.0), float2(1.0, -1.0), float2(1.0, 1.0),
        float2(-1.0, -1.0), float2(1.0, 1.0), float2(-1.0, 1.0))
    let uv = fixed_array(
        float2(0.0, 0.0), float2(1.0, 0.0), float2(1.0, 1.0),
        float2(0.0, 0.0), float2(1.0, 1.0), float2(0.0, 1.0))
    gl_Position = float4(pos[gl_VertexIndex], 0.0, 1.0)
    av_uv = uv[gl_VertexIndex]
}

// pick a layer 0..3 from the quadrant, sample the 2D array at (u, v, layer)
var @binding = 0 arr_tex : sampler2DArray
var @in @location = 0 af_uv : float2
var @out @location = 0 arr_color : float4
[fragment_shader(name="arr_frag_spv")]
def arr_frag {
    var layer = 0.0f
    if (af_uv.x > 0.5f) {
        layer = layer + 1.0f
    }
    if (af_uv.y > 0.5f) {
        layer = layer + 2.0f
    }
    arr_color = texture(arr_tex, float3(af_uv.x, af_uv.y, layer))
}
