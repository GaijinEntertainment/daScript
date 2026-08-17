options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 06 -- a procedural skybox via a cubemap. The vertex shader emits one cube vertex per draw
// vertex; the cube position IS the sampling direction (a normalized unit cube around the camera) so
// the fragment shader can sample the cubemap with the world direction without a separate UV. Two
// rails are exercised:
//
//   1. ``samplerCube`` and its ``texture(samplerCube, vec3)`` sampling overload -- the same
//      ``texture()`` builtin tutorial 04 used for ``sampler2D``, but now with a 3-vector direction
//      instead of a 2D UV. dasSpirv lowers it to ``OpImageSampleImplicitLod`` against an
//      ``OpTypeImage`` with ``Dim = Cube``.
//
//   2. The **depth=1.0 trick** for skyboxes. After the projection multiply we snap ``clip.z = clip.w``
//      so the post-divide NDC ``z`` is exactly ``1.0``: every skybox fragment lives at the far
//      plane. Paired with a ``LESS_OR_EQUAL`` depth test, the skybox passes only where the depth
//      buffer is still at its cleared 1.0 -- so once tutorial 07 (or a later combined scene) lays
//      down foreground geometry in the same render pass, the skybox automatically fills the gaps
//      without overdrawing what's in front of it. This tutorial draws skybox only; the trick is set
//      up here so the next tutorial can drop foreground geometry into the same pipeline with no
//      changes.

module skybox_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind_uniform helper
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== shared UBO =====

//! Rotation-only view matrix (translation stripped) plus the perspective projection. The
//! translation strip keeps the skybox cube centred on the camera -- the skybox appears at
//! infinity, no matter where the camera moves.
struct Camera {
    view      : float4x4   // rotation-only view (keeps the skybox glued to the camera)
    proj      : float4x4
    view_full : float4x4   // full view incl. translation (the sphere + floor live in the world)
    cam_pos   : float4     // xyz = world camera position (for the reflection view vector)
    material  : float4     // rgb = metal tint, a = reflectivity — cycled over time for variety
}
var @uniform @set = 0 @binding = 0 cam : Camera

// ===== cubemap binding =====

//! 6-face cubemap of the procedural sky (sun + gradient + horizon). VK_IMAGE_VIEW_TYPE_CUBE on the
//! host side; ``samplerCube`` here makes dasSpirv emit an ``OpTypeImage`` with ``Dim=Cube``.
var @set = 0 @binding = 1 sky_tex : samplerCube

// ===== vertex shader =====

var @in @location = 0 a_pos : float3
var @out @location = 0 v_dir : float3

[vulkan_vertex_shader(name="skybox_vert_spv")]
def skybox_vs {
    // Pass the local cube position as the per-fragment sampling direction. Because we render a unit
    // cube centred at the origin and the view is rotation-only, the position is the world-space
    // direction from camera to vertex.
    v_dir = a_pos
    // Standard rotation+projection. Then snap z to w so the post-divide NDC z = 1.0 (the far plane).
    // dasSpirv does not support write-swizzle (clip.z = clip.w), so rebuild the vec4 via constructor.
    let clip = cam.proj * cam.view * float4(a_pos, 1.0)
    gl_Position = float4(clip.x, clip.y, clip.w, clip.w)
}

// ===== fragment shader =====

var @in @location = 0 f_dir : float3
var @out @location = 0 frag_color : float4

[vulkan_fragment_shader(name="skybox_frag_spv")]
def skybox_fs {
    // ``f_dir`` is the interpolated cube position. Normalize to get the unit direction the cubemap
    // expects; the cube face + intra-face UV are computed by the sampler from this direction.
    let col = texture(sky_tex, normalize(f_dir)).rgb
    frag_color = float4(col, 1.0)
}

// ===== reflective sphere + floor =====
// Both live in the world (full view, not the rotation-only skybox view) and sample the SAME cubemap
// the skybox renders. One shared vertex shader; the fragment shaders diverge (metal vs checkerboard).

var @in @location = 1 a_normal : float3          // sphere/floor vertex normal (skybox ignores it)
var @out @location = 1 v_world_pos    : float3
var @out @location = 2 v_world_normal : float3

[vulkan_vertex_shader(name="scene_vert_spv")]
def scene_vs {
    v_world_pos = a_pos
    v_world_normal = a_normal
    gl_Position = cam.proj * cam.view_full * float4(a_pos, 1.0)
}

var @in @location = 1 f_world_pos    : float3
var @in @location = 2 f_world_normal : float3

//! GLSL-style reflect(I, N) = I - 2*dot(N,I)*N, with I the incident (eye->surface) direction.
def private reflect_dir(i, n : float3) : float3 {
    return i - n * (2.0 * dot(n, i))
}

//! Mirror-metal sphere. Reflect the view about the surface normal, sample the cubemap, and tint by
//! the cycled ``material`` (Schlick metal Fresnel: tinted reflectance at normal incidence ramping to
//! white at grazing). The host morphs ``material`` chrome -> gold -> copper -> steel over time.
[vulkan_fragment_shader(name="sphere_frag_spv")]
def sphere_fs {
    let n = normalize(f_world_normal)
    let v = normalize(float3(cam.cam_pos.x - f_world_pos.x, cam.cam_pos.y - f_world_pos.y,
                             cam.cam_pos.z - f_world_pos.z))
    let ndotv = max(dot(n, v), 0.0)
    let env = texture(sky_tex, reflect_dir(float3(-v.x, -v.y, -v.z), n)).rgb
    let tint = float3(cam.material.x, cam.material.y, cam.material.z)
    let f0 = tint * cam.material.w
    let fres = pow(1.0 - ndotv, 5.0)
    let reflectance = lerp(f0, float3(1.0, 1.0, 1.0), float3(fres, fres, fres))
    let col = env * reflectance
    frag_color = float4(col.x, col.y, col.z, 1.0)
}

//! Checkerboard floor with a faint Fresnel sky reflection so it reads as a polished surface grounding
//! the sphere (not a flat plane floating in the sky).
[vulkan_fragment_shader(name="floor_frag_spv")]
def floor_fs {
    let n = normalize(f_world_normal)
    let gx = floor(f_world_pos.x * 0.5 + 100.0)
    let gz = floor(f_world_pos.z * 0.5 + 100.0)
    // true checkerboard: cell parity (gx+gz) mod 2, via floor (no mod builtin needed)
    let cell = gx + gz
    let parity = cell - floor(cell * 0.5) * 2.0          // 0.0 = even cell, 1.0 = odd cell
    let checker = parity < 0.5 ? 0.85 : 0.28
    let base = float3(checker, checker, checker) * float3(0.72, 0.74, 0.80)
    let v = normalize(float3(cam.cam_pos.x - f_world_pos.x, cam.cam_pos.y - f_world_pos.y,
                             cam.cam_pos.z - f_world_pos.z))
    let env = texture(sky_tex, reflect_dir(float3(-v.x, -v.y, -v.z), n)).rgb
    let fres = pow(1.0 - max(dot(n, v), 0.0), 4.0) * 0.4
    let col = lerp(base, env, float3(fres, fres, fres))
    frag_color = float4(col.x, col.y, col.z, 1.0)
}
