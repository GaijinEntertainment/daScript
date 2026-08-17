options gen2
options _comment_hygiene = true
options indenting = 4

// Triangle shaders authored in daslang, lowered to SPIR-V at compile time by dasSpirv's
// [vertex_shader]/[fragment_shader] macros. Shared by the triangle examples (offscreen_triangle,
// offscreen_triangle_boost, window_triangle) via a same-directory require. The emitted blobs are the
// public `tri_vert_spv` / `tri_frag_spv : array<uint>` globals, fed to create_shader_module. This
// replaces the committed examples/shaders/triangle.vert/.frag(.spv) — no GLSL, no glslang.
//
// Distinct module name from tests/integration/triangle_shaders.das (the same shaders kept as the
// test's own same-dir fixture) so `require` resolution is never ambiguous between the two.

module triangle_example_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

// vertex: the classic gl_VertexIndex hardcoded triangle — shader-local constant arrays of clip-space
// positions + per-vertex colors. v0 top (red), v1 bottom-right (green), v2 bottom-left (blue).
var @out @location = 0 v_color : float3
[vertex_shader(name="tri_vert_spv")]
def tri_vert {
    let positions = fixed_array(float2(0.0, -0.6), float2(0.6, 0.6), float2(-0.6, 0.6))
    let colors = fixed_array(float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0))
    gl_Position = float4(positions[gl_VertexIndex], 0.0, 1.0)
    v_color = colors[gl_VertexIndex]
}

// fragment: the interpolated varying -> framebuffer color, opaque.
var @in @location = 0 fi_color : float3
var @out @location = 0 frag_color : float4
[fragment_shader(name="tri_frag_spv")]
def tri_frag {
    frag_color = float4(fi_color, 1.0)
}
