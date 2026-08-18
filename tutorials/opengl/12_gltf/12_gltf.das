options gen2
options gc
options persistent_heap
require glfw/glfw_boost
require daslib/strings_convert
require opengl/opengl_boost
require gltf/gltf_boost
require gltf/gltf_gl
require stbimage
require math
require daslib/math_boost
require daslib/safe_addr
require live/live_gc

// 12 - glTF PBR Capstone: deferred + HDR + bloom (OpenGL / WebGL2 tutorial)
//
// The ladder's finale, and the one rung that loads a real production asset. Every prior
// rung generated its geometry procedurally or read a Wavefront OBJ; this one loads a
// **glTF 2.0** model -- Khronos's runtime 3D format -- through the pure-daslang dasGLTF
// module, then renders it with the whole modern pipeline the earlier rungs built:
//
//   * tutorial 10's DEFERRED shading (a multi-target G-buffer) + shadow map + SSAO, and
//   * tutorial 11's HDR float target + Karis BLOOM + ACES tonemap.
//
// The model is BoomBox (CC0, from the Khronos sample set): a portable stereo with a full
// metallic-roughness material set -- base colour, metal-rough, tangent-space normal,
// occlusion, and an EMISSIVE display. That emissive map is the point: its glowing panel
// carries above-1.0 intensity into the HDR target and blooms, exactly like rung 11's
// emissive cubes, but now driven by a real texture on a real mesh.
//
// dasGLTF does all the asset work: `load_gltf` parses the .glb (JSON + binary chunks, base64,
// accessor decode) into a backend-agnostic GltfScene; `upload_gltf` turns that into GL
// buffers + textures (sRGB for base-colour/emissive, linear for the rest, mipmapped). We
// never touch a vertex or a pixel by hand -- we walk `scene.nodes`, bind each primitive's
// VAO and material, and draw. The deferred + HDR + bloom machinery below is authored in the
// same daslang shader DSL as every other rung, lowered to GLSL by dasGlsl.
//
// Six passes:
//   1. SHADOW    -> depth-only from the sun's POV (tutorial 08 pattern).
//   2. GEOMETRY  -> the 4-attachment G-buffer: albedo+occlusion, world-normal+roughness,
//                   world-pos+metallic, emissive. The glTF model + a procedural floor.
//   3. SSAO      -> hemisphere-kernel occlusion from the G-buffer normal + position, then a
//                   depth-aware (bilateral) separable Gaussian blur to denoise it edge-safely.
//   4. LIGHTING  -> a fullscreen deferred pass: Cook-Torrance GGX per surface, one shadowed
//                   sun + three orbiting point lights + hemisphere/IBL ambient, plus the
//                   boosted emissive, written LINEAR into the RGBA16F HDR target.
//   5. BLOOM     -> tutorial 11's bright / down x4 / up x4 Karis pyramid over the HDR target.
//   6. COMPOSITE -> add bloom, ACES-tonemap into [0,1], gamma-encode, to the screen.
//
// One homogeneous program: main() is the desktop windowed viewer; on the web the wasm run
// path drives init / update / shutdown once per browser frame, with BoomBox.glb + the HDR
// environment fetched into the virtual filesystem first (see the .das.assets.json sidecar).
// This rung renders static models; skinned glTF is shown by the standalone animation example.

// ===== geometry pass: vertex layout matches gltf_gl's GltfGlVertex (@location 0..7) =====
// We consume 0..3 (position / normal / tangent / uv0). The procedural floor supplies the
// same four attributes, so one geometry program draws both the model and the floor.
var @in @location = 0 a_position : float3
var @in @location = 1 a_normal   : float3
var @in @location = 2 a_tangent  : float4
var @in @location = 3 a_uv0      : float2

var @uniform u_model    : float4x4
var @uniform u_normal_matrix : float3x3  // inverse-transpose of u_model's 3x3 (correct normals under scale)
var @uniform u_view     : float4x4
var @uniform u_proj     : float4x4
var @uniform u_light_vp : float4x4
var @uniform u_material : float          // 0 = glTF model, 1 = procedural floor

// PBR material factors (glTF material, per primitive).
var @uniform u_base_color_factor  : float4
var @uniform u_metallic_factor    : float
var @uniform u_roughness_factor   : float
var @uniform u_emissive_factor    : float3
var @uniform u_normal_scale       : float
var @uniform u_occlusion_strength : float
var @uniform u_alpha_cutoff       : float
var @uniform u_alpha_mode         : float   // 0 OPAQUE, 1 MASK, 2 BLEND

// base-color + emissive are sRGB textures (hardware sRGB->linear decode on sample); the rest
// are linear. gltf_gl folds an absent map to a 1x1 white / flat-normal default, so every
// sampler is always valid.
var @uniform @stage = 0 u_base_color_tex  : sampler2D
var @uniform @stage = 1 u_metal_rough_tex : sampler2D
var @uniform @stage = 2 u_normal_tex      : sampler2D
var @uniform @stage = 3 u_occlusion_tex   : sampler2D
var @uniform @stage = 4 u_emissive_tex    : sampler2D

var @inout gv_world_pos     : float3
var @inout gv_world_normal  : float3
var @inout gv_world_tangent : float3
var @inout gv_tangent_w     : float
var @inout gv_uv0           : float2

// The four G-buffer outputs. GLSL ES 3.00 (WebGL2) requires an explicit layout(location=N)
// on every fragment output once there is more than one -- so the @location is load-bearing.
var @out @location = 0 g_albedo_occ    : float4   // albedo.rgb + occlusion.a
var @out @location = 1 g_normal_rough  : float4   // world normal.xyz + roughness.a
var @out @location = 2 g_worldpos_metal : float4  // world position.xyz + metallic.a
var @out @location = 3 g_emissive      : float4   // emissive.rgb (raw; boosted at lighting)

