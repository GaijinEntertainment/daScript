options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 10 -- deferred shading "putting it all together". Four shader pairs sharing one cube +
// floor scene:
//
//   * SHADOW PASS  (shadow_vs, no fragment shader) -- depth-only render from a directional light's
//     POV into a 1024x1024 D32_SFLOAT image. Lifted from tutorial 08's shadow rail. The output is
//     sampled in the LIGHTING pass via `sampler2DShadow` + `textureCompare` (hardware PCF).
//
//   * G-BUFFER PASS  (gbuffer_vs + gbuffer_fs) -- writes THREE @location outputs simultaneously
//     (MRT, the dasSpirv rail PR #3195 enables): albedo+specular (RGBA8 with spec in .a), perturbed
//     world-space normal (RGBA16F with shininess in .a), world position (RGBA16F with material tag
//     in .a). Material is branched by the per-draw push constant `op.material` (0 = cube, 1 =
//     floor) with tri-planar UVs derived from world position -- no per-vertex UVs. NORMAL MAPPING
//     via derivative-based TBN (Schueler 2010): tangent + bitangent reconstructed from dFdx/dFdy
//     of world-pos and tri-planar UV, then a tangent-space normal sample is rotated into world
//     space and written to the G-buffer.
//
//   * SSAO PASS  (ssao_vs fullscreen tri + ssao_fs) -- samples the G-buffer normal + world-position
//     as SAMPLED textures (NEAREST), so it can read NEIGHBOUR texels. Real screen-space AO: a
//     hemisphere of sample points around each fragment's world position is projected back to screen
//     and tested against the stored surface depth -- crevices + the cat-floor contact darken.
//
//   * LIGHTING PASS  (lighting_vs fullscreen tri + lighting_fs) -- samples the G-buffer (albedo+spec,
//     normal, world-pos) + the SSAO factor as SAMPLED textures, samples the shadow map
//     (sampler2DShadow + 5x5 PCF) + the HDR environment, and accumulates ONE directional sun +
//     NUM_LIGHTS orbiting colored point lights packed in the scene UBO. A 6-mode `debug_mode` push
//     constant flips between the composed lit result and the raw G-buffer / SSAO / lighting-only views.
//
// Three single-subpass render passes hand off via attachment finalLayouts (gbuffer color ->
// SHADER_READ_ONLY -> sampled by ssao + lighting; ssao -> SHADER_READ_ONLY -> sampled by lighting),
// the more common real-world deferred architecture than one multi-subpass pass with input attachments.
//
// UBO LAYOUT (two UBOs):
//   * `xform : XformCam` at set 0 / binding 0  -- view + proj + light_vp (gbuffer + shadow + ssao)
//   * `scene : SceneCam` at set 1 / binding 1  -- light_vp + light_dir + camera_pos + point lights
//     (lighting pass). Two UBOs keep the gbuffer's per-object xform separate from the scene globals.

module deferred_tut_shaders public

require vulkan/vulkan_boost public
require vulkan/spirv_vulkan_shader public
require spirv/spirv_builtins public
require math

let NUM_LIGHTS = 3

// ===== shared UBO data structures =====

// Point lights are unrolled into pos0/col0..pos2/col2 float4 fields rather than a struct-array,
// because dasSpirv's compute_block_layout currently rejects fixed-array and nested-struct fields
// in @uniform/@push_constant blocks (only scalars/vectors/matrices on the leaves).

struct XformCam {
    view     : float4x4
    proj     : float4x4
    light_vp : float4x4
}
var @uniform @set = 0 @binding = 0 xform : XformCam

struct SceneCam {
    light_vp   : float4x4
    light_dir  : float4    // xyz = direction TO the sun (normalised); w unused
    camera_pos : float4    // xyz = world camera position; w = time (seconds)
    // Point lights: each takes two float4 -- pos (xyz=pos, w=range) + col (xyz=color, w=falloff).
    pos0 : float4
    col0 : float4
    pos1 : float4
    col1 : float4
    pos2 : float4
    col2 : float4
}
var @uniform @set = 1 @binding = 1 scene : SceneCam

