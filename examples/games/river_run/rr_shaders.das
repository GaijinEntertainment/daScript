options gen2
options persistent_heap
options indenting = 4

require glfw/glfw_boost public
require opengl/opengl_boost public
require opengl/opengl_gen public
require opengl/opengl_cache public
require daslib/math_boost public
require daslib/safe_addr public

// Scene shading for River Run.
//
// Every surface goes through one deferred-lit forward pass that writes two
// targets: HDR colour, and view-space normal + linear depth for the screen-space
// passes in rr_postfx. The lighting model is a low-cost analytic environment --
// one sun with a shadow map, a sky hemisphere, and a bounce term tinted by the
// water -- so the section palettes can drive a whole time-of-day per section
// without any authored art.
//
// The shader DSL emits `#version 300 es` on emscripten, so everything here is
// held to the GLSL ES 3.00 / WebGL2 feature set: no compute, no textureGather,
// MRT through `@out @location`, shadow compare through sampler2DShadow.

// --- Vertex interface (matches opengl_gen's PreviewVertex layout) ---

var @in @location = 0 v_position : float3
var @in @location = 1 v_normal : float3
var @in @location = 2 v_texture : float2

var @uniform v_model : float4x4
var @uniform v_view : float4x4
var @uniform v_projection : float4x4
var @uniform v_light_vp : float4x4

// --- Varyings ---

var @inout f_world : float3
var @inout f_normal : float3
var @inout f_uv : float2
var @inout f_view_pos : float3
var @inout f_ray : float3

// --- Environment (host sets these once per frame) ---

var @uniform f_SunDir : float3          // unit vector pointing TOWARD the sun
var @uniform f_SunColor : float3
var @uniform f_SkyColor : float3        // zenith
var @uniform f_HorizonColor : float3
var @uniform f_BounceColor : float3     // light kicked back up off the water
var @uniform f_FogColor : float3
var @uniform f_FogParams : float3       // x = density, y = height falloff, z = start distance
var @uniform f_CameraPos : float3
var @uniform f_GameTime : float
var @uniform f_ZFar : float
var @uniform f_NightAmount : float      // 0 = day, 1 = full night (stars, dimmer sun)

// --- Camera basis, for the attributeless sky pass ---

var @uniform f_CamRight : float3
var @uniform f_CamUp : float3
var @uniform f_CamFwd : float3
var @uniform f_CamScale : float2        // (tan(fov/2) * aspect, tan(fov/2))

// --- Per-draw material ---

var @uniform f_Color : float4
var @uniform f_Material : float4        // x roughness, y spec strength, z emissive, w rim

// Composite models (helicopter, gunboat, jet) bake a part index into uv.x and a
// gloss modifier into uv.y, so a whole multi-coloured hull is one draw call
// instead of one per painted panel.
var @uniform f_PaletteOn : float
var @uniform f_Tint0 : float3
var @uniform f_Tint1 : float3
var @uniform f_Tint2 : float3
var @uniform f_Tint3 : float3

// --- Shadowing ---

// Unit 4, not 0: the post chain and the font shader both own unit 0, and GL
// refuses to sample a colour texture through a sampler2DShadow ("bound to wrong
// sampler type (Depth) - using zero texture"), which silently drops shadows.
var @uniform @stage = 4 f_ShadowMap : sampler2DShadow
var @uniform f_ShadowTexel : float2     // 1 / shadow map size

// --- Water / terrain extras ---

var @uniform f_RiverColor : float4
var @uniform f_BankColor : float4
var @uniform f_ShoreColor : float3      // wet sand / gravel at the waterline
var @uniform f_RockColor : float3       // exposed rock on the steep upper slopes

// --- Fragment outputs (MRT: HDR colour + view normal & linear depth) ---

var @out @location = 0 f_FragColor : float4
var @out @location = 1 f_FragNormalDepth : float4

// ============================================================================
// Shared helpers
// ============================================================================

// math::fract and glsl_common::fract both match here, so spell the fractional
// part out; it lowers to the same GLSL either way.
def private frac1(x : float) : float {
    return x - floor(x)
}

def private frac2(p : float2) : float2 {
    return p - floor(p)
}

def private hash21(p : float2) : float {
    return frac1(sin(dot(p, float2(127.1, 311.7))) * 43758.545)
}