// ----- shadow pass: depth-only from the sun's POV -----
[vertex_program]
def shadow_vs {
    gl_Position = u_light_vp * u_model * float4(a_position, 1.0)
}

[fragment_program]
def shadow_fs {
    // depth-only framebuffer (colour draw buffer is GL_NONE) -- this write is discarded.
    g_albedo_occ = float4(1.0, 1.0, 1.0, 1.0)
}

// ----- geometry pass -----
[vertex_program]
def gbuffer_vs {
    let world = u_model * float4(a_position, 1.0)
    gv_world_pos = world.xyz
    gv_world_normal = normalize(u_normal_matrix * a_normal)
    gv_world_tangent = u_normal_matrix * a_tangent.xyz
    gv_tangent_w = a_tangent.w
    gv_uv0 = a_uv0
    gl_Position = u_proj * u_view * world
}

// Tangent-space normal mapping when a tangent frame exists; fall back to the geometric normal
// otherwise (the flat-normal default texture also yields no perturbation). Same body as the
// dasGLTF forward PBR shader.
def perturb_gltf_normal(uv : float2) : float3 {
    let n_geom = normalize(gv_world_normal)
    let tlen = length(gv_world_tangent)
    var n = n_geom
    if (tlen > 0.0001) {
        let t = gv_world_tangent * (1.0 / tlen)
        let b = cross(n_geom, t) * gv_tangent_w
        let s = texture(u_normal_tex, uv).xyz * 2.0 - float3(1.0, 1.0, 1.0)
        let sx = s.x * u_normal_scale
        let sy = s.y * u_normal_scale
        n = normalize(t * sx + b * sy + n_geom * s.z)
    }
    return n
}

[fragment_program]
def gbuffer_fs {
    var albedo = float3(0.2, 0.2, 0.22)
    var normal = normalize(gv_world_normal)
    var metallic = 0.0
    var roughness = 1.0
    var occlusion = 1.0
    var emissive = float3(0.0, 0.0, 0.0)
    if (u_material > 0.5) {
        // procedural floor: a rough neutral dielectric that receives shadow + AO + reflection.
        albedo = float3(0.20, 0.20, 0.23)
        roughness = 0.85
        metallic = 0.0
    } else {
        // glTF metallic-roughness material. base-color + emissive sample linear (sRGB texture
        // format decodes in hardware), so no manual pow(2.2) is needed here.
        let uv = gv_uv0
        let base = texture(u_base_color_tex, uv) * u_base_color_factor
        if (u_alpha_mode > 0.5 && u_alpha_mode < 1.5 && base.w < u_alpha_cutoff) {
            discard()
        }
        let mr = texture(u_metal_rough_tex, uv)
        albedo = base.xyz
        metallic = mr.z * u_metallic_factor
        roughness = clamp(mr.y * u_roughness_factor, 0.04, 1.0)
        occlusion = 1.0 + u_occlusion_strength * (texture(u_occlusion_tex, uv).x - 1.0)
        emissive = texture(u_emissive_tex, uv).xyz * u_emissive_factor
        normal = perturb_gltf_normal(uv)
    }
    g_albedo_occ = float4(albedo, occlusion)
    g_normal_rough = float4(normal, roughness)
    g_worldpos_metal = float4(gv_world_pos, metallic)
    g_emissive = float4(emissive, 1.0)
}

// ===== fullscreen passes (SSAO / lighting / bloom / composite share post_vs) =====
var @in @location = 0 q_pos : float2
var @inout v_uv : float2
var @out post_color : float4      // shared single output for every fullscreen pass

[vertex_program]
def post_vs {
    gl_Position = float4(q_pos, 0.0, 1.0)
    v_uv = q_pos * 0.5 + float2(0.5, 0.5)
}

def smoothstep3(a, b, x : float) : float {
    let t = clamp((x - a) / (b - a), 0.0, 1.0)
    return t * t * (3.0 - 2.0 * t)
}

def hash21(p : float2) : float {
    let v = sin(dot(p, float2(127.1, 311.7))) * 43758.5453
    return v - floor(v)
}

// ----- SSAO pass: reads the G-buffer normal + world position AS TEXTURES (so it can sample
// neighbours) and accumulates a hemisphere-kernel occlusion factor. Same kernel as rung 10.
var @uniform @stage = 0 ssao_g_normal   : sampler2D
var @uniform @stage = 1 ssao_g_worldpos : sampler2D

let SSAO_K = 16
let SSAO_RADIUS = 0.6
let SSAO_BIAS = 0.03
let SSAO_STRENGTH = 1.7

[fragment_program]
def ssao_fs {
    let n_raw = texture(ssao_g_normal, v_uv).xyz
    var ao = 1.0
    if (dot(n_raw, n_raw) >= 0.01) {
        let n = normalize(n_raw)
        let p = texture(ssao_g_worldpos, v_uv).xyz
        let cur_vz = (u_view * float4(p, 1.0)).z
        var up = float3(0.0, 1.0, 0.0)
        if (abs(n.y) > 0.95) {
            up = float3(1.0, 0.0, 0.0)
        }
        let tangent = normalize(cross(up, n))
        let bitangent = cross(n, tangent)
        let ang0 = hash21(v_uv * 1024.0) * 6.2831853
        var occ = 0.0
        for (i in range(SSAO_K)) {
            let fi = (float(i) + 0.5) / float(SSAO_K)
            let ang = ang0 + float(i) * 2.3998277
            let rr = SSAO_RADIUS * sqrt(fi)
            let off = (cos(ang) * tangent + sin(ang) * bitangent) * rr + n * (rr * 0.5)
            let sp = p + off
            let clip = u_proj * u_view * float4(sp, 1.0)
            let wv = max(clip.w, 0.0001)
            let suv = float2(clip.x / wv * 0.5 + 0.5, clip.y / wv * 0.5 + 0.5)
            let stored_p = texture(ssao_g_worldpos, suv).xyz
            let stored_n = texture(ssao_g_normal, suv).xyz
            let stored_vz = (u_view * float4(stored_p, 1.0)).z
            let sample_vz = (u_view * float4(sp, 1.0)).z
            let range_check = smoothstep3(0.0, 1.0, SSAO_RADIUS / max(abs(cur_vz - stored_vz), 0.0001))
            if (clip.w > 0.0 && suv.x >= 0.0 && suv.x <= 1.0 && suv.y >= 0.0 && suv.y <= 1.0
                    && dot(stored_n, stored_n) > 0.01 && stored_vz >= sample_vz + SSAO_BIAS) {
                occ = occ + range_check
            }
        }
        ao = clamp(1.0 - SSAO_STRENGTH * occ / float(SSAO_K), 0.0, 1.0)
    }
    post_color = float4(ao, ao, ao, 1.0)
}