//! Per-draw push constant: 64-byte model matrix + 4-byte material tag (0 = cube, 1 = floor).
//! Material drives the gbuffer fragment's albedo+normal branch and is mirrored into the G-buffer
//! world-pos.w so the lighting pass can read it without a 5th attachment.
struct ObjectPush {
    model    : float4x4
    material : int
}
var @push_constant op : ObjectPush

// ===== shared vertex inputs (cube + floor share pos+normal, stride 24, no UV) =====

var @in @location = 0 a_pos    : float3
var @in @location = 1 a_normal : float3
var @in @location = 2 a_uv     : float2

// ===== shadow pass =====

//! Pass 1: depth-only render from the sun's POV. No color attachment, no fragment shader -- just
//! gl_Position = light_vp * model * pos. After this pass the D32_SFLOAT image holds, per light-
//! space (x, y), the depth of the closest caster.
[vulkan_vertex_shader(name="shadow_vert_spv")]
def shadow_vs {
    gl_Position = xform.light_vp * op.model * float4(a_pos, 1.0)
}

// ===== G-buffer pass =====

var @out @location = 0 v_world_pos     : float3
var @out @location = 1 v_world_normal  : float3
var @flat @out @location = 2 v_material : int
var @out @location = 3 v_uv            : float2

[vulkan_vertex_shader(name="gbuffer_vert_spv")]
def gbuffer_vs {
    let world4 = op.model * float4(a_pos, 1.0)
    v_world_pos = float3(world4.x, world4.y, world4.z)
    let n_world4 = op.model * float4(a_normal, 0.0)
    v_world_normal = normalize(float3(n_world4.x, n_world4.y, n_world4.z))
    v_material = op.material
    v_uv = a_uv
    gl_Position = xform.proj * xform.view * world4
}

// Cat statue PBR maps (concrete_cat_statue, CC0): diff = sRGB albedo, nor_gl = OpenGL-convention
// tangent normal, arm = AO(r) / Roughness(g) / Metallic(b). UV-mapped (the cat carries OBJ UVs).
var @uniform @set = 0 @binding = 1 cat_albedo : sampler2D
var @uniform @set = 0 @binding = 2 cat_normal : sampler2D
var @uniform @set = 0 @binding = 3 cat_arm    : sampler2D

// Floor brick maps (ambientCG Bricks031, CC0): Color = sRGB albedo, NormalGL = tangent normal,
// AmbientOcclusion = AO, Roughness drives the spec. World-XZ tiled.
var @uniform @set = 0 @binding = 4 floor_albedo : sampler2D
var @uniform @set = 0 @binding = 5 floor_normal : sampler2D
var @uniform @set = 0 @binding = 6 floor_ao     : sampler2D
var @uniform @set = 0 @binding = 7 floor_roughness : sampler2D

var @in @location = 0 f_world_pos    : float3
var @in @location = 1 f_world_normal : float3
var @flat @in @location = 2 f_material : int
var @in @location = 3 f_uv           : float2

var @out @location = 0 g_albedo_out   : float4   // rgb = albedo, a = specular intensity
var @out @location = 1 g_normal_out   : float4   // rgb = perturbed world-space normal, a = shininess (Blinn exp)
var @out @location = 2 g_worldpos_out : float4   // rgb = world pos, a = material tag

//! Derivative-based TBN reconstruction (Schueler 2010). NO per-vertex tangent buffer required.
def private perturb_normal(n : float3; p : float3; uv : float2; tn : float3) : float3 {
    let dp1 = dFdx(p)
    let dp2 = dFdy(p)
    let duv1 = dFdx(uv)
    let duv2 = dFdy(uv)
    let dp2perp = cross(dp2, n)
    let dp1perp = cross(n, dp1)
    let T = dp2perp * duv1.x + dp1perp * duv2.x
    let B = dp2perp * duv1.y + dp1perp * duv2.y
    let invmax = 1.0 / sqrt(max(dot(T, T), dot(B, B)))
    return normalize(T * (tn.x * invmax) + B * (tn.y * invmax) + n * tn.z)
}