// Value noise with a smooth (cubic) interpolant. The bank shading uses this
// instead of the old floor()-quantized hash, which tiled the ground into a
// visible chessboard.
def private vnoise(p : float2) : float {
    let i = floor(p)
    let f = frac2(p)
    let u = f * f * (float2(3.0) - 2.0 * f)
    let a = hash21(i)
    let b = hash21(i + float2(1.0, 0.0))
    let c = hash21(i + float2(0.0, 1.0))
    let d = hash21(i + float2(1.0, 1.0))
    return lerp(lerp(a, b, u.x), lerp(c, d, u.x), u.y)
}

def private fbm(p : float2; octaves : int) : float {
    var sum = 0.0
    var amp = 0.5
    var freq = p
    for (_i in range(octaves)) {
        sum += vnoise(freq) * amp
        freq *= 2.02
        amp *= 0.5
    }
    return sum
}

// Height-attenuated exponential distance fog. Fading toward the horizon colour
// (not a flat grey) is what makes the far bank read as atmosphere rather than a
// clipped mesh.
def private apply_fog(color : float3; world : float3; view_dist : float) : float3 {
    // Fog only starts biting past f_FogParams.z. Without that offset the haze
    // greys out the play area itself, which is what made the first pass read as
    // washed out rather than atmospheric.
    let d = max(view_dist - f_FogParams.z, 0.0)
    let height_falloff = exp(-max(world.z, 0.0) * f_FogParams.y)
    let amount = saturate(1.0 - exp(-d * f_FogParams.x * height_falloff))
    return lerp(color, f_FogColor, amount)
}

// 3x3 PCF against the single ortho cascade. The normal-facing slope bias keeps
// near-grazing bank geometry from acne without detaching contact shadows.
def private sun_shadow(world : float3; nrm : float3) : float {
    let lp = v_light_vp * float4(world, 1.0)
    var proj = lp.xyz / lp.w
    proj = proj * 0.5 + float3(0.5)
    if (proj.z > 1.0 || proj.x < 0.0 || proj.x > 1.0 || proj.y < 0.0 || proj.y > 1.0) {
        return 1.0
    }
    let ndl = saturate(dot(nrm, f_SunDir))
    let bias = lerp(0.0035, 0.0006, ndl)
    var sum = 0.0
    for (y in range(-1, 2)) {
        for (x in range(-1, 2)) {
            let ofs = float2(float(x), float(y)) * f_ShadowTexel
            sum += textureCompare(f_ShadowMap, proj.xy + ofs, proj.z - bias)
        }
    }
    return sum * (1.0 / 9.0)
}

// Key + sky hemisphere + water bounce, plus a Blinn specular lobe and a rim
// term. Roughness/spec/emissive/rim ride in f_Material so every prop shares one
// program and one pipeline state.
def private shade_surface(albedo : float3; nrm : float3; world : float3; shadow, spec_scale : float) : float3 {
    let view_dir = normalize(f_CameraPos - world)
    let ndl = saturate(dot(nrm, f_SunDir))

    let key = f_SunColor * ndl * shadow
    let sky_mix = saturate(nrm.z * 0.5 + 0.5)
    let ambient = lerp(f_BounceColor, f_SkyColor, sky_mix)

    let roughness = max(f_Material.x, 0.04)
    let shininess = 2.0 / (roughness * roughness) - 2.0
    let half_dir = normalize(f_SunDir + view_dir)
    let spec = pow(saturate(dot(nrm, half_dir)), max(shininess, 1.0)) * f_Material.y * spec_scale * shadow * ndl

    let fresnel = pow(1.0 - saturate(dot(nrm, view_dir)), 4.0)
    let rim = fresnel * f_Material.w * (f_SkyColor * 0.6 + f_HorizonColor * 0.4)

    return albedo * (key + ambient) + f_SunColor * spec + rim + albedo * f_Material.z
}

def private encode_normal_depth(nrm : float3) : float4 {
    let view_nrm = normalize(float3x3(v_view) * nrm)
    return float4(view_nrm * 0.5 + float3(0.5), saturate(-f_view_pos.z / f_ZFar))
}