// ----- SSAO blur: depth-aware (bilateral) separable Gaussian to denoise -----
// The 16-sample kernel is rotated per pixel (hash21), trading banding for high-frequency noise;
// a 9-tap Gaussian run horizontally then vertically smooths it. Each tap is weighted by view-space
// depth similarity (bilateral), so the blur smooths within a surface but never bleeds occlusion
// across a silhouette edge into the background. u_blur_dir picks the axis + step width per pass.
var @uniform @stage = 0 blur_src : sampler2D
var @uniform @stage = 1 blur_pos : sampler2D   // G-buffer world position (for the depth weight)
var @uniform u_blur_dir : float2

let BLUR_DEPTH_SHARP = 8.0   // larger = the depth weight falls off faster across an edge

// One bilateral tap: returns (weight, weight * ao) so the caller normalizes by total weight.
def bilateral_tap(uv : float2; spatial, center_z : float) : float2 {
    let z = (u_view * float4(texture(blur_pos, uv).xyz, 1.0)).z
    let dz = z - center_z
    let w = spatial * exp(-dz * dz * BLUR_DEPTH_SHARP)
    return float2(w, w * texture(blur_src, uv).x)
}

[fragment_program]
def ssao_blur_fs {
    let o = u_blur_dir
    let center_z = (u_view * float4(texture(blur_pos, v_uv).xyz, 1.0)).z
    var acc = bilateral_tap(v_uv, 0.227027, center_z)
    acc = acc + bilateral_tap(v_uv + o, 0.1945946, center_z) + bilateral_tap(v_uv - o, 0.1945946, center_z)
    acc = acc + bilateral_tap(v_uv + o * 2.0, 0.1216216, center_z) + bilateral_tap(v_uv - o * 2.0, 0.1216216, center_z)
    acc = acc + bilateral_tap(v_uv + o * 3.0, 0.054054, center_z) + bilateral_tap(v_uv - o * 3.0, 0.054054, center_z)
    acc = acc + bilateral_tap(v_uv + o * 4.0, 0.016216, center_z) + bilateral_tap(v_uv - o * 4.0, 0.016216, center_z)
    let result = acc.y / max(acc.x, 0.0001)
    post_color = float4(result, result, result, 1.0)
}

// ----- deferred lighting pass -----
var @uniform @stage = 0 lit_albedo_occ    : sampler2D
var @uniform @stage = 1 lit_normal_rough  : sampler2D
var @uniform @stage = 2 lit_worldpos_metal : sampler2D
var @uniform @stage = 3 lit_emissive      : sampler2D
var @uniform @stage = 4 lit_ssao          : sampler2D
var @uniform @stage = 5 shadow_map        : sampler2DShadow
var @uniform @stage = 6 env_map           : sampler2D
var @uniform u_cam_pos   : float3
var @uniform u_light_dir : float3   // direction TOWARD the sun, normalized
var @uniform u_time      : float

let SHADOW_DIM_F = 1024.0
let EMISSIVE_BOOST = 6.0          // pushes the emissive display well above the bright-pass knee

// Trowbridge-Reitz GGX normal distribution.
def distribution_ggx(n, h : float3; roughness : float) : float {
    let a = roughness * roughness
    let a2 = a * a
    let ndoth = max(dot(n, h), 0.0)
    let d = ndoth * ndoth * (a2 - 1.0) + 1.0
    return a2 / max(3.14159265 * d * d, 0.0000001)
}

// Smith geometry term with Schlick-GGX, direct-lighting k = (r+1)^2 / 8.
def geometry_smith(n, v, l : float3; roughness : float) : float {
    let r = roughness + 1.0
    let k = r * r * 0.125
    let ndotv = max(dot(n, v), 0.0)
    let ndotl = max(dot(n, l), 0.0)
    let gv = ndotv / (ndotv * (1.0 - k) + k)
    let gl = ndotl / (ndotl * (1.0 - k) + k)
    return gv * gl
}

def fresnel_schlick(cos_theta : float; f0 : float3) : float3 {
    let m = clamp(1.0 - cos_theta, 0.0, 1.0)
    let m5 = m * m * m * m * m
    return f0 + (float3(1.0, 1.0, 1.0) - f0) * m5
}

// One Cook-Torrance light: (diffuse + specular) * radiance * ndotl, shared by the sun and the
// point lights.
def cook_torrance(n, v, l, albedo, f0 : float3; roughness, metallic : float; radiance : float3) : float3 {
    let h = normalize(v + l)
    let ndotl = max(dot(n, l), 0.0)
    let ndotv = max(dot(n, v), 0.0)
    let d = distribution_ggx(n, h, roughness)
    let g = geometry_smith(n, v, l, roughness)
    let f = fresnel_schlick(max(dot(h, v), 0.0), f0)
    let spec_denom = 4.0 * ndotv * ndotl + 0.0001
    let specular = f * (d * g * (1.0 / spec_denom))
    let kd = (float3(1.0, 1.0, 1.0) - f) * (1.0 - metallic)
    let diffuse = kd * albedo * (1.0 / 3.14159265)
    return (diffuse + specular) * radiance * ndotl
}