[vulkan_fragment_shader(name="gbuffer_frag_spv")]
def gbuffer_fs {
    let n = normalize(f_world_normal)
    var albedo = float3(0.0, 0.0, 0.0)
    var spec_intensity = 0.0
    var perturbed_normal = float3(0.0, 1.0, 0.0)

    // Material branch on the per-draw tag: f_material is the @flat int varying routed from the push
    // constant (0 = cat statue, 1 = floor), reliable because dasSpirv emits the Flat decoration. Unlike
    // the old `world_pos.y < 0.5` hack, the cat can sit ON the floor without picking up brick material.
    if (f_material == 1) {
        // Floor: real ambientCG Bricks031 maps, tiled by world XZ. Texture AO baked into the albedo;
        // the normal map gives mortar relief via the derivative TBN. Brick is matte -> low spec.
        let uv = float2(f_world_pos.x, f_world_pos.z) * 0.35
        let fb_ao = texture(floor_ao, uv).x
        albedo = texture(floor_albedo, uv).rgb * (0.5 + 0.5 * fb_ao)
        // brick is matte: mortar (rough) ~0 spec, brick faces (smoother) a touch more
        spec_intensity = (1.0 - texture(floor_roughness, uv).x) * 0.4
        // Bricks031 has shallow relief; boost the tangent xy so the mortar reads, then renormalize
        // (the raw `*2-1` decode is NOT unit-length after bilinear/JPEG, and the xy is weak).
        let tn_raw = texture(floor_normal, uv).rgb * 2.0 - float3(1.0, 1.0, 1.0)
        let tn = normalize(float3(tn_raw.x * 3.0, tn_raw.y * 3.0, tn_raw.z))
        perturbed_normal = perturb_normal(n, f_world_pos, uv, tn)
    } else {
        // Cat: real concrete-statue PBR maps, UV-mapped. ARM packs AO/Roughness/Metallic, diff is the
        // albedo, nor_gl perturbs the geometric normal via the derivative TBN. AO bakes into the albedo;
        // roughness drives specular intensity, so the HDR env reflection is stronger where it's smoother.
        let alb = texture(cat_albedo, f_uv).rgb
        let arm = texture(cat_arm, f_uv).rgb
        let tex_ao = arm.x
        let rough = arm.y
        albedo = alb * (0.55 + 0.45 * tex_ao)
        spec_intensity = (1.0 - rough) * 0.6
        let tn = normalize(texture(cat_normal, f_uv).rgb * 2.0 - float3(1.0, 1.0, 1.0))
        perturbed_normal = perturb_normal(n, f_world_pos, f_uv, tn)
    }

    g_albedo_out   = float4(albedo, spec_intensity)
    g_normal_out   = float4(perturbed_normal, 32.0)             // .a = Blinn shininess
    g_worldpos_out = float4(f_world_pos, float(f_material))
}

// ===== SSAO pass (real screen-space ambient occlusion) =====

// The G-buffer normal + world-position are SAMPLED images here, not subpass inputs, so this pass can read
// NEIGHBOUR texels -- what input attachments cannot do. Per fragment: sample a hemisphere around its world
// position, reproject each, count points hidden behind geometry; crevices + the cat-floor contact darken.

// A production AO adds a noise texture + separable blur; this keeps a per-pixel hash rotation + 16-sample spiral.

var @out @location = 0 v_uv_ssao : float2

[vulkan_vertex_shader(name="ssao_vert_spv")]
def ssao_vs {
    let xs = fixed_array(-1.0,  3.0, -1.0)
    let ys = fixed_array(-1.0, -1.0,  3.0)
    let idx = gl_VertexIndex
    let x = xs[idx]
    let y = ys[idx]
    gl_Position = float4(x, y, 0.0, 1.0)
    v_uv_ssao = float2((x + 1.0) * 0.5, (y + 1.0) * 0.5)
}

// G-buffer as sampled textures (NEAREST). xform (set 0 / binding 0, shared with the gbuffer pass)
// supplies view + proj so we can project hemisphere sample points back to screen space.
var @uniform @set = 0 @binding = 1 ssao_g_normal   : sampler2D
var @uniform @set = 0 @binding = 2 ssao_g_worldpos : sampler2D

var @in @location = 0 f_uv_ssao : float2
var @out @location = 0 ssao_out : float4