// Procedural sky: zenith-to-horizon ramp, a haze band pinned at the horizon, a
// sun disc with a wide forward-scatter glow, drifting cloud fbm, and stars that
// fade in with f_NightAmount.
def private sky_color(dir : float3) : float3 {
    let up = saturate(dir.z)
    let horizon_t = pow(1.0 - up, 6.0)
    var col = lerp(f_SkyColor, f_HorizonColor, horizon_t)

    let sun_dot = saturate(dot(dir, f_SunDir))
    let glow = pow(sun_dot, 8.0) * 0.35 + pow(sun_dot, 128.0) * 0.9
    col += f_SunColor * glow
    let disc = smoothstep(0.9982, 0.9992, sun_dot)
    col = lerp(col, f_SunColor * 3.0, disc * (1.0 - f_NightAmount * 0.85))

    if (dir.z > -0.02) {
        let plane = dir.xy / max(dir.z + 0.12, 0.02)
        let drift = float2(f_GameTime * 0.012, f_GameTime * 0.004)
        let cloud = fbm(plane * 1.9 + drift, 4)
        let cover = smoothstep(0.46, 0.72, cloud) * smoothstep(0.0, 0.14, dir.z)
        let cloud_lit = lerp(f_HorizonColor, f_SkyColor * 0.4 + f_SunColor * 0.8, saturate(sun_dot + 0.35))
        col = lerp(col, cloud_lit, cover * 0.55)

        // The visible sky is a narrow band, so the star field needs a dense grid
        // to land more than a handful of cells in frame.
        let star_grid = floor(plane * 95.0)
        let star = hash21(star_grid)
        let twinkle = 0.65 + 0.35 * sin(f_GameTime * 2.5 + star * 40.0)
        let star_lit = smoothstep(0.976, 0.998, star) * twinkle
        col += float3(star_lit * f_NightAmount * smoothstep(0.01, 0.16, dir.z)) * float3(0.9, 0.95, 1.0)
    }
    return col
}

// ============================================================================
// Sky — attributeless fullscreen triangle
// ============================================================================

[vertex_program]
def vs_sky {
    let id = gl_VertexIndex
    let uv = float2(float((id << 1) & 2), float(id & 2))
    let ndc = uv * 2.0 - float2(1.0)
    f_ray = normalize(f_CamFwd + f_CamRight * (ndc.x * f_CamScale.x) + f_CamUp * (ndc.y * f_CamScale.y))
    gl_Position = float4(ndc, 1.0, 1.0)
}

[fragment_program]
def fs_sky {
    f_FragColor = float4(sky_color(normalize(f_ray)), 1.0)
    f_FragNormalDepth = float4(0.5, 0.5, 1.0, 1.0)
}

// ============================================================================
// Shadow caster — depth only
// ============================================================================

[vertex_program]
def vs_shadow {
    gl_Position = v_light_vp * (v_model * float4(v_position, 1.0))
}

[fragment_program]
def fs_shadow {
    f_FragColor = float4(1.0)
}

// ============================================================================
// Props — hull, boats, jets, bridges, trees, houses, pickups
// ============================================================================

[vertex_program]
def vs_prop {
    let world = v_model * float4(v_position, 1.0)
    f_world = world.xyz
    f_normal = normalize(float3x3(v_model) * v_normal)
    f_uv = v_texture
    let view = v_view * world
    f_view_pos = view.xyz
    gl_Position = v_projection * view
}

[fragment_program]
def fs_prop {
    let nrm = normalize(f_normal)
    var albedo = f_Color.xyz
    if (f_PaletteOn > 0.5) {
        // Nested step() selects, so the branch stays uniform across the quad.
        var picked = f_Tint0
        picked = lerp(picked, f_Tint1, step(0.5, f_uv.x))
        picked = lerp(picked, f_Tint2, step(1.5, f_uv.x))
        picked = lerp(picked, f_Tint3, step(2.5, f_uv.x))
        albedo = picked * f_Color.xyz
    }
    let shadow = sun_shadow(f_world, nrm)
    // uv.y is the part's gloss modifier; 0 reads matte, 1 reads lacquered.
    let gloss = (f_PaletteOn > 0.5 ? 0.30 + f_uv.y * 1.6 : 1.0)
    let lit = shade_surface(albedo, nrm, f_world, shadow, gloss)
    f_FragColor = float4(apply_fog(lit, f_world, length(f_view_pos)), f_Color.w)
    f_FragNormalDepth = encode_normal_depth(nrm)
}

// ============================================================================
// Terrain — river banks and the centre island of a split
//
// uv.x carries distance from the water's edge in world units (packed by
// gen_river_banks), which drives the sand/grass/rock ramp and lets the shoreline
// stay put as the banks weave.
// ============================================================================

[vertex_program]
def vs_terrain {
    let world = v_model * float4(v_position, 1.0)
    f_world = world.xyz
    f_normal = normalize(float3x3(v_model) * v_normal)
    f_uv = v_texture
    let view = v_view * world
    f_view_pos = view.xyz
    gl_Position = v_projection * view
}