def reflect3(i, nn : float3) : float3 {
    return i - nn * (2.0 * dot(nn, i))
}

def equirect_uv(d : float3) : float2 {
    let u = atan2(d.z, d.x) * 0.15915494 + 0.5
    let v = acos(clamp(d.y, -1.0, 1.0)) * 0.31830989
    return float2(u, v)
}

def pcf_shadow(uv : float2; ref : float) : float {
    let texel = 1.0 / SHADOW_DIM_F
    var sum = 0.0
    for (j in range(-2, 3)) {
        for (i in range(-2, 3)) {
            let off = float2(float(i), float(j)) * texel
            sum = sum + textureCompare(shadow_map, uv + off, ref)
        }
    }
    return sum * (1.0 / 25.0)
}

[fragment_program]
def lighting_fs {
    let albedo_occ = texture(lit_albedo_occ, v_uv)
    let normal_rough = texture(lit_normal_rough, v_uv)
    let worldpos_metal = texture(lit_worldpos_metal, v_uv)
    let emissive = texture(lit_emissive, v_uv).xyz
    let ssao_factor = texture(lit_ssao, v_uv).x
    let albedo = albedo_occ.xyz
    let occ_tex = albedo_occ.w
    let n_raw = normal_rough.xyz
    let roughness = normal_rough.w
    let p = worldpos_metal.xyz
    let metallic = worldpos_metal.w
    let is_bg = dot(n_raw, n_raw) < 0.01

    var out_col = float3(0.0, 0.0, 0.0)
    if (is_bg) {
        // linear background gradient (composite gamma-encodes it with everything else)
        let t = clamp(v_uv.y, 0.0, 1.0)
        out_col = lerp(float3(0.015, 0.015, 0.025), float3(0.07, 0.08, 0.12), float3(t, t, t))
    } else {
        let n = normalize(n_raw)
        let v = normalize(u_cam_pos - p)
        let f0 = lerp(float3(0.04, 0.04, 0.04), albedo, float3(metallic, metallic, metallic))

        // shadowed sun: light-space NDC -> shadow uv + depth ref (GL z in [-1,1] -> [0,1])
        let sun_dir = normalize(u_light_dir)
        let ndotl_sun = max(dot(n, sun_dir), 0.0)
        let lc = u_light_vp * float4(p, 1.0)
        let lw = max(lc.w, 0.0001)
        let ndc = float3(lc.x / lw, lc.y / lw, lc.z / lw)
        let shadow_uv = float2(ndc.x * 0.5 + 0.5, ndc.y * 0.5 + 0.5)
        let ref_depth = clamp(ndc.z * 0.5 + 0.5, 0.0, 1.0)
        let bias = max(0.0025 * (1.0 - ndotl_sun), 0.0009)
        var shadow = pcf_shadow(shadow_uv, ref_depth - bias)
        if (ndotl_sun < 0.05) {
            shadow = 0.0
        }
        let sun = cook_torrance(n, v, sun_dir, albedo, f0, roughness, metallic, float3(2.6, 2.4, 2.0)) * shadow

        // three orbiting coloured point lights, procedural from u_time (the deferred dividend:
        // each light is a cheap add over the screen, no per-light geometry)
        var pts = float3(0.0, 0.0, 0.0)
        for (i in range(3)) {
            let fi = float(i)
            let ang = u_time * 0.7 + fi * 2.0944
            let lp = float3(cos(ang) * 3.4, 1.8 + 0.6 * sin(u_time + fi), sin(ang) * 3.4)
            let lcol = float3(0.5 + 0.5 * cos(fi * 2.5), 0.5 + 0.5 * cos(fi * 2.5 + 2.0), 0.5 + 0.5 * cos(fi * 2.5 + 4.0))
            let lvec = lp - p
            let d = max(length(lvec), 0.0001)
            let ldir = lvec / float3(d, d, d)
            let atten = pow(max(1.0 - d / 6.5, 0.0), 2.0)
            pts = pts + cook_torrance(n, v, ldir, albedo, f0, roughness, metallic, lcol * 2.0) * atten
        }

        // ambient: hemisphere sky blended with the HDR equirect environment (diffuse along the
        // normal, Fresnel-weighted specular along the reflected view -- this is what gives the
        // metal parts real reflections). Modulated by baked occlusion * SSAO.
        let ao = clamp(min(occ_tex, ssao_factor), 0.0, 1.0)
        let amb_t = n.y * 0.5 + 0.5
        let hemi = lerp(float3(0.03, 0.03, 0.04), float3(0.12, 0.14, 0.20), float3(amb_t, amb_t, amb_t))
        let env_diff = texture(env_map, equirect_uv(n)).xyz
        let kd = 1.0 - metallic
        let ambient_diff = lerp(hemi, env_diff, float3(0.5, 0.5, 0.5)) * albedo * kd
        let inc = float3(-v.x, -v.y, -v.z)
        let rdir = reflect3(inc, n)
        let env_refl = texture(env_map, equirect_uv(rdir)).xyz
        let fres = fresnel_schlick(max(dot(n, v), 0.0), f0)
        let env_spec = env_refl * fres
        let ambient = (ambient_diff + env_spec) * ao

        out_col = ambient + (sun + pts) * (0.6 + 0.4 * ao)
    }
    // emissive is unlit and unshadowed; boosted well above 1.0 so it survives the bright pass.
    out_col = out_col + emissive * EMISSIVE_BOOST
    // LINEAR HDR out -- the composite pass does ACES tonemap + gamma, not this pass.
    post_color = float4(out_col, 1.0)
}

// ===== bloom passes (tutorial 11, verbatim) =====
var @uniform @stage = 0 src0        : sampler2D
var @uniform @stage = 1 src_bloom   : sampler2D
var @uniform u_threshold      : float
var @uniform u_soft_knee      : float
var @uniform u_src_rcp        : float2
var @uniform u_bloom_intensity : float

