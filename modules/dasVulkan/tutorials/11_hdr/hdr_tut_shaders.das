options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 11 -- HDR + Karis-style bloom. Five shader pairs, one per pass in the bloom pipeline:
//
//   1. SCENE   : draws the instanced cube swarm into an HDR target (R16G16B16A16_SFLOAT). The 1000
//                cubes carry an ``emissive`` per-instance flag; emissive cubes multiply their base
//                colour by ``EMISSIVE_BOOST`` (= 8.0) so the linear RGB lands in [4,8] -- well above
//                the bright-pass knee. Non-emissive cubes stay in [0,1].
//
//   2. BRIGHT  : fullscreen pass; reads the HDR scene, applies a soft-knee threshold (Frostbite-style
//                quadratic curve), writes the bright-only image into bloom mip 0 at 1/2 resolution.
//                The soft knee keeps the transition gradual so almost-bright pixels (highlights on
//                non-emissive cubes) contribute a fraction of their luminance.
//
//   3. DOWN    : fullscreen pass, 4 invocations (mip 0->1, 1->2, 2->3, 3->4). 5-sample "Kawase /
//                Karis-bilinear" filter: one centre sample weighted 0.5, four diagonal samples at
//                +-1 texel weighted 0.125 each. Bilinear filtering makes each diagonal sample act as
//                a 2x2 box average, so 5 GPU samples cover a 16-pixel footprint per output texel.
//                A vec2 push-constant (`src_rcp`) supplies the inverse of the SOURCE mip dimensions
//                so the same shader runs at every mip level.
//
//   4. UP      : fullscreen pass with additive blend, 4 invocations (4->3, 3->2, 2->1, 1->0). 9-tap
//                tent (3x3 with 1-2-1 / 2-4-2 / 1-2-1 / 16 weights) -- the classic Kawase upsample.
//                Reads the smaller mip via the same push-constant `src_rcp` and adds onto the
//                larger mip (loadOp=LOAD + COLOR_BLEND srcColor=ONE dstColor=ONE on RGB; the
//                colour-write mask drops alpha so the dst alpha stays pinned at the bright-pass
//                value).
//
//   5. COMPOSITE: fullscreen pass to the LDR backbuffer. Reads HDR scene + bloom mip 0; combines as
//                ``hdr + bloom * bloom_intensity`` (intensity = 0.06 by default, kept conservative so
//                the dim cubes are still recognisable), applies ACES-fitted tonemap and gamma 1/2.2
//                to encode for the sRGB target.
//
// All shaders share `cam_time : float4` in the scene UBO (xyz = camera world position, w = seconds).
// The scene pass has no push-constant -- only the UBO. The four post-process passes share one
// push-constant block (`BloomCtrl`: `src_rcp + threshold + soft_knee + bloom_intensity`); each pass
// reads only the fields it needs and the host fills the relevant ones before each draw.

module hdr_tut_shaders public

require vulkan/vulkan_boost public  // Device + DeviceMemory used by the generated bind_uniform helper
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== scene UBO =====

//! Shared scene constants -- view / projection matrices, camera world position packed with time.
struct Camera {
    view     : float4x4
    proj     : float4x4
    cam_time : float4     // xyz = camera world position, w = time in seconds
}
var @uniform @set = 0 @binding = 0 cam : Camera

// ===== push constants =====

//! One shared push-constant block. Different passes read different fields; daslang's static_if isn't
//! at play -- the SPIR-V emitter sees every load, but Vulkan happily ignores writes to fields that
//! no draw reads. Keeping a single block keeps the descriptor layout and pipeline-layout count tiny.
struct BloomCtrl {
    src_rcp         : float2  // 1.0 / source-mip dimensions, used by DOWN + UP for sample offsets
    threshold       : float   // bright-pass: pixels with luma < threshold are killed
    soft_knee       : float   // bright-pass: soft-knee width around threshold (0 = hard cut)
    bloom_intensity : float   // composite: how loud bloom is over the HDR scene
}
var @push_constant pc : BloomCtrl

// ===== SCENE pass =====

// per-vertex (binding 0): pos + normal
var @in @location = 0 a_pos    : float3
var @in @location = 1 a_normal : float3
// per-instance (binding 1): offset + colour + phase + emissive (1.0 if cube should glow, 0.0 else)
var @in @location = 2 a_offset    : float3
var @in @location = 3 a_color     : float3
var @in @location = 4 a_phase     : float
var @in @location = 5 a_emissive  : float

var @out @location = 0 v_world_pos    : float3
var @out @location = 1 v_world_normal : float3
var @out @location = 2 v_color        : float3
var @out @location = 3 v_emissive     : float

