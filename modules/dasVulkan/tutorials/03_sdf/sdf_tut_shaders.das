options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 03 - a signed-distance-field raymarcher in pure daslang. A [compute_shader] fires one
// invocation per pixel: build a ray from the pixel's screen position, march it through a scene
// defined by smooth-blended distance functions (a sphere + an orbiting torus floating above a
// checkered plane), shade the hit with key + ambient + soft shadow + atmospheric fog, then write
// RGBA8 to a storage image. No vertex buffer, no triangles, no textures -- the whole scene is math.
// Same compute path as the Mandelbrot zoom (storage image + push-constant time + per-frame redispatch);
// the novelty is the shader. Authored in daslang, lowered to SPIR-V at compile time by dasSpirv --
// the raymarch exercises user-defined function calls, 3-vector swizzle, and a wide GLSL.std.450
// breadth (length / normalize / cross / dot / floor / pow / exp / cos / sin / clamp / FAbs / FMix).

module sdf_tut_shaders public

require vulkan/vulkan_boost public  // CommandBuffer + PipelineLayout for the generated push_constants function
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// the output storage image (binding 0); the render path creates it Rgba8 and reads it back.
var @binding = 0 out_img : image2D

//! the square render resolution (multiple of 8 so the 8x8 compute local size tiles cleanly). The host
//! sizes the image off this constant; the shader sizes its pixel->ray mapping off the bound image via
//! imageSize so it does not reference SDF_DIM at runtime.
let public SDF_DIM = 768

let MAX_STEPS = 96          // raymarch iteration budget per pixel
let MAX_DIST = 60.0f        // far plane: distance at which we declare a miss
let HIT_EPS = 0.001f        // distance threshold that counts as a surface hit
let SHADOW_K = 32.0f        // soft-shadow penumbra sharpness (IQ recipe)

// the single animation parameter, pushed each frame from wall-clock time. Drives camera orbit, torus
// rotation, sphere bob, and the blob's cosine-palette colour pulse.
struct Push {
    time : float
}
var @push_constant pc : Push

// IQ's polynomial smooth union: blends two SDFs over a soft radius k. Reduces to min(a,b) when the
// two surfaces are far apart, but produces a curved meeting band where they overlap -- the visual
// signature of distance-field rendering.
def private smin(a, b, k : float) : float {
    let h = max(k - abs(a - b), 0.0f) / k
    return min(a, b) - h * h * h * k * (1.0f / 6.0f)
}

// SDF primitives. Each returns the signed distance from p to the surface (negative = inside).
def private sd_sphere(p : float3; r : float) : float {
    return length(p) - r
}

def private sd_torus(p : float3; major, minor : float) : float {
    let q = float2(length(p.xz) - major, p.y)
    return length(q) - minor
}

def private sd_plane(p : float3; h : float) : float {
    return p.y - h
}

// 2D rotation around the origin; used to spin the torus's local coordinate frame about Y.
def private rot2(p : float2; ang : float) : float2 {
    let c = cos(ang)
    let s = sin(ang)
    return float2(c * p.x - s * p.y, s * p.x + c * p.y)
}

// The scene SDF. Returns (distance_to_nearest_surface, material_id) so the shader can branch on the
// material at the hit point. material 0 = blob (sphere smin torus), material 1 = ground.
def private map(p : float3; t : float) : float2 {
    let d_plane = sd_plane(p, -0.5f)
    let d_sphere = sd_sphere(p - float3(0.0f, 0.2f * sin(t * 1.5f), 0.0f), 0.55f)
    // Torus orbits the sphere via its rotated local xz frame, rebuilt whole by constructor: dasSpirv has no write-swizzle (`pt.x = ...`) yet.
    let xz = rot2(float2(p.x, p.z), t * 0.8f)
    let pt = float3(xz.x, p.y, xz.y)
    let d_torus = sd_torus(pt - float3(1.0f, 0.0f, 0.0f), 0.32f, 0.12f)
    let d_blob = smin(d_sphere, d_torus, 0.3f)
    if (d_plane < d_blob) {
        return float2(d_plane, 1.0f)
    }
    return float2(d_blob, 0.0f)
}