// Bright pass: Frostbite soft-knee threshold + Karis weight 1/(1+luma) so an above-1 emissive
// value is crushed near 1.0 and the bloom pyramid stays in a filterable range.
[fragment_program]
def bright_fs {
    let hdr = texture(src0, v_uv).xyz
    let luma = dot(hdr, float3(0.2126, 0.7152, 0.0722))
    let knee = max(u_soft_knee, 0.0001)
    let tt = clamp((luma - u_threshold + knee) / (2.0 * knee), 0.0, 1.0)
    let curve = tt * tt * (3.0 - 2.0 * tt)
    let karis_weight = 1.0 / (1.0 + luma)
    post_color = float4(hdr * karis_weight * curve, 1.0)
}

// Down pass: 5-tap Karis-bilinear (centre 0.5, four diagonals 0.125), each diagonal a 2x2 box.
[fragment_program]
def down_fs {
    let o = u_src_rcp
    let c = texture(src0, v_uv).xyz
    let lt = texture(src0, v_uv + float2(-o.x, -o.y)).xyz
    let rt = texture(src0, v_uv + float2(o.x, -o.y)).xyz
    let lb = texture(src0, v_uv + float2(-o.x, o.y)).xyz
    let rb = texture(src0, v_uv + float2(o.x, o.y)).xyz
    let sum = c * 0.5 + (lt + rt + lb + rb) * 0.125
    post_color = float4(sum, 1.0)
}

// Up pass: 9-tap tent (1-2-1 / 2-4-2 / 1-2-1, sum 16), host additively blends onto the larger mip.
[fragment_program]
def up_fs {
    let o = u_src_rcp
    let s00 = texture(src0, v_uv + float2(-o.x, -o.y)).xyz
    let s10 = texture(src0, v_uv + float2(0.0, -o.y)).xyz
    let s20 = texture(src0, v_uv + float2(o.x, -o.y)).xyz
    let s01 = texture(src0, v_uv + float2(-o.x, 0.0)).xyz
    let s11 = texture(src0, v_uv).xyz
    let s21 = texture(src0, v_uv + float2(o.x, 0.0)).xyz
    let s02 = texture(src0, v_uv + float2(-o.x, o.y)).xyz
    let s12 = texture(src0, v_uv + float2(0.0, o.y)).xyz
    let s22 = texture(src0, v_uv + float2(o.x, o.y)).xyz
    let tent = ((s00 + s20 + s02 + s22) * (1.0 / 16.0) +
        (s10 + s01 + s21 + s12) * (2.0 / 16.0) +
        s11 * (4.0 / 16.0))
    post_color = float4(tent, 1.0)
}

// ACES filmic fit (Narkowicz 2015): maps the unbounded HDR range into [0, 1].
def aces_fit(x : float3) : float3 {
    let a = float3(2.51, 2.51, 2.51)
    let b = float3(0.03, 0.03, 0.03)
    let c = float3(2.43, 2.43, 2.43)
    let d = float3(0.59, 0.59, 0.59)
    let e = float3(0.14, 0.14, 0.14)
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0))
}

[fragment_program]
def composite_fs {
    let hdr = texture(src0, v_uv).xyz
    let bloom = texture(src_bloom, v_uv).xyz
    let merged = hdr + bloom * u_bloom_intensity
    let mapped = aces_fit(merged)
    // the default GL / WebGL2 framebuffer does no sRGB encode, so gamma-encode here
    let g = 1.0 / 2.2
    let encoded = pow(mapped, float3(g, g, g))
    post_color = float4(encoded, 1.0)
}

// ===== host state =====

let MODEL = "tutorials/_assets/gltf/BoomBox.glb"
let HDRI = "tutorials/_assets/hdri/cannon_2k.hdr"
let RENDER_W = 1280
let RENDER_H = 720
let SHADOW_DIM = 1024
let BLOOM_MIPS = 5

var prog_shadow, prog_gbuffer, prog_ssao, prog_ssao_blur, prog_light : uint
var prog_bright, prog_down, prog_up, prog_composite : uint

var g_scene : GltfScene
var g_model : GltfGlModel
var g_root : float4x4

var floor_vao, floor_vbo, floor_ebo : uint
var quad_vao, quad_vbo : uint

var g_fbo, g_albedo, g_normal, g_worldpos, g_emissive_tex, g_depth : uint
var ssao_fbo, ssao_tex : uint
var ssao_blur_fbo, ssao_blur_tex : uint
var shadow_fbo, shadow_tex : uint
var hdr_fbo, hdr_color : uint
var tex_env : uint

var bloom_tex : uint[BLOOM_MIPS]
var bloom_fbo : uint[BLOOM_MIPS]
var bloom_w : int[BLOOM_MIPS]
var bloom_h : int[BLOOM_MIPS]

var window : GLFWwindow?
var time : float = 0.0

[vertex_buffer]
struct FloorVertex {
    pos     : float3
    normal  : float3
    tangent : float4
    uv      : float2
}

[vertex_buffer]
struct QuadVertex {
    xy : float2
}

// a large ground plane at y = 0 for the model to sit on (shadow / AO / reflection receiver)
let floor_verts = [FloorVertex(
    pos=float3(-8, 0, 8), normal=float3(0, 1, 0), tangent=float4(1, 0, 0, 1), uv=float2(0, 0)), FloorVertex(
    pos=float3(8, 0, 8), normal=float3(0, 1, 0), tangent=float4(1, 0, 0, 1), uv=float2(1, 0)), FloorVertex(
    pos=float3(8, 0, -8), normal=float3(0, 1, 0), tangent=float4(1, 0, 0, 1), uv=float2(1, 1)), FloorVertex(
    pos=float3(-8, 0, -8), normal=float3(0, 1, 0), tangent=float4(1, 0, 0, 1), uv=float2(0, 1)
)];
let floor_indices = fixed_array<int>(0, 1, 2, 0, 2, 3)

