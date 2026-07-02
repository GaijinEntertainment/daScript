options gen2
options indenting = 4

// GPU entry points for the one-source path tracer. The kernels call the SAME pt_core functions the
// CPU engines run; dasSpirv lowers them (shared helpers included) to SPIR-V at compile time.

module pt_shaders public

require math
require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public

require pt_core public

// set 0: binding 0 = rgba32f accumulation, binding 1 = rgba8 display; pt_core's scene SSBOs sit at
// bindings 2 (verts) and 3 (tris)
var @binding = 0 @format = "rgba32f" pt_accum : image2D
var @binding = 1 pt_display : image2D

struct PtPush {
    frame : uint
}
var @push_constant pt_push : PtPush

[vulkan_compute_shader(local_size_x=8, local_size_y=8, name="pt_compute_spv")]
def pt_compute {
    let px = int2(gl_GlobalInvocationID.xy)
    let dim = imageSize(pt_display)
    if (px.x >= dim.x || px.y >= dim.y) {
        return
    }
    let frame = int(pt_push.frame)
    let c = pt_render_pixel(px.x, px.y, frame, dim.x, dim.y)
    var acc = imageLoad(pt_accum, px).xyz
    if (frame == 0) {
        acc = float3(0.0)
    }
    acc += c
    imageStore(pt_accum, px, float4(acc, 1.0))
    imageStore(pt_display, px, float4(pt_tonemap(acc / float(frame + 1)), 1.0))
}

// ===== hardware ray tracing: traversal replaces pt_intersect, everything else is the same code =====
//
// The raygen mirrors pt_trace_path bounce-for-bounce — camera, RNG, NEE light sampling, materials,
// normals, accumulation all come from pt_core — but each intersection is a traceRayEXT into the
// TLAS built from the same pt_verts/pt_tris the software intersector loops over. The closest-hit
// and miss shaders are minimal reporters (t + primitive id in the payload); the shadow ray uses
// terminate-on-first-hit + skip-closest-hit with miss_index 1, so only the shadow miss can flip
// its payload to "lit" (tutorial-15 pattern, recursion-free at max_recursion = 1).

var @binding = 4 pt_tlas : accelerationStructureEXT

var @ray_payload pt_hit : float4                    // x = t, y = primitive id, z = hit flag
var @ray_payload @location = 1 pt_shadow : float3   // x = lit flag (0 until the shadow miss runs)

[vulkan_raygen_shader(name="pt_rgen_spv")]
def pt_rgen {
    let gid = gl_LaunchIDEXT
    let dim = gl_LaunchSizeEXT
    let px = int2(gid.xy)
    let frame = int(pt_push.frame)
    var rng = pt_seed(px.x, px.y, frame)
    let u = (float(px.x) + pt_rng(rng)) / float(dim.x)
    let v = (float(px.y) + pt_rng(rng)) / float(dim.y)
    var ro = float3(0.0)
    var rd = float3(0.0)
    pt_camera_ray(u, v, ro, rd)
    var throughput = float3(1.0)
    var radiance = float3(0.0)
    var specular = true
    for (bounce in range(PT_MAX_BOUNCES)) {
        pt_hit = float4(0.0)
        traceRayEXT(pt_tlas, 0x01u, 0xFFu, 0u, 0u, 0u, ro, PT_MIN_T, rd, PT_MAX_T, pt_hit)
        if (pt_hit.z == 0.0) {
            break
        }
        let t = pt_hit.x
        let prim = int(pt_hit.y)
        let mat = pt_tris[prim].w
        if (mat == PT_MAT_LIGHT) {
            if (specular) {
                radiance += throughput * pt_mat_emission(mat)
            }
            break
        }
        let hitp = ro + rd * t
        let n = pt_tri_normal(prim, rd)
        if (pt_mat_is_mirror(mat)) {
            rd = reflect(rd, n)
            ro = hitp + n * 0.001
            specular = true
            continue
        }
        let albedo = pt_mat_albedo(mat)
        let lp = pt_light_sample(rng)
        let direct = pt_light_term(hitp, n, lp)
        if (direct.x > 0.0 || direct.y > 0.0) {
            let ld = lp - hitp
            let dist = length(ld)
            pt_shadow = float3(0.0)
            // opaque + terminate-on-first-hit + skip-closest-hit; tmax stops just short of the
            // light quad, so any hit is an occluder and only the shadow miss can set lit
            traceRayEXT(pt_tlas, 0x0Du, 0xFFu, 0u, 0u, 1u, hitp + n * 0.001, 0.001, ld / dist, dist - 0.01, pt_shadow)
            if (pt_shadow.x > 0.5) {
                radiance += throughput * albedo * direct
            }
        }
        throughput *= albedo
        rd = pt_cosine_dir(n, rng)
        ro = hitp + n * 0.001
        specular = false
    }
    var acc = imageLoad(pt_accum, px).xyz
    if (frame == 0) {
        acc = float3(0.0)
    }
    acc += radiance
    imageStore(pt_accum, px, float4(acc, 1.0))
    imageStore(pt_display, px, float4(pt_tonemap(acc / float(frame + 1)), 1.0))
}

var @incoming_ray_payload pt_miss_in : float4

[vulkan_miss_shader(name="pt_miss_spv")]
def pt_miss {
    pt_miss_in = float4(0.0)    // z = 0: no hit
}

var @incoming_ray_payload pt_shadow_in : float3

[vulkan_miss_shader(name="pt_shadow_miss_spv")]
def pt_shadow_miss {
    pt_shadow_in = float3(1.0)  // the shadow ray reached the light: lit
}

var @incoming_ray_payload pt_chit_in : float4

[vulkan_closest_hit_shader(name="pt_chit_spv")]
def pt_chit {
    pt_chit_in = float4(gl_HitTEXT, float(gl_PrimitiveID), 1.0, 0.0)
}

// ===== display: fullscreen triangle sampling the display image (letterboxed by `scale`) =====

struct PtQuadPush {
    scale : float2
}
var @push_constant pt_qpush : PtQuadPush

var @out @location = 0 pt_quad_uv : float2

[vulkan_vertex_shader(name="pt_quad_vert_spv")]
def pt_quad_vs {
    let vi = gl_VertexIndex
    let u = float((vi << 1) & 2)
    let v = float(vi & 2)
    pt_quad_uv = float2(u, v)
    gl_Position = float4((u * 2.0 - 1.0) * pt_qpush.scale.x, (v * 2.0 - 1.0) * pt_qpush.scale.y, 0.0, 1.0)
}

var @in @location = 0 pt_quad_uv_in : float2
var @out @location = 0 pt_quad_color : float4
var @binding = 0 pt_tex : sampler2D

[vulkan_fragment_shader(name="pt_quad_frag_spv")]
def pt_quad_fs {
    pt_quad_color = texture(pt_tex, pt_quad_uv_in)
}