let SSAO_K        = 16
let SSAO_RADIUS   = 0.6
let SSAO_BIAS     = 0.03
let SSAO_STRENGTH = 1.7      // scales raw occlusion so the cat-floor crease reads in the composite

//! Per-pixel rotation hash -- breaks the 16-sample spiral pattern so the AO doesn't band.
def private hash21(p : float2) : float {
    return fract(sin(dot(p, float2(127.1, 311.7))) * 43758.5453)
}

[vulkan_fragment_shader(name="ssao_frag_spv")]
def ssao_fs {
    let uv = f_uv_ssao
    let n_raw = texture(ssao_g_normal, uv).xyz
    if (dot(n_raw, n_raw) < 0.01) {
        ssao_out = float4(1.0, 1.0, 1.0, 1.0)        // background -> no darkening
        return
    }
    let n = normalize(n_raw)
    let p = texture(ssao_g_worldpos, uv).xyz
    let cur_vz = (xform.view * float4(p, 1.0)).z

    // Tangent basis around the normal (pick a non-parallel up vector).
    var up = float3(0.0, 1.0, 0.0)
    if (abs(n.y) > 0.95) {
        up = float3(1.0, 0.0, 0.0)
    }
    let tangent = normalize(cross(up, n))
    let bitangent = cross(n, tangent)
    let ang0 = hash21(uv * 1024.0) * 6.2831853

    var occ = 0.0
    for (i in range(SSAO_K)) {
        let fi = (float(i) + 0.5) / float(SSAO_K)
        let ang = ang0 + float(i) * 2.3998277                       // golden angle, radians
        let rr = SSAO_RADIUS * sqrt(fi)
        // Hemisphere-biased offset: spiral in the tangent plane, lifted along the normal.
        let off = (cos(ang) * tangent + sin(ang) * bitangent) * rr + n * (rr * 0.5)
        let sp = p + off
        let clip = xform.proj * xform.view * float4(sp, 1.0)
        let w = max(clip.w, 0.0001)
        let suv = float2(clip.x / w * 0.5 + 0.5, clip.y / w * 0.5 + 0.5)
        let stored_n = texture(ssao_g_normal, suv).xyz
        let stored_p = texture(ssao_g_worldpos, suv).xyz
        let stored_vz = (xform.view * float4(stored_p, 1.0)).z
        let sample_vz = (xform.view * float4(sp, 1.0)).z
        let range_check = smoothstep(0.0, 1.0, SSAO_RADIUS / max(abs(cur_vz - stored_vz), 0.0001))
        // View looks down -Z: a stored surface CLOSER to the camera has a GREATER (less negative) z,
        // so it occludes the sample point. Guard against off-screen + background samples.
        if (clip.w > 0.0 && suv.x >= 0.0 && suv.x <= 1.0 && suv.y >= 0.0 && suv.y <= 1.0
                && dot(stored_n, stored_n) > 0.01 && stored_vz >= sample_vz + SSAO_BIAS) {
            occ += range_check
        }
    }
    let ao = clamp(1.0 - SSAO_STRENGTH * occ / float(SSAO_K), 0.0, 1.0)
    ssao_out = float4(ao, ao, ao, 1.0)
}

// ===== lighting pass (subpass 2) =====

var @out @location = 0 v_uv_lit : float2

[vulkan_vertex_shader(name="lighting_vert_spv")]
def lighting_vs {
    let xs = fixed_array(-1.0,  3.0, -1.0)
    let ys = fixed_array(-1.0, -1.0,  3.0)
    let idx = gl_VertexIndex
    let x = xs[idx]
    let y = ys[idx]
    gl_Position = float4(x, y, 0.0, 1.0)
    v_uv_lit = float2((x + 1.0) * 0.5, (y + 1.0) * 0.5)
}

struct DebugPC {
    debug_mode : int
}
var @push_constant dbg : DebugPC

// G-buffer + SSAO as sampled textures (NEAREST, set 0). The fullscreen-triangle uv maps 1:1 to the
// G-buffer texels, so a NEAREST fetch reads each fragment's own deferred values exactly.
var @uniform @set = 0 @binding = 0 lit_albedo   : sampler2D
var @uniform @set = 0 @binding = 1 lit_normal   : sampler2D
var @uniform @set = 0 @binding = 2 lit_worldpos : sampler2D
var @uniform @set = 0 @binding = 3 lit_ssao     : sampler2D

