options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 14 -- the Utah teapot, tessellated on the GPU by a mesh-shader pipeline. There is no
// vertex buffer and no index buffer: the 32 bicubic Bezier patches of the classic Newell teapot are
// uploaded once as control points, and the GPU amplifies them into geometry every frame.
//
//   TASK  one workgroup, 32-patch-wide. Decides which patches survive (back-facing patches are
//         culled as the teapot rotates -- the visible proof that the task stage removed geometry on
//         the GPU) and writes the survivor indices into a @task_payload. EmitMeshTasksEXT then
//         dispatches exactly `count` mesh workgroups, one per surviving patch.
//
//   MESH  one workgroup per surviving patch. Reads its patch index from the payload (keyed by
//         gl_WorkGroupID), evaluates the bicubic Bezier surface on a TESS x TESS grid -- on-GPU patch
//         tessellation, a flagship mesh-shader technique -- and emits the grid as (TESS+1)^2 vertices
//         and 2*TESS*TESS triangles, with a finite-difference surface normal per vertex.
//
//   FRAG  view-facing Lambert + rim (robust to per-patch winding), with an optional per-patch debug
//         tint that colors each patch distinctly to show the "1 patch = 1 mesh workgroup" structure.
//
// Every line is daslang, lowered to SPIR-V at compile time by dasSpirv (no glslang). Geometry data
// lives in teapot_data.das; the host (teapot_tut.das) uploads it to the control-point UBO.

module teapot_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind_uniform_* helpers
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

let PATCH_COUNT = 32
let TESS  = 7    // TESS x TESS quad grid per patch
let VERTS = 64   // (TESS+1)^2  -- emitter needs literal-constant globals (SetMeshOutputsEXT operand)
let TRIS  = 98   // 2*TESS^2

// ===== UBO =====

//! One UBO (binding 0), shared by every shader in this module (teapot task/mesh/frag, ground floor,
//! env sky). generate_bind_uniform writes the WHOLE struct from any one reachable shader, so the host
//! calls a single *_bind_uniform per frame and every stage sees the same allocation.
struct TeapotUBO {
    mvp      : float4x4     //!< proj*view*model -- teapot positions
    model    : float4x4     //!< world rotation -- normals + cull facing
    vp       : float4x4     //!< proj*view; the ground floor carries its own world coords, so no model
    light_vp : float4x4     //!< the light's proj*view, for the shadow-map pass
    camera   : float4       //!< .xyz = world eye, .w = debug-tint flag (0/1)
    light    : float4       //!< .xyz = world light direction (the sun; shared by env + shadow)
    cp       : float4[512]  //!< 32 patches x 16 control points (xyz; w pad), patch p = cp[p*16 .. p*16+15], row-major 4x4
}
var @uniform @set = 0 @binding = 0 ubo : TeapotUBO

// ===== task -> mesh payload =====

struct PatchPayload {
    count   : uint
    visible : uint[32]
}
var @task_payload payload : PatchPayload

// ===== mesh -> fragment varyings =====

var @out @location = 0 v_world_normal : array<float3>
var @out @location = 1 v_world_pos    : array<float3>
var @out @location = 2 v_patch        : array<float>

// ===== bicubic Bezier evaluation (helpers shared by task + mesh) =====