// Scalar overload for the shadow march (no material needed).
def private map_d(p : float3; t : float) : float {
    return map(p, t).x
}

// Forward-difference normal at p: 4 SDF taps (1 baseline + 3 axis-displaced), normalized. Cheaper
// than the 6-tap symmetric central difference; the slight +eps bias is invisible at this eps and at
// our distance threshold. Upgrade to central if normals look skewed under stronger lighting.
def private get_normal(p : float3; t : float) : float3 {
    let eps = 0.001f
    let d = map_d(p, t)
    let nx = map_d(p + float3(eps, 0.0f, 0.0f), t) - d
    let ny = map_d(p + float3(0.0f, eps, 0.0f), t) - d
    let nz = map_d(p + float3(0.0f, 0.0f, eps), t) - d
    return normalize(float3(nx, ny, nz))
}

// March from ro toward rd. Returns (t_hit, material); t_hit >= MAX_DIST means miss (sky). Three exits:
// HIT_EPS hit -> set mat + break; far-plane escape -> break; step budget exhausted -> force a miss,
// since otherwise dist could land just under MAX_DIST and the caller would shade garbage at material 0.
def private march(ro, rd : float3; t : float) : float2 {
    var dist = 0.0f
    var mat = 0.0f
    var hit = 0       // 1 once HIT_EPS is satisfied; dasSpirv doesn't expose bool consts in shaders
    for (_i in range(MAX_STEPS)) {
        let p = ro + rd * dist
        let m = map(p, t)
        if (m.x < HIT_EPS) {
            mat = m.y
            hit = 1
            break
        }
        dist = dist + m.x
        if (dist > MAX_DIST) {
            break
        }
    }
    if (hit == 0) {
        dist = MAX_DIST + 1.0f
    }
    return float2(dist, mat)
}

// Soft shadow via IQ's penumbra estimate: track min(h/dist) along the shadow ray; small ratios mean
// the ray grazed close to a surface (deep penumbra), larger ratios mean an unobstructed path.
def private soft_shadow(ro, rd : float3; t : float) : float {
    var res = 1.0f
    var dist = 0.05f       // small offset to dodge self-shadowing from the surface we just hit
    for (_i in range(48)) {
        let p = ro + rd * dist
        let h = map_d(p, t)
        if (h < 0.001f) {
            res = 0.0f
            break
        }
        res = min(res, SHADOW_K * h / dist)
        dist = dist + clamp(h, 0.02f, 1.0f)
        if (dist > 10.0f) {
            break
        }
    }
    return clamp(res, 0.0f, 1.0f)
}

// Vertical sky gradient (warm horizon -> cool zenith) with a tiny solar disc along sun_dir.
def private sky_color(rd, sun_dir : float3) : float3 {
    let h = clamp(rd.y * 0.5f + 0.5f, 0.0f, 1.0f)
    let horizon = float3(0.94f, 0.78f, 0.62f)    // warm peach
    let zenith = float3(0.45f, 0.62f, 0.85f)     // soft blue
    var col = lerp(horizon, zenith, float3(h, h, h))
    let sun = max(dot(rd, sun_dir), 0.0f)
    col = col + float3(1.0f, 0.85f, 0.6f) * pow(sun, 64.0f)
    return col
}

// Checkered ground albedo: parity of (floor(x) + floor(z)) selects between two warm-grey tones.
def private ground_color(p : float3) : float3 {
    let cx = floor(p.x)
    let cz = floor(p.z)
    let sum = cx + cz
    let parity = sum - 2.0f * floor(0.5f * sum)        // (cx + cz) mod 2 as a float
    let a = float3(0.95f, 0.92f, 0.85f)
    let b = float3(0.30f, 0.34f, 0.38f)
    return lerp(a, b, float3(parity, parity, parity))
}

