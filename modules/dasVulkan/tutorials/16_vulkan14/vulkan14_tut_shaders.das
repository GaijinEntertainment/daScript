options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 16 -- the Vulkan 1.4 scene's shaders. Two passes inside ONE dynamic rendering: the
// geometry pass draws three textured cubes into two G-buffer attachments (albedo at location 0, the
// world normal at location 1); the lighting pass reads both back at the same fragment through
// `subpassInput` / `subpassLoad` -- dynamic rendering LOCAL READ, Vulkan 1.4's form of a subpass -- and
// composes the lit color into the third attachment. Every descriptor the host binds is PUSHED per
// draw (vkCmdPushDescriptorSet, 1.4 core): the camera UBO, each cube's texture, both input attachments.
// None is allocated from a pool. One push-constant struct serves both passes: the vertex stage reads
// `model`, the lighting stage reads `light_dir`.

module vulkan14_tut_shaders public

require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== shared =====

//! View + projection, std140: two column-major mat4, 128 bytes.
struct Camera {
    view : float4x4
    proj : float4x4
}
var @uniform @set = 0 @binding = 0 cam : Camera

//! One push-constant block for both passes: the geometry pass stamps `model` per cube, the lighting
//! pass reads `light_dir` (xyz = unit direction TOWARD the light, w unused). 80 bytes.
struct ScenePC {
    model     : float4x4
    light_dir : float4
}
var @push_constant pc : ScenePC

// ===== geometry pass: cube -> albedo + normal =====

var @in @location = 0 a_pos    : float3
var @in @location = 1 a_uv     : float2
var @in @location = 2 a_normal : float3

var @out @location = 0 v_normal : float3
var @out @location = 1 v_uv     : float2

[vulkan_vertex_shader(name="gbuf_vert_spv")]
def gbuf_vs {
    gl_Position = cam.proj * cam.view * pc.model * float4(a_pos, 1.0)
    // pc.model is rotation + translation only, so the normal transforms with the same matrix
    v_normal = (pc.model * float4(a_normal, 0.0)).xyz
    v_uv = a_uv
}

var @set = 0 @binding = 1 tex : sampler2D   // this cube's texture, pushed before its draw

var @in @location = 0 f_normal : float3
var @in @location = 1 f_uv     : float2
var @out @location = 0 g_albedo : float4   // attachment 0 of the rendering
var @out @location = 1 g_normal : float4   // attachment 1: world normal packed into [0,1]

[vulkan_fragment_shader(name="gbuf_frag_spv")]
def gbuf_fs {
    g_albedo = float4(texture(tex, f_uv).rgb, 1.0)   // alpha 1 marks "a cube is here" for the lighting pass
    g_normal = float4(normalize(f_normal) * 0.5 + float3(0.5, 0.5, 0.5), 1.0)
}

// ===== lighting pass: albedo + normal -> lit color =====

//! Fullscreen triangle: (-1,-1), (3,-1), (-1,3) cover the whole viewport with three unindexed vertices.
[vulkan_vertex_shader(name="light_vert_spv")]
def light_vs {
    let xs = fixed_array(-1.0, 3.0, -1.0)
    let ys = fixed_array(-1.0, -1.0, 3.0)
    let i = gl_VertexIndex
    gl_Position = float4(xs[i], ys[i], 0.0, 1.0)
}

// The two G-buffer attachments, read at THIS fragment only. `@input_attachment_index` is the index
// the host maps attachment 0 and 1 to with vkCmdSetRenderingInputAttachmentIndices; the host binds
// them as INPUT_ATTACHMENT descriptors (pushed), in the RENDERING_LOCAL_READ layout.
var @set = 0 @binding = 0 @input_attachment_index = 0 in_albedo : subpassInput
var @set = 0 @binding = 1 @input_attachment_index = 1 in_normal : subpassInput

var @out @location = 0 out_color : float4   // location 0 is REMAPPED to attachment 2 by the host

[vulkan_fragment_shader(name="light_frag_spv")]
def light_fs {
    let albedo = subpassLoad(in_albedo)
    if (albedo.w < 0.5) {
        // nothing drawn here: a sky gradient, brighter toward the top (y = 0 is the top row in Vulkan)
        let t = gl_FragCoord.y / 512.0
        out_color = float4(0.05 + 0.10 * (1.0 - t), 0.06 + 0.12 * (1.0 - t), 0.12 + 0.20 * (1.0 - t), 1.0)
        return
    }
    let n = normalize(subpassLoad(in_normal).xyz * 2.0 - float3(1.0, 1.0, 1.0))
    let l = normalize(pc.light_dir.xyz)
    let diffuse = max(dot(n, l), 0.0) * 0.85 + 0.15   // ambient floor 0.15
    out_color = float4(albedo.xyz * diffuse, 1.0)
}
