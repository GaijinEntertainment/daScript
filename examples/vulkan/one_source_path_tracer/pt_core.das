options gen2
options indenting = 4

// One-source path tracer core. The SAME functions below run on the CPU (interpreter or JIT,
// single- or multi-threaded) and lower to SPIR-V (compute and ray-tracing stages) via dasSpirv.
// GPU-lowerable authoring rules observed throughout: 32-bit scalar/vector params only (no
// struct/array params), no recursion, stateless PCG RNG threaded by ref, scene data in @ssbo
// globals that the CPU fills and reads directly while the GPU binds them as storage buffers.

module pt_core public

options solid_context

require math

// ===== scene data — one source: CPU fills + reads these, GPU binds them as SSBOs =====

var @ssbo @binding = 2 pt_verts : array<float4>     // xyz = position (float4 => std430 stride matches CPU)
var @ssbo @binding = 3 pt_tris : array<uint4>       // xyz = vertex indices, w = material id

let PT_TRI_COUNT = 32                               // 6 quads + 2 boxes * 5 faces; verified by pt_init_scene

let {
    PT_MIN_T = 0.001
    PT_MAX_T = 1.0e7
    PT_MAX_BOUNCES = 5
}

let {
    PT_MAT_WHITE = 0u
    PT_MAT_RED = 1u
    PT_MAT_GREEN = 2u
    PT_MAT_LIGHT = 3u
    PT_MAT_MIRROR = 4u
}

// ceiling light quad: x in [-0.35,0.35], z in [0.75,1.45] at y=1.995, normal (0,-1,0)
let {
    PT_LIGHT_Y = 1.995
    PT_LIGHT_X0 = -0.35
    PT_LIGHT_X1 = 0.35
    PT_LIGHT_Z0 = 0.75
    PT_LIGHT_Z1 = 1.45
    PT_LIGHT_AREA = 0.49
}

// ===== materials as code — shared by every engine, no material buffer to marshal =====

def pt_mat_albedo(m : uint) : float3 {
    if (m == PT_MAT_RED) {
        return float3(0.63, 0.065, 0.05)
    }
    if (m == PT_MAT_GREEN) {
        return float3(0.14, 0.45, 0.091)
    }
    if (m == PT_MAT_MIRROR) {
        return float3(0.95, 0.95, 0.95)
    }
    return float3(0.73, 0.73, 0.73)
}

def pt_mat_emission(m : uint) : float3 {
    if (m == PT_MAT_LIGHT) {
        return float3(17.0, 12.0, 6.0)
    }
    return float3(0.0)
}

def pt_mat_is_mirror(m : uint) : bool => m == PT_MAT_MIRROR

// ===== stateless PCG RNG — identical streams on CPU and GPU =====

def pt_rng_uint(var state : uint&) : uint {
    state = state * 747796405u + 2891336453u
    let word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u
    return (word >> 22u) ^ word
}

def pt_rng(var state : uint&) : float {
    return float(pt_rng_uint(state) >> 8u) * (1.0 / 16777216.0)
}

def pt_seed(x, y, frame : int) : uint {
    var state = (uint(x) * 1973u + uint(y) * 9277u + uint(frame) * 26699u) | 1u
    return pt_rng_uint(state) | 1u
}

// ===== pinhole camera looking down +Z into the box front opening (square aspect) =====

def pt_camera_ray(u, v : float; var ro : float3&; var rd : float3&) {
    ro = float3(0.0, 1.0, -3.0)
    rd = normalize(float3((u * 2.0 - 1.0) * 0.35, (1.0 - v * 2.0) * 0.35, 1.0))
}

// ===== intersection — Moller-Trumbore over the shared triangle list =====

def pt_hit_tri(ro, rd : float3; v0, v1, v2 : float3; tmax : float; var t : float&) : bool {
    let e1 = v1 - v0
    let e2 = v2 - v0
    let pv = cross(rd, e2)
    let det = dot(e1, pv)
    if (abs(det) < 1e-8) {
        return false
    }
    let inv = 1.0 / det
    let tv = ro - v0
    let u = dot(tv, pv) * inv
    if (u < 0.0 || u > 1.0) {
        return false
    }
    let qv = cross(tv, e1)
    let v = dot(rd, qv) * inv
    if (v < 0.0 || u + v > 1.0) {
        return false
    }
    let tt = dot(e2, qv) * inv
    if (tt < PT_MIN_T || tt > tmax) {
        return false
    }
    t = tt
    return true
}

