options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 01 - the rotating triangle. The classic gl_VertexIndex hello-triangle (shader-local
// clip-space positions + per-vertex RGB, interpolated across the face), spun by a push-constant
// angle so the tutorial's capture loop has a real per-frame GPU parameter to drive. Shaders are
// authored in daslang and lowered to SPIR-V at compile time by dasSpirv (require spirv/spirv_shader):
// the emitted tri_spin_vert_spv / tri_spin_frag_spv : array<uint> globals feed create_shader_module.

module triangle_tut_shaders public

require vulkan/vulkan_boost public  // CommandBuffer + PipelineLayout for the generated push_constants function
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// the per-frame rotation angle (radians), pushed each draw. @push_constant blocks are struct-typed
// (the std430 Block the emitter decorates), so the lone float lives in a one-field struct.
struct SpinPush {
    angle : float
}
var @push_constant pc : SpinPush

var @out @location = 0 v_color : float3
[vulkan_vertex_shader(name="tri_spin_vert_spv")]
def tri_spin_vert {
    let positions = fixed_array(float2(0.0, -0.6), float2(0.6, 0.6), float2(-0.6, 0.6))
    let colors = fixed_array(float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0))
    let p = positions[gl_VertexIndex]
    let c = cos(pc.angle)
    let s = sin(pc.angle)
    gl_Position = float4(p.x * c - p.y * s, p.x * s + p.y * c, 0.0, 1.0)
    v_color = colors[gl_VertexIndex]
}

var @in @location = 0 fi_color : float3
var @out @location = 0 frag_color : float4
[vulkan_fragment_shader(name="tri_spin_frag_spv")]
def tri_spin_frag {
    frag_color = float4(fi_color, 1.0)
}