var @uniform @set = 1 @binding = 0 shadow_map : sampler2DShadow

//! Equirectangular HDR environment map (Cannon, Poly Haven, CC0). RGBA32F. Sampled by direction for
//! image-based lighting: the env colour along the surface normal tints the diffuse ambient, and along
//! the reflected view gives a real specular reflection.
var @uniform @set = 1 @binding = 2 env_map : sampler2D

let SHADOW_MAP_PX = 1024.0

def private pcf_shadow(uv : float2; ref : float) : float {
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

//! Direction -> equirectangular UV. atan2/acos confirmed working in dasSpirv (the "no inverse trig"
//! gap was a phantom). u wraps the azimuth, v maps the polar angle top(0) -> bottom(1).
def private equirect_uv(d : float3) : float2 {
    let u = atan2(d.z, d.x) * 0.15915494 + 0.5        // 1 / (2*pi)
    let v = acos(clamp(d.y, -1.0, 1.0)) * 0.31830989  // 1 / pi
    return float2(u, v)
}

var @in @location = 0 f_uv_lit : float2
var @out @location = 0 frag_color : float4

//! One point-light's contribution. Branchless: when distance > range, atten clamps to 0 so the
//! light naturally drops off. `pos.w` = range; `col.w` = falloff exponent on (1 - d/range).
def private point_light_contrib(pos : float4; col : float4; p : float3; n : float3; view_dir : float3;
                                shininess : float; spec_intensity : float) : float3 {
    let lvec = pos.xyz - p
    let ld = max(length(lvec), 0.0001)
    let ldir = lvec / float3(ld, ld, ld)
    let ndotl = max(dot(n, ldir), 0.0)
    let atten = pow(max(1.0 - ld / pos.w, 0.0), col.w)
    let hp = normalize(ldir + view_dir)
    let spec_pt = pow(max(dot(n, hp), 0.0), shininess) * spec_intensity
    return col.xyz * (ndotl + spec_pt) * atten
}

[vulkan_fragment_shader(name="lighting_frag_spv")]
def lighting_fs {   // nolint:STYLE038 - shader body - phases are pipeline-coupled
    // 16-row mode-indicator strip (drawn in shader, so window + recording + test all see it).
    if (gl_FragCoord.y < 16.0) {
        if (dbg.debug_mode == 1) {
            frag_color = float4(0.95, 0.45, 0.45, 1.0)         // albedo = warm red
        } elif (dbg.debug_mode == 2) {
            frag_color = float4(0.45, 0.95, 0.55, 1.0)         // normal = green
        } elif (dbg.debug_mode == 3) {
            frag_color = float4(0.45, 0.55, 0.95, 1.0)         // world-pos = blue
        } elif (dbg.debug_mode == 4) {
            frag_color = float4(0.80, 0.80, 0.80, 1.0)         // ssao = grey
        } elif (dbg.debug_mode == 5) {
            frag_color = float4(0.95, 0.55, 0.95, 1.0)         // lighting-only = magenta
        } else {
            frag_color = float4(0.95, 0.85, 0.45, 1.0)         // lit = yellow
        }
        return
    }

    let albedo_spec = texture(lit_albedo, f_uv_lit)
    let normal_mat  = texture(lit_normal, f_uv_lit)
    let worldpos_mt = texture(lit_worldpos, f_uv_lit)
    let ssao_factor = texture(lit_ssao, f_uv_lit).x

    let albedo = albedo_spec.rgb
    let spec_intensity = albedo_spec.a
    let n = normalize(normal_mat.xyz)
    let shininess = normal_mat.a
    let p = worldpos_mt.xyz

    if (dbg.debug_mode == 1) {
        frag_color = float4(albedo, 1.0)
        return
    }
    if (dbg.debug_mode == 2) {
        frag_color = float4(n * 0.5 + float3(0.5, 0.5, 0.5), 1.0)
        return
    }
    if (dbg.debug_mode == 3) {
        frag_color = float4(p * 0.15 + float3(0.5, 0.5, 0.5), 1.0)
        return
    }
    if (dbg.debug_mode == 4) {
        frag_color = float4(ssao_factor, ssao_factor, ssao_factor, 1.0)
        return
    }

    // Background pixels (normal is zero) -> sunset gradient.
    if (dot(n, n) < 0.01) {
        let sky_top = float3(0.06, 0.08, 0.18)
        let sky_bot = float3(0.36, 0.22, 0.30)
        let t = clamp(f_uv_lit.y, 0.0, 1.0)
        let bg = lerp(sky_top, sky_bot, float3(t, t, t))
        frag_color = float4(bg, 1.0)
        return
    }

    let view_dir = normalize(scene.camera_pos.xyz - p)

    // ----- Sun (shadowed directional) -----
    let sun_dir = normalize(scene.light_dir.xyz)
    let sun_color = float3(1.25, 1.10, 0.80)
    let ndotl_sun = max(dot(n, sun_dir), 0.0)

    let lc = scene.light_vp * float4(p, 1.0)
    let lw = max(lc.w, 0.0001)
    let ndc = float3(lc.x / lw, lc.y / lw, lc.z / lw)
    let shadow_uv = float2(ndc.x * 0.5 + 0.5, ndc.y * 0.5 + 0.5)
    let ref_depth = clamp(ndc.z, 0.0, 1.0)
    // Larger bias than tutorial 8 -- the deferred path's normal-mapped surfaces vary normal at
    // sub-pixel scale, which amplifies depth-bias quantization into the banding you saw.
    let frag_bias = max(0.003 * (1.0 - ndotl_sun), 0.001)
    let lit_ref = clamp(ref_depth - frag_bias, 0.0, 1.0)
    var shadow = pcf_shadow(shadow_uv, lit_ref)
    if (ndotl_sun < 0.05) { shadow = 0.0 }

    let half_sun = normalize(sun_dir + view_dir)
    let spec_sun = pow(max(dot(n, half_sun), 0.0), shininess) * spec_intensity

    let sun_contrib = sun_color * (ndotl_sun + spec_sun) * shadow

    // ----- Point lights ----- (unrolled because UBO field array isn't supported by dasSpirv)
    var pt_contrib = float3(0.0, 0.0, 0.0)
    pt_contrib = pt_contrib + point_light_contrib(scene.pos0, scene.col0, p, n, view_dir, shininess, spec_intensity)
    pt_contrib = pt_contrib + point_light_contrib(scene.pos1, scene.col1, p, n, view_dir, shininess, spec_intensity)
    pt_contrib = pt_contrib + point_light_contrib(scene.pos2, scene.col2, p, n, view_dir, shininess, spec_intensity)

    // ----- Hemisphere ambient + HDR environment IBL (modulated by SSAO) -----
    let sky_color = float3(0.22, 0.24, 0.36)
    let ground_color = float3(0.08, 0.06, 0.05)
    let amb_t = n.y * 0.5 + 0.5
    let hemi_ambient = lerp(ground_color, sky_color, float3(amb_t, amb_t, amb_t))
    // env colour along the normal tints the diffuse ambient (cheap irradiance approximation); env
    // colour along the reflected view gives a real specular reflection, weighted by Fresnel + material.
    let env_amb = texture(env_map, equirect_uv(n)).rgb
    let ambient = lerp(hemi_ambient, env_amb, float3(0.4, 0.4, 0.4))   // splat t — dasSpirv FMix needs vec t
    let inc = float3(-view_dir.x, -view_dir.y, -view_dir.z)
    let rdir = inc - n * (2.0 * dot(n, inc))
    let env_refl = texture(env_map, equirect_uv(rdir)).rgb
    let fres = pow(1.0 - max(dot(n, view_dir), 0.0), 4.0)
    let env_spec = env_refl * (spec_intensity * (0.10 + 0.7 * fres))

    let ao = clamp(ssao_factor, 0.0, 1.0)
    let lighting = (ambient * ao) + (sun_contrib + pt_contrib) * (0.55 + 0.45 * ao) + env_spec * ao

    if (dbg.debug_mode == 5) {
        frag_color = float4(lighting, 1.0)
        return
    }

    let col = albedo * lighting
    frag_color = float4(col, 1.0)
}