def pt_intersect(ro, rd : float3; var t : float&; var prim : int&) : bool {
    t = PT_MAX_T
    prim = -1
    for (i in range(PT_TRI_COUNT)) {
        let tri = pt_tris[i]
        let v0 = pt_verts[int(tri.x)].xyz
        let v1 = pt_verts[int(tri.y)].xyz
        let v2 = pt_verts[int(tri.z)].xyz
        var tt = 0.0
        if (pt_hit_tri(ro, rd, v0, v1, v2, t, tt)) {
            t = tt
            prim = i
        }
    }
    return prim >= 0
}

def pt_tri_normal(prim : int; rd : float3) : float3 {
    let tri = pt_tris[prim]
    let v0 = pt_verts[int(tri.x)].xyz
    let v1 = pt_verts[int(tri.y)].xyz
    let v2 = pt_verts[int(tri.z)].xyz
    var n = normalize(cross(v1 - v0, v2 - v0))
    if (dot(n, rd) > 0.0) {
        n = -n
    }
    return n
}

// ===== sampling =====

def pt_sphere_dir(var rng : uint&) : float3 {
    let z = 1.0 - 2.0 * pt_rng(rng)
    let phi = 6.2831853 * pt_rng(rng)
    let r = sqrt(max(0.0, 1.0 - z * z))
    return float3(r * cos(phi), r * sin(phi), z)
}

def pt_cosine_dir(n : float3; var rng : uint&) : float3 {
    let d = n + pt_sphere_dir(rng)
    if (dot(d, d) < 1e-6) {
        return n
    }
    return normalize(d)
}

// ===== next-event estimation — shared light sampling; only the occlusion query is per-engine =====

def pt_light_sample(var rng : uint&) : float3 {
    let lx = lerp(PT_LIGHT_X0, PT_LIGHT_X1, pt_rng(rng))
    let lz = lerp(PT_LIGHT_Z0, PT_LIGHT_Z1, pt_rng(rng))
    return float3(lx, PT_LIGHT_Y, lz)
}

//! Unshadowed direct-light term for a lambert surface at hitp with normal n toward light point lp:
//! Le * cos_surf * cos_light * area / (pi * d^2). Multiply by albedo * visibility at the call site.
def pt_light_term(hitp, n, lp : float3) : float3 {
    let ld = lp - hitp
    let d2 = dot(ld, ld)
    let ldir = ld * rsqrt(d2)
    let cosS = dot(n, ldir)
    let cosL = ldir.y   // light normal is (0,-1,0); incoming dir at light is -ldir
    if (cosS <= 0.0 || cosL <= 0.0) {
        return float3(0.0)
    }
    return pt_mat_emission(PT_MAT_LIGHT) * (cosS * cosL * PT_LIGHT_AREA / (PI * d2))
}

// ===== integrator — iterative, mirror + lambert, NEE + emissive on specular paths =====

def pt_trace_path(ro0, rd0 : float3; var rng : uint&) : float3 {
    var ro = ro0
    var rd = rd0
    var throughput = float3(1.0)
    var radiance = float3(0.0)
    var specular = true
    for (_bounce in range(PT_MAX_BOUNCES)) {
        var t = 0.0
        var prim = -1
        if (!pt_intersect(ro, rd, t, prim)) {
            break
        }
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
        // next-event estimation: sample the light, shadow-test with the shared intersector
        let lp = pt_light_sample(rng)
        let direct = pt_light_term(hitp, n, lp)
        if (direct.x > 0.0 || direct.y > 0.0) {
            var st = 0.0
            var sprim = -1
            if (pt_intersect(hitp + n * 0.001, normalize(lp - hitp), st, sprim)) {
                if (pt_tris[sprim].w == PT_MAT_LIGHT) {
                    radiance += throughput * albedo * direct
                }
            }
        }
        throughput *= albedo
        rd = pt_cosine_dir(n, rng)
        ro = hitp + n * 0.001
        specular = false
    }
    return radiance
}

// ===== per-pixel entry — 1 sample; every engine accumulates these =====

def pt_render_pixel(x, y, frame, width, height : int) : float3 {
    var rng = pt_seed(x, y, frame)
    let u = (float(x) + pt_rng(rng)) / float(width)
    let v = (float(y) + pt_rng(rng)) / float(height)
    var ro = float3(0.0)
    var rd = float3(0.0)
    pt_camera_ray(u, v, ro, rd)
    return pt_trace_path(ro, rd, rng)
}