//! Evaluate the bicubic Bezier patch at (u,v) in [0,1]^2. base = patch*16. Fully unrolled so the only
//! dynamic index is into the control-point UBO array (base varies per patch).
def eval_patch(base : int; u, v : float) : float3 {
    let su = 1.0 - u
    let sv = 1.0 - v
    let bu0 = su * su * su; let bu1 = 3.0 * su * su * u; let bu2 = 3.0 * su * u * u; let bu3 = u * u * u
    let bv0 = sv * sv * sv; let bv1 = 3.0 * sv * sv * v; let bv2 = 3.0 * sv * v * v; let bv3 = v * v * v
    let r0 = bv0 * ubo.cp[base +  0].xyz + bv1 * ubo.cp[base +  1].xyz + bv2 * ubo.cp[base +  2].xyz + bv3 * ubo.cp[base +  3].xyz
    let r1 = bv0 * ubo.cp[base +  4].xyz + bv1 * ubo.cp[base +  5].xyz + bv2 * ubo.cp[base +  6].xyz + bv3 * ubo.cp[base +  7].xyz
    let r2 = bv0 * ubo.cp[base +  8].xyz + bv1 * ubo.cp[base +  9].xyz + bv2 * ubo.cp[base + 10].xyz + bv3 * ubo.cp[base + 11].xyz
    let r3 = bv0 * ubo.cp[base + 12].xyz + bv1 * ubo.cp[base + 13].xyz + bv2 * ubo.cp[base + 14].xyz + bv3 * ubo.cp[base + 15].xyz
    return bu0 * r0 + bu1 * r1 + bu2 * r2 + bu3 * r3
}

//! Surface normal at (u,v) via central finite differences -- robust and simple (no analytic
//! derivative basis). Model space.
def eval_normal(base : int; u, v : float) : float3 {
    let e = 0.012
    let du = eval_patch(base, min(u + e, 1.0), v) - eval_patch(base, max(u - e, 0.0), v)
    let dv = eval_patch(base, u, min(v + e, 1.0)) - eval_patch(base, u, max(v - e, 0.0))
    let nn = cross(du, dv)
    let len2 = dot(nn, nn)
    // A collapsed control-net row (the lid-knob apex rows are single points) makes one finite-
    // difference zero, so cross() is the zero vector and normalize() would be NaN -- a NaN vertex
    // attribute makes the driver discard the whole primitive (the knob vanished). Fall back instead.
    if (len2 < 1.0e-10) {
        return float3(0.0, 0.0, 1.0)
    }
    return normalize(nn)
}

// ===== TASK: per-patch cull =====

[vulkan_task_shader(local_size_x=1, name="teapot_task_spv")]
def teapot_task {
    // Per-patch backface cull -- the GPU-driven cluster cull: a patch whose world normal faces away is
    // hidden by the front of the teapot, so it is dropped before any mesh workgroup runs and the image
    // is unchanged. A small negative threshold keeps grazing patches so the silhouette stays solid.
    var n = 0u
    for (p in range(PATCH_COUNT)) {
        let base = p * 16
        var c = float3(0.0, 0.0, 0.0)
        for (i in range(16)) {
            c += ubo.cp[base + i].xyz
        }
        c *= 1.0 / 16.0                       // patch center = mean of the control net
        let world_c = (ubo.model * float4(c, 1.0)).xyz
        let outward = normalize(world_c)      // teapot is centered at the origin
        // Patch normal from the control net. A collapsed row (the lid-knob apex is a single point) zeroes
        // one span -> cross() is zero -> normalize() is NaN, and `dot(NaN,..) > t` is false for ANY t, so
        // the knob got culled: fall back to `outward` there, and orient cross(du,dv) outward (winding varies).
        let du = ubo.cp[base + 3].xyz - ubo.cp[base + 0].xyz
        let dv = ubo.cp[base + 12].xyz - ubo.cp[base + 0].xyz
        let raw = cross(du, dv)
        var world_n = outward
        if (dot(raw, raw) > 1.0e-10) {
            world_n = normalize((ubo.model * float4(raw, 0.0)).xyz)
            if (dot(world_n, outward) < 0.0) {
                world_n = -world_n
            }
        }
        let to_cam = normalize(ubo.camera.xyz - world_c)
        if (dot(world_n, to_cam) > -0.92) {
            payload.visible[n] = uint(p)
            n ++
        }
    }
    payload.count = n
    EmitMeshTasksEXT(n, 1u, 1u)
}

// ===== MESH: tessellate one patch =====

