options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 08 -- the canonical two-pass shadow-map dance. The same depth image flows through both
// pipelines: pass 1 writes the scene's depth from the LIGHT'S point of view into an offscreen
// D32_SFLOAT image; pass 2 reads that SAME image as a `sampler2DShadow` and asks "is anything closer
// to the light here than I am?" -- if yes, the fragment is in shadow. The headline rails:
//
//   1. ``sampler2DShadow`` + ``textureCompare(s, uv, ref) : float``. dasSpirv lowers to
//      ``OpImageSampleDref*`` against an image with the Depth=1 flag and a compare-sampler
//      descriptor. With LINEAR filter the result is the hardware-PCF average over the 2x2
//      footprint, returned in [0, 1] (1 = fully lit, 0 = fully shadowed).
//
//   2. **One image, two roles** -- the shadow map is created USAGE_DEPTH_STENCIL_ATTACHMENT |
//      USAGE_SAMPLED. The shadow render pass's ``finalLayout = DEPTH_STENCIL_READ_ONLY_OPTIMAL``
//      hands it to the main pass already laid out for sampling -- no explicit barrier.
//
//   3. **Slope-scaled depth bias** -- the shadow pipeline enables ``depthBiasEnable`` with a small
//      slope factor. Without it, surfaces self-shadow on every grazing fragment ("shadow acne").
//
//   4. **Light-space matrix** -- a directional light gets an orthographic projection sized to the
//      scene. The light-space NDC is remapped from [-1, 1] to [0, 1] for the shadow-map UV lookup;
//      Vulkan's NDC z is already in [0, 1] so the reference value is used as-is.

module shadow_tut_shaders public

require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

// ===== UBO + push constant =====

//! Shared uniforms used by both passes' vertex shaders + the main fragment shader. ``light_vp`` is
//! the directional light's view-projection (ortho * look-at); ``light_dir`` is the direction *to*
//! the light (normalised). std140-aligned: float4x4 fields land at 16-byte boundaries, float4s pad
//! the trailing scalar slots.
struct Camera {
    view       : float4x4
    proj       : float4x4
    light_vp   : float4x4
    light_dir  : float4    // xyz = direction TO the light; w unused (std140 pad)
    camera_pos : float4    // xyz = world camera position; w unused
}
var @uniform @set = 0 @binding = 0 cam : Camera

//! Per-draw model matrix. 64 bytes fits comfortably in Vulkan's guaranteed-128-byte push-constant
//! window. The shadow pass and main pass both read this -- one push covers both stage updates per
//! object (we declare the same struct, daslang emits one PC block per stage flag).
struct ObjectPush {
    model : float4x4
}
var @push_constant op : ObjectPush

// ===== shared geometry attributes =====

//! Position + world-space normal. Cube uses split vertices so each face has its own face-normal;
//! floor is a single quad with normal = (0, 1, 0). Stride = 24 bytes; the host's vertex-input
//! attribute offsets match (a_pos at +0, a_normal at +12).
var @in @location = 0 a_pos    : float3
var @in @location = 1 a_normal : float3

// ===== shadow pass =====

//! Pass 1: render every caster's depth from the LIGHT'S view. No color attachment, no fragment
//! work -- just `gl_Position = light_vp * model * pos`. After this pass the offscreen D32_SFLOAT
//! image holds, for each light-space (x, y), the depth of the closest caster.
[vulkan_vertex_shader(name="shadow_vert_spv")]
def shadow_vs {
    gl_Position = cam.light_vp * op.model * float4(a_pos, 1.0)
}

// (no fragment shader in pass 1 -- pipeline runs with `pStages = vertex-only`; rasterizer
//  produces depth without a fragment stage when there are no color attachments.)

// ===== main pass =====

var @out @location = 0 v_world_pos     : float3
var @out @location = 1 v_world_normal  : float3
var @out @location = 2 v_light_space   : float4    // pre-perspective-divide
var @out @location = 3 v_obj_pos       : float3    // object-space pos/normal for tri-planar brick
var @out @location = 4 v_obj_normal    : float3

