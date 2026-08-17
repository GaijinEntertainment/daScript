options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-4 GPU-gate shaders: a textured quad transformed by a UBO matrix and modulated by a
// push-constant tint, authored in daslang and lowered to SPIR-V at compile time by dasSpirv.
// This is the real-driver proof of the Phase-4 emitter surface (UBO + column-major matrix +
// combined image sampler + push constant) -- spirv-val can confirm the blob is structurally valid,
// but only a rendered image can confirm the column-major matrix layout, the sampler UV mapping, and
// the push-constant delivery are SEMANTICALLY correct end to end. The host (scene_helpers.das)
// uploads the MVP column-major; a correct render proves daslang's float4x4 layout matches the
// emitter's ColMajor + MatrixStride 16 decoration.

module phase4_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public
require math

// vertex: a unit quad (two triangles, 6 verts) indexed by gl_VertexIndex, transformed by the UBO
// MVP. Per-vertex UV is passed to the fragment stage. The MVP is read column-major from the @uniform
// block (ColMajor + MatrixStride 16); cam.mvp * float4(pos,0,1) lowers to OpMatrixTimesVector.
struct Camera {
    mvp : float4x4      // std140 offset 0 (ColMajor, MatrixStride 16, size 64)
}
var @uniform @set = 0 @binding = 0 cam : Camera

var @out @location = 0 v_uv : float2
[vertex_shader(name="quad_vert_spv")]
def quad_vert {
    let positions = fixed_array(
        float2(-1.0, -1.0), float2(1.0, -1.0), float2(-1.0, 1.0),
        float2(-1.0, 1.0), float2(1.0, -1.0), float2(1.0, 1.0))
    let uvs = fixed_array(
        float2(0.0, 0.0), float2(1.0, 0.0), float2(0.0, 1.0),
        float2(0.0, 1.0), float2(1.0, 0.0), float2(1.0, 1.0))
    let m = cam.mvp
    gl_Position = m * float4(positions[gl_VertexIndex], 0.0, 1.0)
    v_uv = uvs[gl_VertexIndex]
}

// fragment: sample the combined image sampler at the interpolated UV, modulate by the push-constant
// tint. tint=(1,1,0) zeroes blue: a white texel renders yellow (the unambiguous tint proof -- a
// white texel can only lose its blue channel if the push constant was delivered).
struct Tint {
    rgb : float3        // push-constant offset 0
}
var @push_constant tint : Tint
var @set = 0 @binding = 1 tex : sampler2D   // combined image sampler

var @in @location = 0 fi_uv : float2
var @out @location = 0 frag_color : float4
[fragment_shader(name="quad_frag_spv")]
def quad_frag {
    let texel = texture(tex, fi_uv)
    frag_color = float4(texel.rgb * tint.rgb, 1.0)
}
