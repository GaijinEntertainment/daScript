options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 09 -- MSAA + dynamic rendering on the synthwave cube. Pedagogically the cube is the same
// scene as tutorial 04, but the render PIPELINE is different: this pipeline carries a
// VkPipelineRenderingCreateInfo in pNext (so no VkRenderPass / VkFramebuffer object), and writes
// into a 4x multi-sample color attachment that resolves into a 1x readback target. The shader code
// itself is unchanged -- MSAA happens at the pipeline / attachment level, invisible to the fragment
// program.

module msaa_tut_shaders public

require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== shared UBO =====

struct Camera {
    view     : float4x4
    proj     : float4x4
    cam_time : float4     // xyz = camera world position, w = time in seconds
}
var @uniform @set = 0 @binding = 0 cam : Camera

struct ModelPC {
    model : float4x4
}
var @push_constant pc : ModelPC

// ===== vertex shader =====

var @in @location = 0 a_pos    : float3
var @in @location = 1 a_uv     : float2
var @in @location = 2 a_normal : float3

var @out @location = 0 v_world_pos    : float3
var @out @location = 1 v_world_normal : float3
var @out @location = 2 v_uv           : float2

[vulkan_vertex_shader(name="msaa_vert_spv")]
def msaa_vs {
    let world = pc.model * float4(a_pos, 1.0)
    gl_Position = cam.proj * cam.view * world
    v_world_pos = world.xyz
    v_world_normal = (pc.model * float4(a_normal, 0.0)).xyz
    v_uv = a_uv
}

// ===== fragment shader =====

var @set = 0 @binding = 1 tex : sampler2D

var @in @location = 0 f_world_pos    : float3
var @in @location = 1 f_world_normal : float3
var @in @location = 2 f_uv           : float2
var @out @location = 0 frag_color    : float4

[vulkan_fragment_shader(name="msaa_frag_spv")]
def msaa_fs {
    let n = normalize(f_world_normal)
    let v = normalize(cam.cam_time.xyz - f_world_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.0)
    let rim_color = float3(0.4, 0.6, 1.0)
    let scrolled = float2(f_uv.x, f_uv.y + cam.cam_time.w * 0.05)
    let albedo = texture(tex, scrolled).rgb
    let lit = albedo * key + rim * rim_color
    frag_color = float4(lit, 1.0)
}