[fragment_program]
def fs_terrain {
    let shore_dist = f_uv.x
    let p = f_world.xy

    let grain = fbm(p * 0.11, 4)
    let macro = fbm(p * 0.021 + float2(37.0, 11.0), 3)

    // Shore and rock are their own palette entries. Deriving them from the bank
    // colour, as the first pass did, produced a green beach in every biome.
    let sand = f_ShoreColor * (0.86 + grain * 0.30)
    let grass = f_BankColor.xyz * (0.78 + grain * 0.46)
    let dry = f_BankColor.xyz * float3(1.16, 1.06, 0.78)
    let rock = f_RockColor * (0.78 + grain * 0.36)

    // Wet-to-dry sand band right at the waterline, then grass, then rock on the
    // steep upper slopes.
    var albedo = lerp(sand * 0.72, sand, smoothstep(0.0, 1.1, shore_dist))
    albedo = lerp(albedo, grass, smoothstep(1.4, 4.5, shore_dist))
    albedo = lerp(albedo, dry, saturate(macro * 1.4 - 0.35) * smoothstep(6.0, 26.0, shore_dist))

    let nrm = normalize(f_normal)
    let slope = 1.0 - saturate(nrm.z)
    albedo = lerp(albedo, rock, smoothstep(0.55, 0.88, slope))

    let shadow = sun_shadow(f_world, nrm)
    let lit = shade_surface(albedo, nrm, f_world, shadow, 1.0)
    f_FragColor = float4(apply_fog(lit, f_world, length(f_view_pos)), 1.0)
    f_FragNormalDepth = encode_normal_depth(nrm)
}

// ============================================================================
// Water
//
// Three crossing gerstner-ish ripples perturb the normal; the surface then gets
// a Fresnel-weighted sky reflection, a sharp sun glint, depth tinting, and a
// foam band whose width follows uv.x (distance to the nearest bank).
// ============================================================================

def private water_normal(p : float2; t : float) : float3 {
    var d = float2(0.0)
    var amp = 0.055
    var freq = 0.9
    var dir = float2(0.86, 0.5)
    for (i in range(3)) {
        let phase = dot(p, dir) * freq + t * (1.1 + float(i) * 0.45)
        let slope = cos(phase) * amp * freq
        d += dir * slope
        amp *= 0.55
        freq *= 1.9
        dir = float2(dir.x * 0.28 - dir.y * 0.96, dir.x * 0.96 + dir.y * 0.28)
    }
    let ripple = (vnoise(p * 2.4 + float2(t * 0.35, -t * 0.22)) - 0.5) * 0.08
    d += float2(ripple, ripple * 0.6)
    return normalize(float3(-d.x, -d.y, 1.0))
}

[vertex_program]
def vs_water {
    let world = v_model * float4(v_position, 1.0)
    f_world = world.xyz
    f_uv = v_texture
    let view = v_view * world
    f_view_pos = view.xyz
    gl_Position = v_projection * view
}

[fragment_program]
def fs_water {
    let nrm = water_normal(f_world.xy, f_GameTime)
    let view_dir = normalize(f_CameraPos - f_world)
    let shore_dist = f_uv.x

    // Shallow water near the banks reads lighter and greener.
    let depth_t = smoothstep(0.0, 5.5, shore_dist)
    let shallow = f_RiverColor.xyz * 1.55 + float3(0.05, 0.11, 0.06)
    let albedo = lerp(shallow, f_RiverColor.xyz * 0.72, depth_t)

    let fresnel = 0.02 + 0.98 * pow(1.0 - saturate(dot(nrm, view_dir)), 5.0)
    let refl_dir = reflect(-view_dir, nrm)
    let refl = sky_color(normalize(float3(refl_dir.xy, abs(refl_dir.z))))

    let shadow = sun_shadow(f_world, float3(0.0, 0.0, 1.0))
    let ndl = saturate(dot(nrm, f_SunDir))
    let half_dir = normalize(f_SunDir + view_dir)
    let glint = pow(saturate(dot(nrm, half_dir)), 900.0) * shadow

    var col = albedo * (f_SunColor * ndl * 0.35 * shadow + f_SkyColor * 0.55 + f_BounceColor * 0.25)
    col = lerp(col, refl, saturate(fresnel * 0.85))
    col += f_SunColor * glint * 1.5

    // Foam: a bright band hugging the bank, broken up by noise so it does not
    // read as a drawn outline, plus a faint moving lace further out.
    let foam_edge = 1.0 - smoothstep(0.0, 0.55, shore_dist)
    let lace = vnoise(f_world.xy * 5.5 + float2(0.0, f_GameTime * 0.7))
    let churn = vnoise(f_world.xy * 1.7 - float2(f_GameTime * 0.25, 0.0))
    let foam = foam_edge * saturate(0.25 + lace * 1.1) * saturate(0.5 + churn)
    col = lerp(col, float3(0.92, 0.96, 1.0) * (f_SkyColor * 0.5 + float3(0.45)), saturate(foam) * 0.8)

    f_FragColor = float4(apply_fog(col, f_world, length(f_view_pos)), 1.0)
    f_FragNormalDepth = encode_normal_depth(nrm)
}