[vulkan_mesh_shader(local_size_x=1, max_vertices=64, max_primitives=98, name="teapot_mesh_spv")]
def teapot_mesh {
    SetMeshOutputsEXT(uint(VERTS), uint(TRIS))
    let patch = payload.visible[gl_WorkGroupID.x]
    let base = int(patch) * 16
    // evaluate the (TESS+1)^2 grid of surface points + normals
    for (iu in range(TESS + 1)) {
        for (iv in range(TESS + 1)) {
            let u = float(iu) / float(TESS)
            let v = float(iv) / float(TESS)
            let pos = eval_patch(base, u, v)
            let nrm = eval_normal(base, u, v)
            let idx = iu * (TESS + 1) + iv
            gl_MeshVerticesEXT[idx].gl_Position = ubo.mvp * float4(pos, 1.0)
            v_world_normal[idx] = normalize((ubo.model * float4(nrm, 0.0)).xyz)
            v_world_pos[idx] = (ubo.model * float4(pos, 1.0)).xyz
            v_patch[idx] = float(patch)
        }
    }
    // emit two triangles per grid quad
    var t = 0
    for (iu in range(TESS)) {
        for (iv in range(TESS)) {
            let a = iu * (TESS + 1) + iv
            let b = a + 1
            let c = a + (TESS + 1)
            let d = c + 1
            gl_PrimitiveTriangleIndicesEXT[t] = uint3(uint(a), uint(b), uint(c))
            t ++
            gl_PrimitiveTriangleIndicesEXT[t] = uint3(uint(b), uint(d), uint(c))
            t ++
        }
    }
}

// ===== FRAGMENT =====

var @in @location = 0 f_world_normal : float3
var @in @location = 1 f_world_pos    : float3
var @in @location = 2 f_patch        : float
var @out @location = 0 frag_color    : float4

//! Procedural environment sampled by a direction -- what the glazed teapot reflects. Warm horizon ->
//! cool zenith, a darker ground hemisphere, plus a sun disc + glow along the light dir, so the reflected
//! sun rides the surface as it turns and lines up with the specular. Analytic; no cubemap needed.
def env_color(dir : float3) : float3 {
    let up = clamp(dir.y, -1.0, 1.0)
    let t = clamp(up * 0.5 + 0.5, 0.0, 1.0)
    var c = lerp(float3(0.90, 0.80, 0.70), float3(0.32, 0.48, 0.82), float3(t, t, t))
    if (up < 0.0) {
        let g = clamp(-up * 1.5, 0.0, 1.0)
        c = lerp(c, float3(0.18, 0.15, 0.13), float3(g, g, g))
    }
    let sun_dir = normalize(ubo.light.xyz)
    let sd = max(dot(dir, sun_dir), 0.0)
    c = c + float3(1.0, 0.92, 0.78) * (pow(sd, 250.0) * 3.0)   // bright sun disc
    c = c + float3(1.0, 0.88, 0.70) * (pow(sd, 8.0) * 0.4)     // soft glow
    return c
}

[vulkan_fragment_shader(name="teapot_frag_spv")]
def teapot_fs {
    var n = normalize(f_world_normal)
    let view = normalize(ubo.camera.xyz - f_world_pos)
    if (dot(n, view) < 0.0) {            // view-facing: robust to per-patch winding
        n = -n
    }
    let l = normalize(ubo.light.xyz)

    // reflect the view about the surface normal and sample the procedural environment
    let inc = float3(-view.x, -view.y, -view.z)
    let rdir = inc - n * (2.0 * dot(n, inc))
    let env = env_color(rdir)

    // glazed ceramic: warm diffuse base, Fresnel-weighted environment reflection, tight glossy
    // highlight. Fresnel makes the rim mirror-bright; the centre keeps a base sheen.
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3
    let base = float3(0.82, 0.50, 0.26) * key
    let fres = pow(1.0 - max(dot(n, view), 0.0), 4.0)
    let reflectivity = 0.18 + 0.62 * fres
    let half = normalize(l + view)
    let spec = pow(max(dot(n, half), 0.0), 120.0) * 1.3
    var col = lerp(base, env, float3(reflectivity, reflectivity, reflectivity))
    col = col + float3(spec, spec, spec)

    if (ubo.camera.w > 0.5) {             // per-patch debug tint
        let h = fract(f_patch * 0.137)
        let tint = float3(fract(h + 0.0), fract(h + 0.33), fract(h + 0.66))
        col = lerp(col, tint, float3(0.55, 0.55, 0.55))   // splat t: dasSpirv FMix needs a vec t
    }
    frag_color = float4(col, 1.0)
}