let quad = [QuadVertex(xy = float2(-1, -1)), QuadVertex(xy = float2(3, -1)), QuadVertex(xy = float2(-1, 3))];

// One RGBA16F colour texture attached to a fresh framebuffer; LINEAR + CLAMP so the bloom taps
// filter and never wrap. Returns (texture, fbo).
def make_hdr_color(w, h : int) : tuple<uint; uint> {
    var tex : uint
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_2D, tex)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGBA16F), w, h, 0, GL_RGBA, GL_HALF_FLOAT, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    var fbo : uint
    glGenFramebuffers(1, safe_addr(fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0)
    return (tex, fbo)
}

def make_color_attachment(w, h, internal : int; fmt, typ, attach, filt : uint) : uint {
    var tex : uint
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_2D, tex)
    glTexImage2D(GL_TEXTURE_2D, 0, internal, w, h, 0, fmt, typ, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filt)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filt)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glFramebufferTexture2D(GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, tex, 0)
    return tex
}

def load_hdr_texture(fname : string) : uint {
    var x, y, comp : int
    let data = stbi_loadf(fname, safe_addr(x), safe_addr(y), safe_addr(comp), 3)
    if (data == null) {
        panic("HDR load failed: " + fname)
    }
    var tex : uint
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_2D, tex)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_RGB16F), x, y, 0, GL_RGB, GL_FLOAT, unsafe(reinterpret<void?>(data)))
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    stbi_image_free(unsafe(reinterpret<void?>(data)))
    return tex
}

