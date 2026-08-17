options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 04 -- the synthwave cube. Vertex pulls (pos, uv, normal) from a per-vertex buffer,
// transforms by a per-frame model push constant + a UBO view/proj. Fragment samples a procedural
// synthwave-horizon texture, modulates by a key+rim lighting pair, and scrolls the UV vertically
// over time so the texture "drifts" like a CRT. The push constant carries only the model matrix
// (vertex-only); time + camera position live in the UBO so the fragment can read them too.
//
// Every line is daslang, lowered to SPIR-V at compile time by dasSpirv. The rails exercised here:
// per-vertex attribute input (location-decorated), UBO struct with three mat4-sized members
// (std140 column-major), push-constant matrix, combined image sampler in the fragment, and the
// GLSL.std.450 math used by the lighting (`normalize`, `dot`, `max`, `pow`).

module cube_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory types used by the generated bind_uniform_<shader> functions
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== shared UBO =====

//! View+projection matrices and the camera world position (xyz) + time (w) packed in one vec4 so
//! the std140 layout has no scalar-after-vec3 packing surprises -- 64+64+16 = 144 bytes total.
struct Camera {
    view     : float4x4
    proj     : float4x4
    cam_time : float4     // xyz = camera world position, w = time in seconds
}
var @uniform @set = 0 @binding = 0 cam : Camera

//! Per-frame model matrix (vertex-only push constant). 64 bytes -- well under the 128 byte
//! push-constant minimum guarantee.
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

[vulkan_vertex_shader(name="cube_vert_spv")]
def cube_vs {
    let world = pc.model * float4(a_pos, 1.0)
    gl_Position = cam.proj * cam.view * world
    v_world_pos = world.xyz
    // assumes pc.model has uniform scale + rotation only (no shear / non-uniform scale), which is
    // true for the breathing rotating cube; in that case a vec4(normal, 0) multiply transforms the
    // normal correctly without an inverse-transpose.
    v_world_normal = (pc.model * float4(a_normal, 0.0)).xyz
    v_uv = a_uv
}

// ===== fragment shader =====

var @set = 0 @binding = 1 tex : sampler2D

var @in @location = 0 f_world_pos    : float3
var @in @location = 1 f_world_normal : float3
var @in @location = 2 f_uv           : float2
var @out @location = 0 frag_color    : float4

[vulkan_fragment_shader(name="cube_frag_spv")]
def cube_fs {
    let n = normalize(f_world_normal)
    let v = normalize(cam.cam_time.xyz - f_world_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))                // warm key light from above-front
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3                // ambient floor 0.3
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.0)            // silhouette glow
    let rim_color = float3(0.4, 0.6, 1.0)                    // cool cyan
    let scrolled = float2(f_uv.x, f_uv.y + cam.cam_time.w * 0.05)
    let albedo = texture(tex, scrolled).rgb
    let lit = albedo * key + rim * rim_color
    frag_color = float4(lit, 1.0)
}