// ===== SHADOW MAP SAMPLER =====

// The depth-compare combined sampler. dasSpirv emits OpTypeImage with the Depth=1 flag (the
// `sampler2DShadow` marker) and textureCompare lowers to OpImageSampleDrefImplicitLod -> scalar PCF
// in [0,1] (1 = lit, 0 = shadowed). The floor reads it to receive the teapot's cast silhouette.
var @uniform @set = 0 @binding = 1 teapot_shadow_map : sampler2DShadow

let SHADOW_MAP_PX = 1024.0

//! 3x3 PCF: each textureCompare is already hardware-PCF'd over its 2x2 footprint, so 9 taps give a
//! soft, stable shadow edge cheaply.
def private pcf_shadow(uv : float2; ref : float) : float {
    let texel = 1.0 / SHADOW_MAP_PX
    var sum = 0.0
    for (j in range(-1, 2)) {
        for (i in range(-1, 2)) {
            let off = float2(float(i), float(j)) * texel
            sum += textureCompare(teapot_shadow_map, uv + off, ref)
        }
    }
    return sum * (1.0 / 9.0)
}

// ===== GROUND FLOOR =====

// A world-space XZ plane the teapot sits on: its own regular vertex+fragment pipeline, drawn in the
// same color pass as the mesh-shader teapot. It reads `ubo.vp` (no model — the quad is already in
// world space), reflects the SAME env the teapot does, and receives its shadow via sampler2DShadow.

var @in  @location = 0 fl_pos   : float3
var @out @location = 0 fl_world : float3

[vulkan_vertex_shader(name="floor_vert_spv")]
def floor_vs {
    fl_world = fl_pos
    gl_Position = ubo.vp * float4(fl_pos, 1.0)
}

var @in  @location = 0 ff_world : float3
var @out @location = 0 floor_color : float4

[vulkan_fragment_shader(name="floor_frag_spv")]
def floor_fs {
    let n = float3(0.0, 1.0, 0.0)
    // checkerboard via cell parity (gx+gz) mod 2 — bigger cells (x0.6) keep far-field aliasing tame.
    let gx = floor(ff_world.x * 0.6 + 1000.0)
    let gz = floor(ff_world.z * 0.6 + 1000.0)
    let cell = gx + gz
    let parity = cell - floor(cell * 0.5) * 2.0
    let checker = parity < 0.5 ? 0.58 : 0.20
    let l = normalize(ubo.light.xyz)
    let ndotl = max(dot(n, l), 0.0)

    // shadow lookup: project the floor fragment into the light's clip space, remap NDC.xy [-1,1] to
    // the [0,1] shadow-map UV (Vulkan NDC.z is already [0,1], used as the compare reference).
    let lc = ubo.light_vp * float4(ff_world, 1.0)
    let lw = max(lc.w, 0.0001)
    let lndc = float3(lc.x / lw, lc.y / lw, lc.z / lw)
    let suv = float2(lndc.x * 0.5 + 0.5, lndc.y * 0.5 + 0.5)
    let ref = clamp(lndc.z - 0.0015, 0.0, 1.0)        // small constant receiver bias
    var lit = pcf_shadow(suv, ref)
    if (ndotl < 0.05) { lit = 0.0 }

    // ambient + shadowed key light
    var col = float3(checker, checker, checker) * (0.45 + ndotl * 0.7 * lit)
    // polished sheen: Fresnel-weighted reflection of the env about the up normal
    let view = normalize(ubo.camera.xyz - ff_world)
    let inc = float3(-view.x, -view.y, -view.z)
    let rdir = inc - n * (2.0 * dot(n, inc))
    let env = env_color(rdir)
    let fres = pow(1.0 - max(dot(n, view), 0.0), 4.0) * 0.6
    col = lerp(col, env, float3(fres, fres, fres))
    // dissolve the far floor into the sky so the slab has no hard edge and the checker never reaches
    // the aliasing-prone horizon. Horizon colour = env sampled along the (flattened) view ray.
    let horizon = env_color(normalize(float3(-view.x, 0.04, -view.z)))
    let d = length(float2(ff_world.x, ff_world.z))
    let fade = clamp((d - 4.5) * 0.14, 0.0, 1.0)
    col = lerp(col, horizon, float3(fade, fade, fade))
    floor_color = float4(col, 1.0)
}