def create_shadow_target {
    glGenTextures(1, safe_addr(shadow_tex))
    glBindTexture(GL_TEXTURE_2D, shadow_tex)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_DEPTH_COMPONENT24), SHADOW_DIM, SHADOW_DIM, 0,
        GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL)
    glGenFramebuffers(1, safe_addr(shadow_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0)
    var none_buf = uint(GL_NONE)
    glDrawBuffers(1, safe_addr(none_buf))
    glReadBuffer(uint(GL_NONE))
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("shadow framebuffer incomplete")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def create_gbuffer {
    glGenFramebuffers(1, safe_addr(g_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo)
    g_albedo = make_color_attachment(RENDER_W, RENDER_H, int(GL_RGBA8), GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, GL_NEAREST)
    g_normal = make_color_attachment(RENDER_W, RENDER_H, int(GL_RGBA16F), GL_RGBA, GL_HALF_FLOAT, GL_COLOR_ATTACHMENT1, GL_NEAREST)
    g_worldpos = make_color_attachment(RENDER_W, RENDER_H, int(GL_RGBA16F), GL_RGBA, GL_HALF_FLOAT, GL_COLOR_ATTACHMENT2, GL_NEAREST)
    g_emissive_tex = make_color_attachment(RENDER_W, RENDER_H, int(GL_RGBA16F), GL_RGBA, GL_HALF_FLOAT, GL_COLOR_ATTACHMENT3, GL_NEAREST)
    glGenRenderbuffers(1, safe_addr(g_depth))
    glBindRenderbuffer(GL_RENDERBUFFER, g_depth)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, RENDER_W, RENDER_H)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_depth)
    var draw_bufs = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3)
    glDrawBuffers(4, safe_addr(draw_bufs[0]))
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("G-buffer framebuffer incomplete")
    }
    glGenFramebuffers(1, safe_addr(ssao_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo)
    ssao_tex = make_color_attachment(RENDER_W, RENDER_H, int(GL_RGBA8), GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, GL_LINEAR)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("SSAO framebuffer incomplete")
    }
    glGenFramebuffers(1, safe_addr(ssao_blur_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_blur_fbo)
    ssao_blur_tex = make_color_attachment(RENDER_W, RENDER_H, int(GL_RGBA8), GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, GL_LINEAR)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("SSAO blur framebuffer incomplete")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def create_hdr_and_bloom {
    let cf = make_hdr_color(RENDER_W, RENDER_H)
    hdr_color = cf._0
    hdr_fbo = cf._1
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("HDR framebuffer incomplete")
    }
    for (i in range(BLOOM_MIPS)) {
        bloom_w[i] = max(RENDER_W >> (i + 1), 1)
        bloom_h[i] = max(RENDER_H >> (i + 1), 1)
        let bf = make_hdr_color(bloom_w[i], bloom_h[i])
        bloom_tex[i] = bf._0
        bloom_fbo[i] = bf._1
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            panic("bloom mip framebuffer incomplete")
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

// Auto-frame: normalize the model to ~2 units tall, centred in x/z with its base on the floor.
def compute_root {
    let bounds = gltf_scene_bounds(g_scene)
    let bmin = bounds._0
    let bmax = bounds._1
    let center = (bmin + bmax) * 0.5
    let height = max(bmax.y - bmin.y, 0.0001)
    let s = 2.0 / height
    let offset = float3(center.x, bmin.y, center.z)
    g_root = compose(-offset * s, float4(0., 0., 0., 1.), float3(s, s, s))
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(RENDER_W, RENDER_H, "OpenGL - 12 glTF PBR capstone (BoomBox)", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)

    prog_shadow = create_shader_program(@@shadow_vs, @@shadow_fs)
    prog_gbuffer = create_shader_program(@@gbuffer_vs, @@gbuffer_fs)
    prog_ssao = create_shader_program(@@post_vs, @@ssao_fs)
    prog_ssao_blur = create_shader_program(@@post_vs, @@ssao_blur_fs)
    prog_light = create_shader_program(@@post_vs, @@lighting_fs)
    prog_bright = create_shader_program(@@post_vs, @@bright_fs)
    prog_down = create_shader_program(@@post_vs, @@down_fs)
    prog_up = create_shader_program(@@post_vs, @@up_fs)
    prog_composite = create_shader_program(@@post_vs, @@composite_fs)

    g_scene <- load_gltf(MODEL) // nolint:PERF030 — fresh at the one-time init
    if (empty(g_scene.meshes)) {
        panic("no meshes loaded from {MODEL}")
    }
    g_model <- upload_gltf(g_scene) // nolint:PERF030 — fresh at the one-time init
    compute_root()

    glGenVertexArrays(1, safe_addr(floor_vao))
    glBindVertexArray(floor_vao)
    glGenBuffers(1, safe_addr(floor_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo)
    glBufferData(GL_ARRAY_BUFFER, floor_verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<FloorVertex>)
    glGenBuffers(1, safe_addr(floor_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices, GL_STATIC_DRAW)

    glGenVertexArrays(1, safe_addr(quad_vao))
    glBindVertexArray(quad_vao)
    glGenBuffers(1, safe_addr(quad_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo)
    glBufferData(GL_ARRAY_BUFFER, quad, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<QuadVertex>)
    glBindVertexArray(0u)

    tex_env = load_hdr_texture(HDRI)
    create_shadow_target()
    create_gbuffer()
    create_hdr_and_bloom()
    print("glTF capstone loaded: {length(g_scene.meshes)} meshes, {length(g_scene.materials)} materials\n")
}

def bind_gltf_material(mat : GltfMaterial) {
    u_base_color_factor = mat.baseColorFactor
    u_metallic_factor = mat.metallicFactor
    u_roughness_factor = mat.roughnessFactor
    u_emissive_factor = mat.emissiveFactor
    u_normal_scale = mat.normalScale
    u_occlusion_strength = mat.occlusionStrength
    u_alpha_cutoff = mat.alphaCutoff
    u_alpha_mode = mat.alphaMode == GltfAlphaMode.mask ? 1.0 : (mat.alphaMode == GltfAlphaMode.blend ? 2.0 : 0.0)
    u_base_color_tex := gltf_gl_resolve_tex(g_model, mat.baseColorTex, g_model.whiteTex)
    u_metal_rough_tex := gltf_gl_resolve_tex(g_model, mat.metalRoughTex, g_model.whiteTex)
    u_normal_tex := gltf_gl_resolve_tex(g_model, mat.normalTex, g_model.flatNormalTex)
    u_occlusion_tex := gltf_gl_resolve_tex(g_model, mat.occlusionTex, g_model.whiteTex)
    u_emissive_tex := gltf_gl_resolve_tex(g_model, mat.emissiveTex, g_model.whiteTex)
    if (mat.doubleSided) {
        glDisable(GL_CULL_FACE)
    } else {
        glEnable(GL_CULL_FACE)
        glCullFace(GL_BACK)
    }
}

// Draw every static mesh instance in the scene into the currently-bound G-buffer. Rigid nodes
// only (this rung renders static models): u_model = root * node.world.
def draw_gltf_gbuffer {
    u_material = 0.0
    for (node in g_scene.nodes) {
        if (node.mesh < 0 || node.mesh >= length(g_model.meshes)) {
            continue
        }
        u_model = g_root * node.world
        u_normal_matrix = float3x3(transpose(inverse(u_model)))
        for (glp in g_model.meshes[node.mesh].primitives) {
            if (glp.material >= 0 && glp.material < length(g_scene.materials)) {
                bind_gltf_material(g_scene.materials[glp.material])
            } else {
                bind_gltf_material(GltfMaterial())
            }
            gbuffer_vs_bind_uniform(prog_gbuffer)
            gbuffer_fs_bind_uniform(prog_gbuffer)
            gltf_gl_draw_primitive(glp)
        }
    }
}

def draw_floor_gbuffer {
    u_material = 1.0
    u_model = identity4x4()
    u_normal_matrix = identity3x3()
    u_base_color_tex := g_model.whiteTex
    u_metal_rough_tex := g_model.whiteTex
    u_normal_tex := g_model.flatNormalTex
    u_occlusion_tex := g_model.whiteTex
    u_emissive_tex := g_model.whiteTex
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    gbuffer_vs_bind_uniform(prog_gbuffer)
    gbuffer_fs_bind_uniform(prog_gbuffer)
    glBindVertexArray(floor_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
}

def draw_depth {
    for (node in g_scene.nodes) {
        if (node.mesh < 0 || node.mesh >= length(g_model.meshes)) {
            continue
        }
        u_model = g_root * node.world
        shadow_vs_bind_uniform(prog_shadow)
        for (glp in g_model.meshes[node.mesh].primitives) {
            gltf_gl_draw_primitive(glp)
        }
    }
    u_model = identity4x4()
    shadow_vs_bind_uniform(prog_shadow)
    glBindVertexArray(floor_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
}

def draw_fullscreen {
    glBindVertexArray(quad_vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)
}

[export]
def update : bool { // nolint:STYLE038 — one frame of the tutorial pipeline
    time += 1.0 / 60.0
    let t = time
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)

    let cam_angle = t * 0.3
    let cam = float3(cos(cam_angle) * 5.2, 2.7, sin(cam_angle) * 5.2)
    u_view = look_at_rh(cam, float3(0.0, 1.0, 0.0), float3(0, 1, 0))
    u_proj = perspective_rh_minus1_to_1(50.0 * PI / 180.0, float(RENDER_W) / float(RENDER_H), 0.1, 60.0)
    u_cam_pos = cam
    let sun_yaw = t * 0.25
    let light_dir = normalize(float3(cos(sun_yaw) * 0.95, 0.72, sin(sun_yaw) * 0.95))
    u_light_dir = light_dir
    u_light_vp = ortho_rh(-6.0, 6.0, -6.0, 6.0, 0.1, 24.0) * look_at_rh(light_dir * 10.0, float3(0.0, 0.8, 0.0), float3(0, 1, 0))
    u_time = t

    // ===== Pass 1: shadow map from the sun's POV =====
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo)
    glViewport(0, 0, SHADOW_DIM, SHADOW_DIM)
    glClear(GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LESS)
    glDisable(GL_CULL_FACE)
    glEnable(GL_POLYGON_OFFSET_FILL)
    glPolygonOffset(2.0, 4.0)
    glUseProgram(prog_shadow)
    draw_depth()
    glDisable(GL_POLYGON_OFFSET_FILL)

    // ===== Pass 2: geometry -> G-buffer (4 MRT) =====
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo)
    glViewport(0, 0, RENDER_W, RENDER_H)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LESS)
    glDisable(GL_BLEND)
    glUseProgram(prog_gbuffer)
    draw_gltf_gbuffer()
    draw_floor_gbuffer()

    // ===== Pass 3: SSAO =====
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo)
    glViewport(0, 0, RENDER_W, RENDER_H)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glUseProgram(prog_ssao)
    ssao_g_normal := g_normal
    ssao_g_worldpos := g_worldpos
    post_vs_bind_uniform(prog_ssao)
    ssao_fs_bind_uniform(prog_ssao)
    draw_fullscreen()

    // ===== Pass 3b: separable Gaussian blur of the SSAO (denoise) =====
    glUseProgram(prog_ssao_blur)
    // horizontal: ssao_tex -> ssao_blur_tex (2-texel step widens the Gaussian)
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_blur_fbo)
    glViewport(0, 0, RENDER_W, RENDER_H)
    blur_src := ssao_tex
    blur_pos := g_worldpos
    u_blur_dir = float2(2.0 / float(RENDER_W), 0.0)
    post_vs_bind_uniform(prog_ssao_blur)
    ssao_blur_fs_bind_uniform(prog_ssao_blur)
    draw_fullscreen()
    // vertical: ssao_blur_tex -> ssao_tex (so the lighting pass reads the blurred result)
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo)
    glViewport(0, 0, RENDER_W, RENDER_H)
    blur_src := ssao_blur_tex
    blur_pos := g_worldpos
    u_blur_dir = float2(0.0, 2.0 / float(RENDER_H))
    post_vs_bind_uniform(prog_ssao_blur)
    ssao_blur_fs_bind_uniform(prog_ssao_blur)
    draw_fullscreen()

    // ===== Pass 4: deferred lighting -> HDR float target (linear) =====
    glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo)
    glViewport(0, 0, RENDER_W, RENDER_H)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glDisable(GL_DEPTH_TEST)
    glUseProgram(prog_light)
    lit_albedo_occ := g_albedo
    lit_normal_rough := g_normal
    lit_worldpos_metal := g_worldpos
    lit_emissive := g_emissive_tex
    lit_ssao := ssao_tex
    shadow_map := shadow_tex
    env_map := tex_env
    post_vs_bind_uniform(prog_light)
    lighting_fs_bind_uniform(prog_light)
    draw_fullscreen()

    // ===== Pass 5a: bright-pass threshold into bloom mip 0 =====
    glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[0])
    glViewport(0, 0, bloom_w[0], bloom_h[0])
    glUseProgram(prog_bright)
    src0 := hdr_color
    u_threshold = 1.0
    u_soft_knee = 0.6
    post_vs_bind_uniform(prog_bright)
    bright_fs_bind_uniform(prog_bright)
    draw_fullscreen()

    // ===== Pass 5b: downsample mip 0 -> 1 -> 2 -> 3 -> 4 =====
    glUseProgram(prog_down)
    for (i in range(BLOOM_MIPS - 1)) {
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[i + 1])
        glViewport(0, 0, bloom_w[i + 1], bloom_h[i + 1])
        src0 := bloom_tex[i]
        u_src_rcp = float2(1.0 / float(bloom_w[i]), 1.0 / float(bloom_h[i]))
        post_vs_bind_uniform(prog_down)
        down_fs_bind_uniform(prog_down)
        draw_fullscreen()
    }

    // ===== Pass 5c: upsample mip 4 -> 3 -> 2 -> 1 -> 0, additively blended =====
    glEnable(GL_BLEND)
    glBlendFunc(uint(GL_ONE), uint(GL_ONE))
    glUseProgram(prog_up)
    for (k in range(BLOOM_MIPS - 1)) {
        let i = BLOOM_MIPS - 1 - k
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[i - 1])
        glViewport(0, 0, bloom_w[i - 1], bloom_h[i - 1])
        src0 := bloom_tex[i]
        u_src_rcp = float2(1.0 / float(bloom_w[i]), 1.0 / float(bloom_h[i]))
        post_vs_bind_uniform(prog_up)
        up_fs_bind_uniform(prog_up)
        draw_fullscreen()
    }
    glDisable(GL_BLEND)

    // ===== Pass 6: composite HDR scene + bloom -> ACES tonemap -> gamma -> screen =====
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glUseProgram(prog_composite)
    src0 := hdr_color
    src_bloom := bloom_tex[0]
    u_bloom_intensity = 0.7
    post_vs_bind_uniform(prog_composite)
    composite_fs_bind_uniform(prog_composite)
    draw_fullscreen()

    glfwPollEvents()
    glfwSwapBuffers(window)
    return glfwWindowShouldClose(window) == 0
}

[export]
def shutdown {
    delete g_model
    delete g_scene
    glfwDestroyWindow(window)
    glfwTerminate()
}

var max_frame_limit = 0

def parse_args {
    let args <- get_command_line_arguments()
    for (i in range(length(args) - 1)) {
        if (args[i] == "--max-frames") {
            max_frame_limit = try_to_int(args[i + 1]) ?? 0   // garbage = no cap
        }
    }
}

// Desktop driver. On the web this is never called -- the run path drives init / update /
// shutdown once per browser frame and persists the Context across frames.
[export]
def main {
    parse_args()
    init()
    var frame_count = 0
    while (update()) {
        maybe_collect_gc()
        frame_count++
        if (max_frame_limit > 0 && frame_count >= max_frame_limit) {
            break
        }
    }
    shutdown()
}
