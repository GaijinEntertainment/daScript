options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 05 -- a thousand cubes from one draw call. Two vertex bindings: per-vertex (pos + normal)
// at binding 0 with input rate VERTEX, per-instance (offset + color + phase) at binding 1 with input
// rate INSTANCE. vkCmdDrawIndexed with instanceCount=N_INSTANCES fans the same 36 indices over 1000
// instances; the per-instance attributes step once per instance, the per-vertex attributes step once
// per vertex. The shader sees them as plain ``@in @location`` inputs -- no special instancing keyword,
// instancing happens entirely on the host side via the binding's input rate.
//
// Geometry is the same cube as tutorial 04 (24 vertices, 36 indices) but flat-shaded -- the headline
// rail here is per-instance attributes, not lighting. The fragment uses one key light + the
// per-instance colour. Authored in daslang, lowered to SPIR-V at compile time by dasSpirv.

module instancing_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind_uniform helper
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== shared UBO =====

//! View+projection matrices + the camera world position (xyz) packed with time (w). Same shape as
//! tutorial 04's Camera, kept here so the tutorial reads independently.
struct Camera {
    view     : float4x4
    proj     : float4x4
    cam_time : float4     // xyz = camera world position, w = time in seconds
}
var @uniform @set = 0 @binding = 0 cam : Camera

// ===== per-vertex inputs (binding 0) =====

var @in @location = 0 a_pos    : float3
var @in @location = 1 a_normal : float3

// ===== per-instance inputs (binding 1) =====

//! Per-instance world offset. The host samples a 3D Lissajous curve into ``N_INSTANCES`` positions at
//! init; no per-frame buffer rewrite, the swarm is animated entirely from the vertex shader by
//! mixing the instance offset with time + per-instance phase.
var @in @location = 2 a_offset : float3
//! Per-instance RGB tint. Cycled through the hue circle three times across the swarm.
var @in @location = 3 a_color  : float3
//! Per-instance phase in [0,1), used to desynchronise the breathing animation.
var @in @location = 4 a_phase  : float

// ===== varyings =====

var @out @location = 0 v_world_pos    : float3
var @out @location = 1 v_world_normal : float3
var @out @location = 2 v_color        : float3

// ===== vertex shader =====

[vulkan_vertex_shader(name="instancing_vert_spv")]
def instancing_vs {
    let t = cam.cam_time.w
    // per-instance breathing: each cube scales between ~0.5 and ~0.9 at its own phase offset
    let breathe = 0.7 + 0.2 * sin(t * 1.5 + a_phase * 6.2832)
    let local = a_pos * breathe
    // gentle swarm-wide rotation around Y, slow enough to read at 30 fps
    let ang = t * 0.25 + a_phase * 6.2832
    let c = cos(ang)
    let s = sin(ang)
    let rotated = float3(c * local.x - s * local.z, local.y, s * local.x + c * local.z)
    let world = rotated + a_offset
    gl_Position = cam.proj * cam.view * float4(world, 1.0)
    v_world_pos = world
    // the normal is rotated by the same Y rotation; no per-instance scale shear so this is exact
    v_world_normal = float3(c * a_normal.x - s * a_normal.z, a_normal.y, s * a_normal.x + c * a_normal.z)
    v_color = a_color
}

// ===== fragment shader =====

var @in @location = 0 f_world_pos    : float3
var @in @location = 1 f_world_normal : float3
var @in @location = 2 f_color        : float3
var @out @location = 0 frag_color    : float4

[vulkan_fragment_shader(name="instancing_frag_spv")]
def instancing_fs {
    let n = normalize(f_world_normal)
    let v = normalize(cam.cam_time.xyz - f_world_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))                // warm key light from above-front
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3                // ambient floor 0.3
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.5)            // silhouette glow
    let lit = f_color * key + float3(0.25, 0.35, 0.55) * rim * 0.4
    frag_color = float4(lit, 1.0)
}