//! Pass 2 vertex shader: pass world-space pos + normal forward to the fragment shader, plus the
//! position projected into light space for the shadow-map lookup. The perspective divide happens
//! in the fragment shader so v_light_space is varying-correct across primitive interior.
[vulkan_vertex_shader(name="main_vert_spv")]
def main_vs {
    let world4 = op.model * float4(a_pos, 1.0)
    v_world_pos = float3(world4.x, world4.y, world4.z)
    let normal_world4 = op.model * float4(a_normal, 0.0)   // model has no non-uniform scale -> safe
    v_world_normal = normalize(float3(normal_world4.x, normal_world4.y, normal_world4.z))
    v_obj_pos = a_pos
    v_obj_normal = a_normal
    v_light_space = cam.light_vp * world4
    gl_Position = cam.proj * cam.view * world4
}

// ===== shadow sampler =====

//! The depth-compare combined sampler. dasSpirv emits the OpTypeImage with the Depth=1 flag (per
//! the `sampler2DShadow` marker). textureCompare lowers to OpImageSampleDrefImplicitLod and
//! returns the scalar PCF result.
var @uniform @set = 0 @binding = 1 shadow_map : sampler2DShadow

//! Brick PBR maps (ambientCG Bricks031, CC0), sampled tri-planar on the cube. Albedo is sRGB; the
//! OpenGL-convention normal map (linear) perturbs the geometric normal for relief; the AO map (linear)
//! darkens the mortar/pits in the ambient term for depth.
var @uniform @set = 0 @binding = 2 brick_albedo : sampler2D
var @uniform @set = 0 @binding = 3 brick_normal : sampler2D
var @uniform @set = 0 @binding = 4 brick_ao     : sampler2D
var @uniform @set = 0 @binding = 5 brick_roughness : sampler2D

var @in @location = 0 f_world_pos    : float3
var @in @location = 1 f_world_normal : float3
var @in @location = 2 f_light_space  : float4
var @in @location = 3 f_obj_pos      : float3
var @in @location = 4 f_obj_normal   : float3
var @out @location = 0 frag_color    : float4

//! Pass 2 fragment shader. Four lighting rails, each written up in doc/source/tutorials/08_shadow.rst:
//! 5x5 PCF soft shadows, hemisphere ambient, procedural polished tiles on the floor, Blinn-Phong specular
//! on the cube. Material branch is `f_world_pos.y < 0.5` — floor sits at y = 0, cube hovers at y ~ 1-2.

let SHADOW_MAP_PX = 1024.0

def private pcf_shadow(uv : float2; ref : float) : float {
    // 5x5 grid of hardware-PCF'd taps; each textureCompare already PCFs the 2x2 around the tap.
    let texel = 1.0 / SHADOW_MAP_PX
    var sum = 0.0
    for (j in range(-2, 3)) {
        for (i in range(-2, 3)) {
            let off = float2(float(i), float(j)) * texel
            sum += textureCompare(shadow_map, uv + off, ref)
        }
    }
    return sum * (1.0 / 25.0)
}

// Object-space tiling for the tri-planar brick projection. The cube spans [-0.5, 0.5] in object
// space, so scale 1.0 maps one full brick tile across each face (legible courses + mortar).
let BRICK_SCALE = 1.0

// Bricks031 relief is shallow; boost the tangent xy so the brick reads bumpy even face-on (the cube's
// top face, seen like a small floor, is otherwise nearly flat). Decode + boost + renormalize.
let BRICK_NORMAL_STRENGTH = 3.0
def private unpack_brick_normal(rgb : float3) : float3 {
    let t = rgb * 2.0 - float3(1.0, 1.0, 1.0)
    return normalize(float3(t.x * BRICK_NORMAL_STRENGTH, t.y * BRICK_NORMAL_STRENGTH, t.z))
}

//! Tri-planar brick albedo: sample the brick color on each axis-aligned plane and blend by the
//! normal weights `wn`. Replaces the old `sin(uv*84)` micro-texture that aliased into banding.
def private tri_albedo(p : float3; wn : float3) : float3 {
    let ax = texture(brick_albedo, float2(p.z, p.y) * BRICK_SCALE).rgb
    let ay = texture(brick_albedo, float2(p.x, p.z) * BRICK_SCALE).rgb
    let az = texture(brick_albedo, float2(p.x, p.y) * BRICK_SCALE).rgb
    return ax * wn.x + ay * wn.y + az * wn.z
}