// ============================================================================
// Rotor — alpha-blended swept disc with per-blade streaks and motion smear
// ============================================================================

[vertex_program]
def vs_rotor {
    let world = v_model * float4(v_position, 1.0)
    f_world = world.xyz
    f_uv = v_texture
    let view = v_view * world
    f_view_pos = view.xyz
    gl_Position = v_projection * view
}

[fragment_program]
def fs_rotor {
    let c = f_uv * 2.0 - float2(1.0)
    let r = length(c)
    if (r > 1.0) {
        discard()
    }
    let ang = atan2(c.y, c.x)

    // Two blades smeared into a disc: a low-frequency angular ripple plus a
    // solid inner haze that stands in for the blur of a spinning rotor.
    let blades = 0.5 + 0.5 * cos(ang * 2.0 + f_Material.z * 26.0)
    let smear = pow(blades, 3.0) * 0.55 + 0.18
    let ring = smoothstep(0.18, 0.42, r) * (1.0 - smoothstep(0.86, 1.0, r))
    let hub = 1.0 - smoothstep(0.0, 0.22, r)

    let alpha = f_Color.w * saturate(ring * smear + hub * 0.9)
    let tint = f_Color.xyz * (0.75 + smear * 0.5) + f_SunColor * 0.12
    f_FragColor = float4(apply_fog(tint, f_world, length(f_view_pos)), alpha)
}

// ============================================================================
// Soft puff -- a camera-facing billboard with a radial falloff
//
// Smoke and fireballs were low-poly spheres, which gave every puff a hard
// faceted silhouette. A billboard with a soft edge is both cheaper and reads as
// a volume rather than a solid. The quad is built in VIEW space so it always
// faces the camera without the host computing a basis; f_Material carries the
// radius, the emissive and the spin angle.
// ============================================================================

[vertex_program]
def vs_puff {
    let center = v_view * (v_model * float4(0.0, 0.0, 0.0, 1.0))
    let r = f_Material.y
    let view_pos = center.xyz + float3(v_position.x * r, v_position.y * r, 0.0)
    f_view_pos = view_pos
    f_uv = v_texture
    f_world = (v_model * float4(0.0, 0.0, 0.0, 1.0)).xyz
    gl_Position = v_projection * float4(view_pos, 1.0)
}

[fragment_program]
def fs_puff {
    var c = f_uv * 2.0 - float2(1.0)
    // Spin the sampling frame so repeated puffs do not read as clones.
    let a = f_Material.w
    let ca = cos(a)
    let sa = sin(a)
    c = float2(c.x * ca - c.y * sa, c.x * sa + c.y * ca)
    let r2 = dot(c, c)
    if (r2 > 1.0) {
        discard()
    }
    // A lumpy edge, so the billboard does not read as a perfect circle.
    let lump = 0.82 + 0.18 * vnoise(c * 2.4 + float2(f_Material.x * 7.3))
    let falloff = pow(saturate(1.0 - r2 / (lump * lump)), 1.7)
    let alpha = f_Color.w * falloff
    let col = f_Color.xyz * (1.0 + f_Material.z * falloff)
    f_FragColor = float4(apply_fog(col, f_world, length(f_view_pos)), alpha)
}

// ============================================================================
// Shockwave ring -- a flat annulus on the water, drawn additively
// ============================================================================

[vertex_program]
def vs_ring {
    let world = v_model * float4(v_position, 1.0)
    f_world = world.xyz
    f_uv = v_texture
    let view = v_view * world
    f_view_pos = view.xyz
    gl_Position = v_projection * view
}