// ===== ENV SKY =====

// A fullscreen triangle drawn first (depth test off) so the background shows the same procedural env
// the teapot + floor reflect — a scene, not a void. The host uploads the three corner view-rays from
// the camera basis as a per-vertex attribute; the fragment normalizes it and evaluates env_color.

var @in  @location = 0 sky_clip : float2
var @in  @location = 1 sky_ray  : float3
var @out @location = 0 sky_v_ray : float3

[vulkan_vertex_shader(name="sky_vert_spv")]
def sky_vs {
    sky_v_ray = sky_ray
    gl_Position = float4(sky_clip.x, sky_clip.y, 1.0, 1.0)   // z = w = 1 -> far plane
}

var @in  @location = 0 sky_f_ray : float3
var @out @location = 0 sky_color : float4

[vulkan_fragment_shader(name="sky_frag_spv")]
def sky_fs {
    sky_color = float4(env_color(normalize(sky_f_ray)), 1.0)
}

// ===== SHADOW CASTER (mesh-shader, depth-only) =====

// The teapot casts its TRUE silhouette: the same on-GPU Bezier tessellation, rendered depth-only from
// the light's view into the shadow map (pass 1). The task emits ALL patches — a caster must keep the
// camera-facing-away ones, they still occlude — and the mesh writes only clip = light_vp*model*pos.

[vulkan_task_shader(local_size_x=1, name="teapot_shadow_task_spv")]
def teapot_shadow_task {
    for (p in range(PATCH_COUNT)) {
        payload.visible[p] = uint(p)
    }
    payload.count = uint(PATCH_COUNT)
    EmitMeshTasksEXT(uint(PATCH_COUNT), 1u, 1u)
}

[vulkan_mesh_shader(local_size_x=1, max_vertices=64, max_primitives=98, name="teapot_shadow_mesh_spv")]
def teapot_shadow_mesh {
    SetMeshOutputsEXT(uint(VERTS), uint(TRIS))
    let patch = payload.visible[gl_WorkGroupID.x]
    let base = int(patch) * 16
    for (iu in range(TESS + 1)) {
        for (iv in range(TESS + 1)) {
            let u = float(iu) / float(TESS)
            let v = float(iv) / float(TESS)
            let pos = eval_patch(base, u, v)
            let idx = iu * (TESS + 1) + iv
            gl_MeshVerticesEXT[idx].gl_Position = ubo.light_vp * ubo.model * float4(pos, 1.0)
        }
    }
    var t = 0
    for (iu in range(TESS)) {
        for (iv in range(TESS)) {
            let a = iu * (TESS + 1) + iv
            let b = a + 1
            let c = a + (TESS + 1)
            let d = c + 1
            gl_PrimitiveTriangleIndicesEXT[t] = uint3(uint(a), uint(b), uint(c))
            t ++
            gl_PrimitiveTriangleIndicesEXT[t] = uint3(uint(b), uint(d), uint(c))
            t ++
        }
    }
}