//! Tri-planar normal map (Golus "whiteout" blend): unpack the tangent-space brick normal on each
//! plane, reorient it into world space using the geometric normal `n`, and blend by `wn`. Gives the
//! bricks real relief under the moving sun without per-vertex tangents.
def private tri_normal(p : float3; n : float3; wn : float3) : float3 {
    let tx = unpack_brick_normal(texture(brick_normal, float2(p.z, p.y) * BRICK_SCALE).rgb)
    let ty = unpack_brick_normal(texture(brick_normal, float2(p.x, p.z) * BRICK_SCALE).rgb)
    let tz = unpack_brick_normal(texture(brick_normal, float2(p.x, p.y) * BRICK_SCALE).rgb)
    // reorient each plane's tangent normal into world space (whiteout), then triblend
    let cx = float3(abs(tx.z) * n.x, tx.y + n.y, tx.x + n.z)   // X plane -> .zyx
    let cy = float3(ty.x + n.x, abs(ty.z) * n.y, ty.y + n.z)   // Y plane -> .xzy
    let cz = float3(tz.x + n.x, tz.y + n.y, abs(tz.z) * n.z)   // Z plane -> .xyz
    return normalize(cx * wn.x + cy * wn.y + cz * wn.z)
}

//! Tri-planar AO scalar (grayscale, sampled from .r), blended by `wn`. 1 = open, 0 = occluded crevice.
def private tri_ao(p : float3; wn : float3) : float {
    let ax = texture(brick_ao, float2(p.z, p.y) * BRICK_SCALE).x
    let ay = texture(brick_ao, float2(p.x, p.z) * BRICK_SCALE).x
    let az = texture(brick_ao, float2(p.x, p.y) * BRICK_SCALE).x
    return ax * wn.x + ay * wn.y + az * wn.z
}

// Tri-planar roughness scalar (grayscale .r), blended by `wn`. Mortar rough, brick faces smoother.
def private tri_rough(p : float3; wn : float3) : float {
    let ax = texture(brick_roughness, float2(p.z, p.y) * BRICK_SCALE).x
    let ay = texture(brick_roughness, float2(p.x, p.z) * BRICK_SCALE).x
    let az = texture(brick_roughness, float2(p.x, p.y) * BRICK_SCALE).x
    return ax * wn.x + ay * wn.y + az * wn.z
}