[fragment_program]
def fs_ring {
    let c = f_uv * 2.0 - float2(1.0)
    let r = length(c)
    if (r > 1.0) {
        discard()
    }
    // f_Material.x is the ring's normalised thickness; the band tightens and
    // dims as the wave expands, which is what sells it as a travelling front.
    let width = max(f_Material.x, 0.01)
    let band = 1.0 - saturate(abs(1.0 - r) / width)
    let alpha = f_Color.w * band * band
    f_FragColor = float4(f_Color.xyz * (1.0 + f_Material.z), alpha)
}

// ============================================================================
// Unlit / emissive — bullets, particles, trails, HUD geometry
// ============================================================================

[vertex_program]
def vs_unlit {
    let world = v_model * float4(v_position, 1.0)
    f_world = world.xyz
    let view = v_view * world
    f_view_pos = view.xyz
    gl_Position = v_projection * view
}

[fragment_program]
def fs_unlit {
    let col = f_Color.xyz * (1.0 + f_Material.z)
    f_FragColor = float4(apply_fog(col, f_world, length(f_view_pos)), f_Color.w)
}

// ============================================================================
// Program handles + binding helpers
// ============================================================================

var sky_program : uint
var shadow_program : uint
var prop_program : uint
var terrain_program : uint
var water_program : uint
var rotor_program : uint
var puff_program : uint
var ring_program : uint
var unlit_program : uint
var active_program : uint

def create_scene_programs() {
    sky_program = cache_shader_program(vs_sky`shader_text, fs_sky`shader_text)
    shadow_program = cache_shader_program(vs_shadow`shader_text, fs_shadow`shader_text)
    prop_program = cache_shader_program(vs_prop`shader_text, fs_prop`shader_text)
    terrain_program = cache_shader_program(vs_terrain`shader_text, fs_terrain`shader_text)
    water_program = cache_shader_program(vs_water`shader_text, fs_water`shader_text)
    rotor_program = cache_shader_program(vs_rotor`shader_text, fs_rotor`shader_text)
    puff_program = cache_shader_program(vs_puff`shader_text, fs_puff`shader_text)
    ring_program = cache_shader_program(vs_ring`shader_text, fs_ring`shader_text)
    unlit_program = cache_shader_program(vs_unlit`shader_text, fs_unlit`shader_text)
}

def use_prop_program() {
    glUseProgram(prop_program)
    active_program = prop_program
}

def use_unlit_program() {
    glUseProgram(unlit_program)
    active_program = unlit_program
}

def use_puff_program() {
    glUseProgram(puff_program)
    active_program = puff_program
}

// `seed` only varies the edge lumpiness and spin between puffs.
def draw_puff(pos : float3; radius : float; color : float3; alpha, emissive, spin, seed : float) {
    v_model = compose(pos, float4(0.0, 0.0, 0.0, 1.0), float3(1.0))
    f_Color = float4(color, alpha)
    f_Material = float4(seed, radius, emissive, spin)
    vs_puff_bind_uniform(active_program)
    fs_puff_bind_uniform(active_program)
}

// Material presets, so call sites name a surface instead of tuning four floats.
let MAT_MATTE = float4(0.85, 0.05, 0.0, 0.12)
let MAT_HULL = float4(0.34, 0.65, 0.0, 0.35)
let MAT_METAL = float4(0.24, 1.15, 0.0, 0.45)
let MAT_FOLIAGE = float4(0.92, 0.03, 0.0, 0.22)
let MAT_STONE = float4(0.78, 0.10, 0.0, 0.15)
let MAT_GLOW = float4(0.5, 0.4, 0.85, 0.6)

def draw_prop(pos, scale, color : float3; rot_quat : float4 = float4(0.0, 0.0, 0.0, 1.0);
              material : float4 = MAT_MATTE) {
    v_model = compose(pos, rot_quat, scale)
    f_Color = float4(color, 1.0)
    f_Material = material
    vs_prop_bind_uniform(active_program)
    fs_prop_bind_uniform(active_program)
}

def draw_unlit(pos, scale, color : float3; alpha : float = 1.0;
               rot_quat : float4 = float4(0.0, 0.0, 0.0, 1.0); emissive : float = 0.0) {
    v_model = compose(pos, rot_quat, scale)
    f_Color = float4(color, alpha)
    f_Material = float4(1.0, 0.0, emissive, 0.0)
    vs_unlit_bind_uniform(active_program)
    fs_unlit_bind_uniform(active_program)
}

def draw_shadow_caster(pos, scale : float3; rot_quat : float4 = float4(0.0, 0.0, 0.0, 1.0)) {
    v_model = compose(pos, rot_quat, scale)
    vs_shadow_bind_uniform(active_program)
}
