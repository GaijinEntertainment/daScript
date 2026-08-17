options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 15 -- hardware ray tracing (VK_KHR_ray_tracing_pipeline): the shader side.
//
// Three RT stages, all authored in daslang and lowered by dasSpirv:
//
//   RAYGEN  fires one pinhole-camera ray per pixel (the camera orbits the scene by the `angle`
//           push constant), traces it into the TLAS, and imageStores the returned payload color.
//   MISS    paints the sky gradient -- it runs when a camera ray hits nothing.
//   CLOSEST-HIT  flat-shades the hit (per-face normals keyed off gl_PrimitiveID) and casts one
//           SHADOW ray toward the light: terminate-on-first-hit + skip-closest-hit flags, with
//           miss_index = 1 selecting the second miss shader below, which flips the payload to
//           "lit". Any occluder leaves the payload at 0 -- shadow without recursion into shading.
//
// The scene tables (vertices / indices / per-face normals) and the shading functions live here
// too, shared verbatim with the CPU reference tracer in raytracing_tut.das -- the same daslang
// functions run on the GPU (lowered to SPIR-V) and in the interpreter (the test's oracle).

module raytracing_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind helpers
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math public

// ===== the scene: a box on a ground plane =====

//! 12 vertices: ground quad [-30,30]^2 at y=0, then the 1.5-unit box resting on it.
def rt_scene_vertices : array<float3> {
    return <- [
        float3(-30.0, 0.0, -30.0), float3(30.0, 0.0, -30.0),
        float3(30.0, 0.0, 30.0), float3(-30.0, 0.0, 30.0),
        float3(-0.75, 0.0, -0.75), float3(0.75, 0.0, -0.75),
        float3(0.75, 0.0, 0.75), float3(-0.75, 0.0, 0.75),
        float3(-0.75, 1.5, -0.75), float3(0.75, 1.5, -0.75),
        float3(0.75, 1.5, 0.75), float3(-0.75, 1.5, 0.75)
    ]
}

//! 14 triangles: prims 0-1 ground, then box faces front/back/left/right/top/bottom (2 each) —
//! the order rt_normal() keys off.
def rt_scene_indices : array<uint> {
    return <- [
        0u, 1u, 2u, 0u, 2u, 3u,        // ground
        4u, 5u, 9u, 4u, 9u, 8u,        // front  (-z)
        7u, 6u, 10u, 7u, 10u, 11u,     // back   (+z)
        4u, 7u, 11u, 4u, 11u, 8u,      // left   (-x)
        5u, 6u, 10u, 5u, 10u, 9u,      // right  (+x)
        8u, 9u, 10u, 8u, 10u, 11u,     // top    (+y)
        4u, 5u, 6u, 4u, 6u, 7u         // bottom (-y)
    ]
}

//! flat per-face normal by primitive index (the closest-hit shader's whole "vertex fetch")
def rt_normal(prim : int) : float3 {
    let normals = fixed_array(
        float3(0.0, 1.0, 0.0), float3(0.0, 1.0, 0.0),
        float3(0.0, 0.0, -1.0), float3(0.0, 0.0, -1.0),
        float3(0.0, 0.0, 1.0), float3(0.0, 0.0, 1.0),
        float3(-1.0, 0.0, 0.0), float3(-1.0, 0.0, 0.0),
        float3(1.0, 0.0, 0.0), float3(1.0, 0.0, 0.0),
        float3(0.0, 1.0, 0.0), float3(0.0, 1.0, 0.0),
        float3(0.0, -1.0, 0.0), float3(0.0, -1.0, 0.0))
    return normals[prim]
}

def rt_ground_albedo : float3 => float3(0.42, 0.52, 0.42)
def rt_box_albedo : float3 => float3(0.85, 0.35, 0.18)

//! unit vector TOWARD the light: -normalize(float3(0.45, -1.0, 0.35)), pre-folded so the shader
//! reads plain literals
def rt_to_light : float3 => float3(-0.390936, 0.868747, -0.304062)