// Blob albedo: IQ-style cosine palette pulsing with time, brightened slightly toward the top of the
// blob (p.y > 0) so the form reads better under flat lighting. The phase offsets between R/G/B give
// the palette its characteristic hue shift as time advances.
def private blob_color(p : float3; t : float) : float3 {
    let phase = t * 0.6f
    let r = 0.6f + 0.4f * cos(phase + 0.0f)
    let g = 0.45f + 0.4f * cos(phase + 0.8f)
    let b = 0.4f + 0.4f * cos(phase + 1.6f)
    let lift = 0.85f + 0.15f * clamp(p.y + 0.5f, 0.0f, 1.0f)
    return float3(r, g, b) * lift
}

// Reinhard tone map. Compresses any HDR overshoot (rim glow + sun disc) into [0,1]. Output stays
// LINEAR -- the linear->sRGB gamma encode happens at the sRGB-readback blit boundary (and the
// sRGB swapchain blit for the window), not here.
def private tonemap(c : float3) : float3 {
    let one = float3(1.0f, 1.0f, 1.0f)
    return c / (one + c)
}

[vulkan_compute_shader(local_size_x=8, local_size_y=8, name="sdf_spv")]
def sdf_main {
    let gid = gl_GlobalInvocationID
    let dim = imageSize(out_img)

    let t = pc.time

    // screen-normalized pixel coords, y in [-1,1] (sample at pixel center, +y up)
    let px = (2.0f * (float(gid.x) + 0.5f) - float(dim.x)) / float(dim.y)
    let py = -(2.0f * (float(gid.y) + 0.5f) - float(dim.y)) / float(dim.y)

    // camera orbits the scene origin at fixed radius with a gentle vertical sway
    let cam_ang = t * 0.4f
    let cam_dist = 3.0f
    let cam_y = 0.8f + 0.2f * sin(t * 0.3f)
    let ro = float3(cos(cam_ang) * cam_dist, cam_y, sin(cam_ang) * cam_dist)
    let target = float3(0.0f, 0.0f, 0.0f)

    // build the camera basis: right = world_up x fwd, up = fwd x right (handedness is whatever the
    // two cross-product orders below produce; flip either order to mirror the view if needed)
    let fwd = normalize(target - ro)
    let right = normalize(cross(float3(0.0f, 1.0f, 0.0f), fwd))
    let up = cross(fwd, right)

    let focal = 1.6f       // pinhole focal length; larger = narrower FOV
    let rd = normalize(fwd * focal + right * px + up * py)

    let sun_dir = normalize(float3(-0.4f, 0.8f, -0.5f))

    let hit = march(ro, rd, t)
    var col = float3(0.0f, 0.0f, 0.0f)
    if (hit.x >= MAX_DIST) {
        col = sky_color(rd, sun_dir)
    } else {
        let p = ro + rd * hit.x
        let n = get_normal(p, t)
        var albedo = float3(0.5f, 0.5f, 0.5f)
        if (hit.y > 0.5f) {
            albedo = ground_color(p)
        } else {
            albedo = blob_color(p, t)
        }
        let key = max(dot(n, sun_dir), 0.0f)
        let sh = soft_shadow(p + n * 0.001f, sun_dir, t)
        let v = normalize(ro - p)
        let rim = pow(1.0f - max(dot(n, v), 0.0f), 3.0f)
        let amb = 0.25f + 0.25f * (n.y * 0.5f + 0.5f)        // hemisphere ambient (sky/ground bounce)
        let lit = albedo * (amb + key * sh * 0.85f) + float3(1.0f, 0.9f, 0.75f) * rim * 0.2f
        // atmospheric fog: scene fades into the sky color with distance (exponential falloff)
        let f = clamp(1.0f - exp(-0.02f * hit.x * hit.x), 0.0f, 1.0f)
        col = lerp(lit, sky_color(rd, sun_dir), float3(f, f, f))
    }

    col = tonemap(col)
    let coord = int2(int(gid.x), int(gid.y))
    imageStore(out_img, coord, float4(col, 1.0f))
}