[vulkan_fragment_shader(name="main_frag_spv")]
def main_fs {   // nolint:STYLE038 - shader body - phases are pipeline-coupled
    let n = normalize(f_world_normal)
    let l = normalize(float3(cam.light_dir.x, cam.light_dir.y, cam.light_dir.z))
    let ndotl = max(dot(n, l), 0.0)
    let view_dir = normalize(float3(cam.camera_pos.x - f_world_pos.x,
                                    cam.camera_pos.y - f_world_pos.y,
                                    cam.camera_pos.z - f_world_pos.z))

    // Light-space NDC = clip.xyz / clip.w; Vulkan NDC.z is already in [0,1] so the depth ref is
    // used as-is, but xy is in [-1, 1] and the shadow-map UV is [0, 1].
    let w = max(f_light_space.w, 0.0001)
    let ndc = float3(f_light_space.x / w, f_light_space.y / w, f_light_space.z / w)
    let shadow_uv = float2(ndc.x * 0.5 + 0.5, ndc.y * 0.5 + 0.5)
    let ref_depth = clamp(ndc.z, 0.0, 1.0)

    // N·L-scaled fragment-side bias on top of the pipeline's slope-scaled bias. Grazing faces
    // (low N·L) push the reference depth further out so the comparison stops mis-firing on the
    // lit side of the cube, with a floor so even N·L≈1 surfaces get a small bias.
    let frag_bias = max(0.0008 * (1.0 - ndotl), 0.00015)
    let lit_ref = clamp(ref_depth - frag_bias, 0.0, 1.0)
    var lit = pcf_shadow(shadow_uv, lit_ref)
    // Anything facing significantly away from the light is shadow regardless of the depth test;
    // bypassing the lookup avoids the bias artifacts at N·L ≈ 0 where small numerical noise flips
    // the depth comparison.
    if (ndotl < 0.05) { lit = 0.0 }

    // Hemisphere ambient: warm sky overhead, cool dim under. n.y -> [-1, 1]; mix on (n.y+1)/2.
    let sky_color = float3(0.22, 0.26, 0.36)
    let ground_color = float3(0.08, 0.06, 0.05)
    let amb_t = n.y * 0.5 + 0.5
    let ambient = lerp(ground_color, sky_color, float3(amb_t, amb_t, amb_t))

    // Warm sun.
    let sun = float3(1.25, 1.05, 0.78)

    // Material branch: y < 0.5 -> floor (procedural tile + polish); else cube (Blinn-Phong).
    if (f_world_pos.y < 0.5) {
        // ===== floor =====
        // 1-unit tile, with a tighter inner checker for visual interest.
        let gx = f_world_pos.x * 0.5 + 100.0   // +100 keeps fract positive on negative coords
        let gz = f_world_pos.z * 0.5 + 100.0
        let cell_x = floor(gx)
        let cell_z = floor(gz)
        let checker = (sin(cell_x * 12.9898 + cell_z * 78.233) * 0.5 + 0.5) > 0.5 ? 1.0 : 0.0
        let tile_base = lerp(float3(0.34, 0.30, 0.26), float3(0.20, 0.18, 0.16),
                             float3(checker, checker, checker))

        // Tile edge lines (darker grout).
        let fx = gx - cell_x
        let fz = gz - cell_z
        let edge_x = smoothstep(0.0, 0.03, fx) * smoothstep(0.0, 0.03, 1.0 - fx)
        let edge_z = smoothstep(0.0, 0.03, fz) * smoothstep(0.0, 0.03, 1.0 - fz)
        let edge = edge_x * edge_z                    // 1 in interior, 0 on edges
        let tile_albedo = tile_base * (0.55 + edge * 0.45)

        // Fake "reflection" pickup: warm tint near the cube's XZ footprint — the floor reads as if
        // it's catching a touch of the cube's color. Pure shader math, no second pass.
        let cube_xz_dist = sqrt(f_world_pos.x * f_world_pos.x + f_world_pos.z * f_world_pos.z)
        let bounce = exp(-cube_xz_dist * 1.2) * 0.35
        let bounce_tint = float3(0.95, 0.55, 0.30) * bounce

        // Fresnel-like grazing-angle brighten — polished/wet-floor look.
        let view_dot = max(dot(n, view_dir), 0.0)
        let fresnel = pow(1.0 - view_dot, 4.0)
        let sheen = float3(0.85, 0.95, 1.10) * fresnel * 0.45

        let diffuse = sun * ndotl * lit
        let col = ambient * tile_albedo + tile_albedo * diffuse * 0.9 + bounce_tint + sheen
        frag_color = float4(col.x, col.y, col.z, 1.0)
    } else {
        // ===== cube =====

        // OBJECT-space tri-planar brick: the cube is rotated, so sampling in object space keeps the
        // courses aligned to the faces (world-space tri-planar would swirl across the seams). The
        // perturbed object normal is rotated into world space via the model matrix for lighting.
        let on = normalize(f_obj_normal)
        let abs_on = float3(abs(on.x), abs(on.y), abs(on.z))
        let wsum = max(abs_on.x + abs_on.y + abs_on.z, 0.001)
        let wn = abs_on / float3(wsum, wsum, wsum)
        let cube_albedo = tri_albedo(f_obj_pos, wn)
        let obj_bn = tri_normal(f_obj_pos, on, wn)
        let bn4 = op.model * float4(obj_bn, 0.0)
        let bn = normalize(float3(bn4.x, bn4.y, bn4.z))

        // Diffuse + Blinn-Phong off the PERTURBED world normal (brick relief); shadow `lit` stays
        // from the geometric depth pass. Brick is matte: wide specular lobe, low intensity.
        let ao = tri_ao(f_obj_pos, wn)
        let rough = tri_rough(f_obj_pos, wn)
        let bndotl = max(dot(bn, l), 0.0)
        let h = normalize(l + view_dir)
        let ndoth = max(dot(bn, h), 0.0)
        // roughness drives the highlight: smoother brick faces -> tighter, brighter spec than rough mortar
        let shininess = 8.0 + 56.0 * (1.0 - rough)
        let spec = pow(ndoth, shininess) * lit
        let diffuse = sun * bndotl * lit
        let specular = sun * spec * (1.0 - rough) * 0.5
        // AO darkens the ambient (indirect) term in the mortar/pits; a light cavity term on the direct
        // diffuse keeps the recesses reading as recessed even under the sun.
        let col = ambient * cube_albedo * ao + cube_albedo * diffuse * (0.65 + 0.35 * ao) + specular
        frag_color = float4(col.x, col.y, col.z, 1.0)
    }
}