[vulkan_vertex_shader(name="hdr_scene_vert_spv")]
def hdr_scene_vs {
    let t = cam.cam_time.w
    let breathe = 0.7 + 0.2 * sin(t * 1.5 + a_phase * 6.2832)
    let local = a_pos * breathe
    let ang = t * 0.25 + a_phase * 6.2832
    let c = cos(ang)
    let s = sin(ang)
    let rotated = float3(c * local.x - s * local.z, local.y, s * local.x + c * local.z)
    let world = rotated + a_offset
    gl_Position = cam.proj * cam.view * float4(world, 1.0)
    v_world_pos = world
    v_world_normal = float3(c * a_normal.x - s * a_normal.z, a_normal.y, s * a_normal.x + c * a_normal.z)
    v_color = a_color
    v_emissive = a_emissive
}

var @in @location = 0 f_world_pos    : float3
var @in @location = 1 f_world_normal : float3
var @in @location = 2 f_color        : float3
var @in @location = 3 f_emissive     : float
var @out @location = 0 frag_hdr      : float4

let EMISSIVE_BOOST = 8.0          // 8x exposure -- well above the bright-pass knee
let LIT_AMBIENT    = 0.25
let LIT_KEY        = 0.7

[vulkan_fragment_shader(name="hdr_scene_frag_spv")]
def hdr_scene_fs {
    let n = normalize(f_world_normal)
    let v = normalize(cam.cam_time.xyz - f_world_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))
    let key = max(dot(n, l), 0.0) * LIT_KEY + LIT_AMBIENT
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.5)
    // lit_lo stays in [0,1]; emissive cubes multiply by EMISSIVE_BOOST so the HDR target carries
    // values up to ~8. The bright-pass shader will threshold these.
    let lit_lo = f_color * key + float3(0.25, 0.35, 0.55) * rim * 0.4
    let lit = lit_lo * (1.0 + f_emissive * (EMISSIVE_BOOST - 1.0))
    frag_hdr = float4(lit, 1.0)
}

// ===== BRIGHT pass =====

// Fullscreen triangle: 3 unindexed vertices, gl_VertexIndex 0..2, no vertex buffer. The vertex
// outputs a `(x,y)` that covers the screen and a `uv` in [0,1].

var @out @location = 0 v_uv : float2

[vulkan_vertex_shader(name="hdr_fullscreen_vert_spv")]
def hdr_fullscreen_vs {
    // Big triangle trick: (-1,-1), (3,-1), (-1,3) covers the unit square in NDC; UV derived from
    // ndc as `(xy + 1) / 2`, so the visible [0,1] portion of the triangle maps linearly to [0,1] UV.
    let xs = fixed_array(-1.0,  3.0, -1.0)
    let ys = fixed_array(-1.0, -1.0,  3.0)
    let i = gl_VertexIndex
    let x = xs[i]
    let y = ys[i]
    gl_Position = float4(x, y, 0.0, 1.0)
    v_uv = float2((x + 1.0) * 0.5, (y + 1.0) * 0.5)
}

var @uniform @set = 1 @binding = 0 src0 : sampler2D     // bright reads HDR scene at set=1,binding=0

var @in @location = 0 f_uv : float2
var @out @location = 0 frag_bright : float4

//! Frostbite soft-knee: `t = clamp((luma - threshold + knee) / (2*knee), 0, 1); curve = t*t*(3-2*t)`
//! gives a Hermite blend between zero (luma << threshold) and one (luma > threshold + knee). The
//! standard fast-bloom hard cut is `step(threshold, luma)` and produces aliasing on moving emissives.
[vulkan_fragment_shader(name="hdr_bright_frag_spv")]
def hdr_bright_fs {
    let hdr = textureLod(src0, f_uv, 0.0).xyz
    let luma = dot(hdr, float3(0.2126, 0.7152, 0.0722))   // BT.709 luminance
    let knee = max(pc.soft_knee, 0.0001)
    let t = clamp((luma - pc.threshold + knee) / (2.0 * knee), 0.0, 1.0)
    let curve = t * t * (3.0 - 2.0 * t)
    // Partial-Karis weight `1/(1 + luma)` (Karis, "Next Generation Post Processing in Call of Duty
    // Advanced Warfare", SIGGRAPH 2014, slide 142). Without it an 8x emissive cube writes raw HDR ~8.0
    // into mip 0 and the bilinear read clamps to a hard white square; 8/9 ~= 0.89 keeps the pyramid in [0,1].
    let karis_weight = 1.0 / (1.0 + luma)
    frag_bright = float4(hdr * karis_weight * curve, 1.0)
}