// ===== camera (orbits the box at CAM_HEIGHT, always looking at CAM_TARGET) =====

let CAM_DIST = 4.5
let CAM_HEIGHT = 2.2
let CAM_TAN = 0.5773503   // tan(30 deg): 60-degree vertical FOV

def rt_camera_origin(angle : float) : float3 => float3(sin(angle) * CAM_DIST, CAM_HEIGHT, -cos(angle) * CAM_DIST)

def rt_camera_dir(origin : float3; px, py : float; w, h : float) : float3 {
    let fwd = normalize(float3(0.0, 0.6, 0.0) - origin)
    let right = normalize(cross(fwd, float3(0.0, 1.0, 0.0)))
    let up = cross(right, fwd)
    let u = (px + 0.5) / w * 2.0 - 1.0
    let v = 1.0 - (py + 0.5) / h * 2.0   // image row 0 is the top -> +v is up
    return normalize(fwd + right * (u * CAM_TAN) + up * (v * CAM_TAN))
}

// ===== shading (shared GPU / CPU) =====

def sky_color(dir : float3) : float3 {
    let t = clamp(dir.y, 0.0, 1.0)
    return float3(0.75, 0.85, 0.95) + (float3(0.25, 0.45, 0.8) - float3(0.75, 0.85, 0.95)) * t
}

def shade_lambert(n, albedo : float3; lit : float) : float3 {
    let ndl = max(dot(n, rt_to_light()), 0.0)
    return albedo * (0.18 + 0.82 * ndl * lit)
}

// ===== the shaders =====

struct RtPush {
    angle : float
}
var @push_constant pc : RtPush

var @set = 0 @binding = 0 rt_tlas : accelerationStructureEXT
var @set = 0 @binding = 1 rt_img : image2D
var @ray_payload prd : float3

[vulkan_raygen_shader(name="raytracing_rgen_spv")]
def raytracing_rgen {
    let gid = gl_LaunchIDEXT
    let dim = gl_LaunchSizeEXT
    let origin = rt_camera_origin(pc.angle)
    let dir = rt_camera_dir(origin, float(gid.x), float(gid.y), float(dim.x), float(dim.y))
    prd = float3(0.0, 0.0, 0.0)
    traceRayEXT(rt_tlas, 0x01u, 0xFFu, 0u, 0u, 0u, origin, 0.001, dir, 100.0, prd)
    imageStore(rt_img, int2(int(gid.x), int(gid.y)), float4(prd, 1.0))
}

var @incoming_ray_payload miss_prd : float3

[vulkan_miss_shader(name="raytracing_miss_spv")]
def raytracing_miss {
    miss_prd = sky_color(gl_WorldRayDirectionEXT)
}

var @incoming_ray_payload shadow_prd : float3

[vulkan_miss_shader(name="raytracing_shadow_miss_spv")]
def raytracing_shadow_miss {
    shadow_prd = float3(1.0, 1.0, 1.0)   // the shadow ray reached the light: lit
}

var @incoming_ray_payload hit_prd : float3
var @ray_payload @location = 1 shadow_ray : float3

[vulkan_closest_hit_shader(name="raytracing_chit_spv")]
def raytracing_chit {
    let n = rt_normal(gl_PrimitiveID)
    var albedo = rt_box_albedo()
    if (gl_PrimitiveID < 2) {
        albedo = rt_ground_albedo()
    }
    let p = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT
    // shadow ray: opaque (0x1) + terminate-on-first-hit (0x4) + skip-closest-hit (0x8). No hit
    // shader ever runs -- only the shadow miss (miss_index = 1) flips the payload to lit.
    shadow_ray = float3(0.0, 0.0, 0.0)
    traceRayEXT(rt_tlas, 0x0Du, 0xFFu, 0u, 0u, 1u,
        p + n * 0.001, 0.001, rt_to_light(), 100.0, shadow_ray)
    hit_prd = shade_lambert(n, albedo, shadow_ray.x)
}