def pt_tonemap(c : float3) : float3 {
    let m = c / (c + float3(1.0))
    return pow(m, float3(1.0 / 2.2))
}

// ===== scene construction — host/CPU only (never lowered; shaders read the arrays it fills) =====

def private pt_push_quad(a, b, c, d : float3; mat : uint) {
    let base = uint(length(pt_verts))
    pt_verts |> push(float4(a, 0.0))
    pt_verts |> push(float4(b, 0.0))
    pt_verts |> push(float4(c, 0.0))
    pt_verts |> push(float4(d, 0.0))
    pt_tris |> push(uint4(base, base + 1u, base + 2u, mat))
    pt_tris |> push(uint4(base, base + 2u, base + 3u, mat))
}

def private pt_push_box(cx, cz : float; hx, height, hz : float; angle : float; mat : uint) {
    let ca = cos(angle)
    let sa = sin(angle)
    let lx = fixed_array(-hx, hx, hx, -hx)
    let lz = fixed_array(-hz, -hz, hz, hz)
    var b : float3[4]
    var tp : float3[4]
    for (i in range(4)) {
        let wx = cx + lx[i] * ca - lz[i] * sa
        let wz = cz + lx[i] * sa + lz[i] * ca
        b[i] = float3(wx, 0.0, wz)
        tp[i] = float3(wx, height, wz)
    }
    for (i in range(4)) {
        let j = (i + 1) & 3
        pt_push_quad(b[i], b[j], tp[j], tp[i], mat)
    }
    pt_push_quad(tp[0], tp[1], tp[2], tp[3], mat)
}

//! Builds the Cornell scene into pt_verts/pt_tris. Idempotent per context — every worker thread
//! (own context, own globals) calls it before tracing; the host also uploads these arrays to the
//! GPU engines and feeds them to the BLAS build.
def pt_init_scene {
    if (!empty(pt_tris)) {
        return
    }
    // interior: x in [-1,1], y in [0,2], z in [0,2]; camera looks in from z < 0
    pt_push_quad(float3(-1.0, 0.0, 0.0), float3(1.0, 0.0, 0.0), float3(1.0, 0.0, 2.0), float3(-1.0, 0.0, 2.0), PT_MAT_WHITE)            // floor
    pt_push_quad(float3(-1.0, 2.0, 0.0), float3(1.0, 2.0, 0.0), float3(1.0, 2.0, 2.0), float3(-1.0, 2.0, 2.0), PT_MAT_WHITE)            // ceiling
    pt_push_quad(float3(-1.0, 0.0, 2.0), float3(1.0, 0.0, 2.0), float3(1.0, 2.0, 2.0), float3(-1.0, 2.0, 2.0), PT_MAT_WHITE)            // back wall
    pt_push_quad(float3(-1.0, 0.0, 0.0), float3(-1.0, 0.0, 2.0), float3(-1.0, 2.0, 2.0), float3(-1.0, 2.0, 0.0), PT_MAT_RED)            // left wall
    pt_push_quad(float3(1.0, 0.0, 0.0), float3(1.0, 0.0, 2.0), float3(1.0, 2.0, 2.0), float3(1.0, 2.0, 0.0), PT_MAT_GREEN)              // right wall
    pt_push_quad(float3(-0.35, 1.995, 0.75), float3(0.35, 1.995, 0.75), float3(0.35, 1.995, 1.45), float3(-0.35, 1.995, 1.45), PT_MAT_LIGHT)
    pt_push_box(-0.38, 1.3, 0.28, 1.15, 0.28, 0.65, PT_MAT_MIRROR)  // tall mirror box, ~37 deg so it reflects the room
    pt_push_box(0.4, 0.68, 0.28, 0.55, 0.28, -0.3, PT_MAT_WHITE)    // short white box, ~-17 deg
    verify(length(pt_tris) == PT_TRI_COUNT)
}

// ===== CPU band renderer — host only =====

def pt_render_rows(frame, width, height, ymin, ymax : int; var accum : array<float3>) {
    for (y in range(ymin, ymax)) {
        for (x in range(width)) {
            accum[y * width + x] += pt_render_pixel(x, y, frame, width, height)
        }
    }
}