// ===== DOWN pass (Kawase / Karis-bilinear, 5 samples) =====

var @out @location = 0 frag_down : float4

[vulkan_fragment_shader(name="hdr_down_frag_spv")]
def hdr_down_fs {
    let o = pc.src_rcp          // 1 source-texel offset in UV space
    let c  = textureLod(src0, f_uv, 0.0).xyz
    let lt = textureLod(src0, f_uv + float2(-o.x, -o.y), 0.0).xyz
    let rt = textureLod(src0, f_uv + float2( o.x, -o.y), 0.0).xyz
    let lb = textureLod(src0, f_uv + float2(-o.x,  o.y), 0.0).xyz
    let rb = textureLod(src0, f_uv + float2( o.x,  o.y), 0.0).xyz
    // Centre weighted 0.5; the four diagonal samples 0.125 each.
    // Each diagonal sample sits at +-1 texel of the source mip, so bilinear filter effectively
    // averages a 2x2 box -- 5 samples cover a 16-texel footprint of the input.
    let sum = c * 0.5 + (lt + rt + lb + rb) * 0.125
    frag_down = float4(sum, 1.0)
}

// ===== UP pass (3x3 tent, additive blend) =====

var @out @location = 0 frag_up : float4

[vulkan_fragment_shader(name="hdr_up_frag_spv")]
def hdr_up_fs {
    let o = pc.src_rcp
    // 9-tap tent with 1-2-1 / 2-4-2 / 1-2-1 weights (sum = 16, divided out)
    let s00 = textureLod(src0, f_uv + float2(-o.x, -o.y), 0.0).xyz
    let s10 = textureLod(src0, f_uv + float2( 0.0, -o.y), 0.0).xyz
    let s20 = textureLod(src0, f_uv + float2( o.x, -o.y), 0.0).xyz
    let s01 = textureLod(src0, f_uv + float2(-o.x,  0.0), 0.0).xyz
    let s11 = textureLod(src0, f_uv, 0.0).xyz
    let s21 = textureLod(src0, f_uv + float2( o.x,  0.0), 0.0).xyz
    let s02 = textureLod(src0, f_uv + float2(-o.x,  o.y), 0.0).xyz
    let s12 = textureLod(src0, f_uv + float2( 0.0,  o.y), 0.0).xyz
    let s22 = textureLod(src0, f_uv + float2( o.x,  o.y), 0.0).xyz
    let tent = ((s00 + s20 + s02 + s22) * (1.0 / 16.0) +
                (s10 + s01 + s21 + s12) * (2.0 / 16.0) +
                 s11                    * (4.0 / 16.0))
    // Output alpha = 1; the host-side pipeline's blend state writes RGB additively
    // (srcColor=ONE, dstColor=ONE) and the colourWriteMask drops alpha so the dst alpha stays
    // pinned at the bright-pass value.
    frag_up = float4(tent, 1.0)
}

// ===== COMPOSITE pass (HDR scene + bloom -> ACES tonemap -> sRGB LDR) =====

var @uniform @set = 1 @binding = 1 src_bloom : sampler2D

var @out @location = 0 frag_ldr : float4

//! ACES fitted approximation (Krzysztof Narkowicz 2015). Full ACES is a 4-matrix tone mapper; this fit
//! hits the same shoulder/toe shape with one rational polynomial -- cheap, and it keeps the filmic look
//! the bloom targets. daslang shaders need explicit float3 broadcasts (no implicit scalar broadcast).
def aces_fit(x : float3) : float3 {
    let a = float3(2.51, 2.51, 2.51)
    let b = float3(0.03, 0.03, 0.03)
    let c = float3(2.43, 2.43, 2.43)
    let d = float3(0.59, 0.59, 0.59)
    let e = float3(0.14, 0.14, 0.14)
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0))
}

[vulkan_fragment_shader(name="hdr_composite_frag_spv")]
def hdr_composite_fs {
    let hdr   = textureLod(src0,      f_uv, 0.0).xyz
    let bloom = textureLod(src_bloom, f_uv, 0.0).xyz
    let merged = hdr + bloom * pc.bloom_intensity
    let mapped = aces_fit(merged)
    // Output LINEAR post-tonemap values in [0,1] -- the LDR target is R16G16B16A16_SFLOAT. Gamma comes
    // at the *blit*, where the destination is sRGB: the swap target for the window (asserted in
    // show_hdr.das), an R8G8B8A8_SRGB readback for the test. Free in hardware, ~12-bit dark, no banding.
    frag_ldr = float4(mapped, 1.0)
}
