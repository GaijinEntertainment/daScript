options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require geometry/geom_gen
require stbimage
require math
require daslib/math_boost
require daslib/safe_addr

// 10 - Deferred Shading + MRT (OpenGL / WebGL2 tutorial)
//
// The "putting it all together" rung, a faithful port of the dasVulkan deferred
// tutorial: a concrete cat statue (CC0) on a brick floor, deferred-shaded with a real
// shadow map, screen-space ambient occlusion, and HDR image-based lighting. Its headline
// is MULTIPLE RENDER TARGETS (MRT) -- one geometry pass writes THREE colour attachments
// at once (the G-buffer) and later fullscreen passes read them back. Decoupling geometry
// from lighting is what lets deferred shading afford many lights: the per-pixel material
// work runs once, then each light is a cheap add over the screen.
//
// This is the first OpenGL rung to load EXTERNAL ASSETS -- a Wavefront OBJ mesh and PBR
// texture sets. The loading code (`load_obj_mesh`, `load_image_from_file`, `stbi_loadf`)
// is byte-identical to the desktop path: plain `fopen`. On the web the only difference is
// that the harness fetches the asset files into the in-memory filesystem first.
//
// Four passes (the real-world deferred architecture, every piece WebGL2-portable):
//   1. SHADOW   -> a depth-only render from the sun's POV into a depth texture.
//   2. GEOMETRY -> the 3-attachment G-buffer: albedo+spec (RGBA8), perturbed world
//                  normal+shininess (RGBA16F), world pos+material (RGBA16F). Normal
//                  mapping via a derivative-reconstructed TBN (no tangent buffer needed).
//   3. SSAO     -> a fullscreen pass that samples the G-buffer normal+position as
//                  textures (so it can read NEIGHBOURS) and computes a hemisphere-kernel
//                  occlusion factor that darkens crevices and the cat-floor contact.
//   4. LIGHTING -> a fullscreen pass that samples everything (G-buffer + SSAO + shadow
//                  map + HDR env) and accumulates one shadowed sun + three orbiting point
//                  lights + hemisphere ambient + image-based reflection. A filmstrip along
//                  the bottom previews the raw G-buffer + SSAO + shadow buffers.

// ===== shared mesh inputs (cat + floor: pos + normal + uv) =====
var @in @location = 0 a_pos : float3
var @in @location = 1 a_normal : float3
var @in @location = 2 a_uv : float2

// ===== geometry / shadow uniforms + varyings =====
var @uniform u_model : float4x4
var @uniform u_view : float4x4
var @uniform u_proj : float4x4
var @uniform u_light_vp : float4x4
var @uniform u_material : float            // 0 = cat, 1 = floor
var @inout gv_world_pos : float3
var @inout gv_world_normal : float3
var @inout gv_uv : float2

// The three G-buffer outputs (@out @location=N -> layout(location=N) out vec4 ...).
var @out @location = 0 g_albedo_out : float4
var @out @location = 1 g_normal_out : float4
var @out @location = 2 g_worldpos_out : float4

// cat + brick PBR maps; all bound together, the fragment branches on u_material.
var @uniform @stage = 0 cat_albedo : sampler2D
var @uniform @stage = 1 cat_normal : sampler2D
var @uniform @stage = 2 cat_arm : sampler2D       // AO(r) / Roughness(g) / Metallic(b)
var @uniform @stage = 3 floor_albedo : sampler2D
var @uniform @stage = 4 floor_normal : sampler2D
var @uniform @stage = 5 floor_ao : sampler2D
var @uniform @stage = 6 floor_roughness : sampler2D

// ----- shadow pass: depth-only from the sun's POV -----
[vertex_program]
def shadow_vs {
    gl_Position = u_light_vp * u_model * float4(a_pos, 1.0)
}

[fragment_program]
def shadow_fs {
    // depth-only framebuffer (colour draw buffer is GL_NONE) -- this is discarded.
    g_albedo_out = float4(1.0, 1.0, 1.0, 1.0)
}

// ----- geometry pass -----
[vertex_program]
def gbuffer_vs {
    let world = u_model * float4(a_pos, 1.0)
    gv_world_pos = world.xyz
    gv_world_normal = normalize((u_model * float4(a_normal, 0.0)).xyz)
    gv_uv = a_uv
    gl_Position = u_proj * u_view * world
}

// Derivative-based TBN (Schueler 2010): a tangent frame from screen-space derivatives of
// world position + uv, no per-vertex tangent buffer. dFdx/dFdy live in the shared module.
def perturb_normal(nrm, p : float3; uv : float2; tn : float3) : float3 {
    let dp1 = dFdx(p)
    let dp2 = dFdy(p)
    let duv1 = dFdx(uv)
    let duv2 = dFdy(uv)
    let dp2perp = cross(dp2, nrm)
    let dp1perp = cross(nrm, dp1)
    let T = dp2perp * duv1.x + dp1perp * duv2.x
    let B = dp2perp * duv1.y + dp1perp * duv2.y
    let invmax = 1.0 / sqrt(max(dot(T, T), dot(B, B)))
    return normalize(T * (tn.x * invmax) + B * (tn.y * invmax) + nrm * tn.z)
}

[fragment_program]
def gbuffer_fs {
    let n = normalize(gv_world_normal)
    var albedo = float3(0.0, 0.0, 0.0)
    var spec_i = 0.0
    var shininess = 32.0
    var perturbed = n
    // sRGB albedo textures -> linear so the deferred lighting is done in linear space
    // (the lighting pass gamma-encodes the final result, mirroring the Vulkan sRGB target).
    let sRGB = float3(2.2, 2.2, 2.2)
    if (u_material > 0.5) {
        let uv = float2(gv_world_pos.x, gv_world_pos.z) * 0.35
        let fb_ao = texture(floor_ao, uv).x
        albedo = pow(texture(floor_albedo, uv).rgb, sRGB) * (0.5 + 0.5 * fb_ao)
        spec_i = (1.0 - texture(floor_roughness, uv).x) * 0.4
        shininess = 24.0
        // boost the tangent xy so the mortar reads, then renormalize
        let tn_raw = texture(floor_normal, uv).rgb * 2.0 - float3(1.0, 1.0, 1.0)
        let tn = normalize(float3(tn_raw.x * 3.0, tn_raw.y * 3.0, tn_raw.z))
        perturbed = perturb_normal(n, gv_world_pos, uv, tn)
    } else {
        let alb = pow(texture(cat_albedo, gv_uv).rgb, sRGB)
        let arm = texture(cat_arm, gv_uv).rgb
        albedo = alb * (0.55 + 0.45 * arm.x)
        spec_i = (1.0 - arm.y) * 0.6
        shininess = 48.0
        let tn = normalize(texture(cat_normal, gv_uv).rgb * 2.0 - float3(1.0, 1.0, 1.0))
        perturbed = perturb_normal(n, gv_world_pos, gv_uv, tn)
    }
    g_albedo_out = float4(albedo, spec_i)
    g_normal_out = float4(perturbed, shininess)
    g_worldpos_out = float4(gv_world_pos, u_material)
}

// ===== fullscreen passes (SSAO + lighting share post_vs) =====
var @in @location = 0 q_pos : float2
var @inout l_uv : float2

[vertex_program]
def post_vs {
    gl_Position = float4(q_pos, 0.0, 1.0)
    l_uv = q_pos * 0.5 + float2(0.5, 0.5)
}

def smoothstep3(a, b, x : float) : float {
    let t = clamp((x - a) / (b - a), 0.0, 1.0)
    return t * t * (3.0 - 2.0 * t)
}

def hash21(p : float2) : float {
    let v = sin(dot(p, float2(127.1, 311.7))) * 43758.5453
    return v - floor(v)
}

// ----- SSAO pass -----
var @uniform @stage = 0 ssao_g_normal : sampler2D
var @uniform @stage = 1 ssao_g_worldpos : sampler2D
var @out ssao_out : float4

let SSAO_K = 16
let SSAO_RADIUS = 0.6
let SSAO_BIAS = 0.03
let SSAO_STRENGTH = 1.7

[fragment_program]
def ssao_fs {
    let n_raw = texture(ssao_g_normal, l_uv).xyz
    var ao = 1.0
    if (dot(n_raw, n_raw) >= 0.01) {
        let n = normalize(n_raw)
        let p = texture(ssao_g_worldpos, l_uv).xyz
        let cur_vz = (u_view * float4(p, 1.0)).z

        var up = float3(0.0, 1.0, 0.0)
        if (abs(n.y) > 0.95) {
            up = float3(1.0, 0.0, 0.0)
        }
        let tangent = normalize(cross(up, n))
        let bitangent = cross(n, tangent)
        let ang0 = hash21(l_uv * 1024.0) * 6.2831853

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
            // view looks down -Z: a stored surface closer to the camera (greater z) occludes
            if (clip.w > 0.0 && suv.x >= 0.0 && suv.x <= 1.0 && suv.y >= 0.0 && suv.y <= 1.0
                    && dot(stored_n, stored_n) > 0.01 && stored_vz >= sample_vz + SSAO_BIAS) {
                occ = occ + range_check
            }
        }
        ao = clamp(1.0 - SSAO_STRENGTH * occ / float(SSAO_K), 0.0, 1.0)
    }
    ssao_out = float4(ao, ao, ao, 1.0)
}

// ----- lighting pass -----
var @uniform @stage = 0 lit_albedo : sampler2D
var @uniform @stage = 1 lit_normal : sampler2D
var @uniform @stage = 2 lit_worldpos : sampler2D
var @uniform @stage = 3 lit_ssao : sampler2D
var @uniform @stage = 4 shadow_map : sampler2DShadow
var @uniform @stage = 5 env_map : sampler2D
var @uniform u_cam_pos : float3
var @uniform u_light_dir : float3
var @uniform u_time : float
var @out frag_color : float4

let SHADOW_DIM_F = 1024.0

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

def point_light(lp, lcol : float3; p, nrm, vdir : float3; shininess, spec_i, rng : float) : float3 {
    let lvec = lp - p
    let d = max(length(lvec), 0.0001)
    let ldir = lvec / float3(d, d, d)
    let ndotl = max(dot(nrm, ldir), 0.0)
    let atten = pow(max(1.0 - d / rng, 0.0), 2.0)
    let hv = normalize(ldir + vdir)
    let spec = pow(max(dot(nrm, hv), 0.0), shininess) * spec_i
    return lcol * (ndotl + spec) * atten
}

[fragment_program]
def lighting_fs {
    // ----- bottom filmstrip: raw G-buffer (albedo / normal / world-pos) + SSAO + shadow -----
    let TH = 0.18
    let GAP = 0.012
    let Y0 = 0.02
    let in_strip = l_uv.y >= Y0 && l_uv.y <= Y0 + TH
    let tv = (l_uv.y - Y0) / TH
    let x0 = GAP
    let x1 = GAP * 2.0 + TH
    let x2 = GAP * 3.0 + TH * 2.0
    let x3 = GAP * 4.0 + TH * 3.0

    let albedo_spec = texture(lit_albedo, l_uv)
    let normal_sh = texture(lit_normal, l_uv)
    let worldpos = texture(lit_worldpos, l_uv)
    let ssao_factor = texture(lit_ssao, l_uv).x
    let albedo = albedo_spec.rgb
    let spec_i = albedo_spec.a
    let shininess = normal_sh.a
    let p = worldpos.xyz
    let n_raw = normal_sh.xyz
    let is_bg = dot(n_raw, n_raw) < 0.01
    var n = float3(0.0, 1.0, 0.0)
    if (!is_bg) {
        n = normalize(n_raw)
    }

    var out_col = float3(0.0, 0.0, 0.0)
    if (in_strip && l_uv.x >= x0 && l_uv.x <= x0 + TH) {
        out_col = texture(lit_albedo, float2((l_uv.x - x0) / TH, tv)).rgb
    } elif (in_strip && l_uv.x >= x1 && l_uv.x <= x1 + TH) {
        let nn = texture(lit_normal, float2((l_uv.x - x1) / TH, tv)).xyz
        out_col = nn * 0.5 + float3(0.5, 0.5, 0.5)
    } elif (in_strip && l_uv.x >= x2 && l_uv.x <= x2 + TH) {
        let pp = texture(lit_worldpos, float2((l_uv.x - x2) / TH, tv)).xyz
        out_col = pp * 0.12 + float3(0.5, 0.5, 0.5)
    } elif (in_strip && l_uv.x >= x3 && l_uv.x <= x3 + TH) {
        let ao = texture(lit_ssao, float2((l_uv.x - x3) / TH, tv)).x
        out_col = float3(ao, ao, ao)
    } elif (is_bg) {
        let t = clamp(l_uv.y, 0.0, 1.0)
        out_col = lerp(float3(0.10, 0.10, 0.16), float3(0.34, 0.24, 0.26), float3(t, t, t))
    } else {
        let view_dir = normalize(u_cam_pos - p)

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
        let hv_sun = normalize(sun_dir + view_dir)
        let spec_sun = pow(max(dot(n, hv_sun), 0.0), shininess) * spec_i
        let sun = float3(1.25, 1.10, 0.80) * (ndotl_sun + spec_sun) * shadow

        // three orbiting coloured point lights, from u_time
        var pts = float3(0.0, 0.0, 0.0)
        for (i in range(3)) {
            let fi = float(i)
            let ang = u_time * 0.7 + fi * 2.0944
            let lp = float3(cos(ang) * 3.2, 1.6 + 0.6 * sin(u_time + fi), sin(ang) * 3.2)
            let lcol = float3(0.5 + 0.5 * cos(fi * 2.5), 0.5 + 0.5 * cos(fi * 2.5 + 2.0), 0.5 + 0.5 * cos(fi * 2.5 + 4.0))
            pts = pts + point_light(lp, lcol * 1.4, p, n, view_dir, shininess, spec_i, 5.5)
        }

        let ao = clamp(ssao_factor, 0.0, 1.0)
        let amb_t = n.y * 0.5 + 0.5
        let hemi = lerp(float3(0.08, 0.06, 0.05), float3(0.22, 0.24, 0.36), float3(amb_t, amb_t, amb_t))
        let env_amb = texture(env_map, equirect_uv(n)).rgb
        let ambient = lerp(hemi, env_amb, float3(0.4, 0.4, 0.4))

        let inc = float3(-view_dir.x, -view_dir.y, -view_dir.z)
        let rdir = reflect3(inc, n)
        let env_refl = texture(env_map, equirect_uv(rdir)).rgb
        let fres = pow(1.0 - max(dot(n, view_dir), 0.0), 4.0)
        let env_spec = env_refl * (spec_i * (0.10 + 0.7 * fres))

        let lighting = (ambient * ao) + (sun + pts) * (0.55 + 0.45 * ao) + env_spec * ao
        // linear -> sRGB for the display (the default GL framebuffer does no sRGB encode)
        let g = float3(0.4545, 0.4545, 0.4545)
        out_col = pow(albedo * lighting, g)
    }
    frag_color = float4(out_col, 1.0)
}

// ===== GL objects =====

var prog_shadow : uint
var prog_gbuffer : uint
var prog_ssao : uint
var prog_light : uint
var cat_vao, cat_vbo, cat_ebo : uint
var cat_index_count : int
var floor_vao, floor_vbo, floor_ebo : uint
var quad_vao, quad_vbo : uint
var g_fbo, g_albedo, g_normal, g_worldpos, g_depth : uint
var ssao_fbo, ssao_tex : uint
var shadow_fbo, shadow_tex : uint
var g_w : int = 0
var g_h : int = 0
var tex_cat_diff, tex_cat_nor, tex_cat_arm : uint
var tex_brick_color, tex_brick_nor, tex_brick_ao, tex_brick_rough : uint
var tex_env : uint
var window : GLFWwindow?
var time : float = 0.0

let SHADOW_DIM = 1024

let CAT_OBJ = "tutorials/_assets/cat/concrete_cat_statue.obj"
let CAT_DIFF = "tutorials/_assets/cat/textures/concrete_cat_statue_diff_2k.jpg"
let CAT_NOR = "tutorials/_assets/cat/textures/concrete_cat_statue_nor_gl_2k.jpg"
let CAT_ARM = "tutorials/_assets/cat/textures/concrete_cat_statue_arm_2k.jpg"
let BRICK_COLOR = "tutorials/_assets/brick/Bricks031_1K-JPG_Color.jpg"
let BRICK_NOR = "tutorials/_assets/brick/Bricks031_1K-JPG_NormalGL.jpg"
let BRICK_AO = "tutorials/_assets/brick/Bricks031_1K-JPG_AmbientOcclusion.jpg"
let BRICK_ROUGH = "tutorials/_assets/brick/Bricks031_1K-JPG_Roughness.jpg"
let HDRI = "tutorials/_assets/hdri/cannon_2k.hdr"

[vertex_buffer]
struct MeshVertex {
    pos : float3
    normal : float3
    uv : float2
}

def identity_m4 : float4x4 {
    var m : float4x4
    m[0] = float4(1, 0, 0, 0)
    m[1] = float4(0, 1, 0, 0)
    m[2] = float4(0, 0, 1, 0)
    m[3] = float4(0, 0, 0, 1)
    return m
}

let floor_verts = [MeshVertex(
    pos=float3(-6, 0, 6), normal=float3(0, 1, 0), uv=float2(0, 0)), MeshVertex(
    pos=float3(6, 0, 6), normal=float3(0, 1, 0), uv=float2(1, 0)), MeshVertex(
    pos=float3(6, 0, -6), normal=float3(0, 1, 0), uv=float2(1, 1)), MeshVertex(
    pos=float3(-6, 0, -6), normal=float3(0, 1, 0), uv=float2(0, 1)
)];
let floor_indices = fixed_array<int>(0, 1, 2, 0, 2, 3)

let quad = [MeshVertex(pos=float3(-1, -1, 0), normal=float3(), uv=float2()), MeshVertex(
    pos=float3(3, -1, 0), normal=float3(), uv=float2()), MeshVertex(
    pos=float3(-1, 3, 0), normal=float3(), uv=float2())];

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

def upload_cat {
    var frag <- load_obj_mesh(CAT_OBJ, false)
    if (empty(frag.vertices)) {
        panic("cat OBJ failed to load")
    }
    var bmin = float3(1.0e30, 1.0e30, 1.0e30)
    var bmax = float3(-1.0e30, -1.0e30, -1.0e30)
    for (gv in frag.vertices) {
        bmin = min(bmin, gv.xyz)
        bmax = max(bmax, gv.xyz)
    }
    let center = (bmin + bmax) * 0.5
    let s = 2.2 / max(bmax.y - bmin.y, 0.001)
    var verts : array<MeshVertex>
    verts |> reserve(length(frag.vertices))
    for (gv in frag.vertices) {
        let pp = float3((gv.xyz.x - center.x) * s, (gv.xyz.y - bmin.y) * s, (gv.xyz.z - center.z) * s)
        verts |> push(MeshVertex(pos = pp, normal = gv.normal, uv = gv.uv))
    }
    cat_index_count = length(frag.indices)
    glGenVertexArrays(1, safe_addr(cat_vao))
    glBindVertexArray(cat_vao)
    glGenBuffers(1, safe_addr(cat_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, cat_vbo)
    glBufferData(GL_ARRAY_BUFFER, verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<MeshVertex>)
    glGenBuffers(1, safe_addr(cat_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cat_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, frag.indices, GL_STATIC_DRAW)
    delete verts
    delete frag
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

def setup_targets(w, h : int) {
    if (g_fbo != 0u) {
        glDeleteFramebuffers(1, safe_addr(g_fbo))
        glDeleteTextures(1, safe_addr(g_albedo))
        glDeleteTextures(1, safe_addr(g_normal))
        glDeleteTextures(1, safe_addr(g_worldpos))
        glDeleteRenderbuffers(1, safe_addr(g_depth))
        glDeleteFramebuffers(1, safe_addr(ssao_fbo))
        glDeleteTextures(1, safe_addr(ssao_tex))
    }
    // G-buffer: 3 MRT + depth
    glGenFramebuffers(1, safe_addr(g_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo)
    g_albedo = make_color_attachment(w, h, int(GL_RGBA8), GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, GL_NEAREST)
    g_normal = make_color_attachment(w, h, int(GL_RGBA16F), GL_RGBA, GL_HALF_FLOAT, GL_COLOR_ATTACHMENT1, GL_NEAREST)
    g_worldpos = make_color_attachment(w, h, int(GL_RGBA16F), GL_RGBA, GL_HALF_FLOAT, GL_COLOR_ATTACHMENT2, GL_NEAREST)
    glGenRenderbuffers(1, safe_addr(g_depth))
    glBindRenderbuffer(GL_RENDERBUFFER, g_depth)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_depth)
    var draw_bufs = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2)
    glDrawBuffers(3, safe_addr(draw_bufs[0]))
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("G-buffer framebuffer incomplete")
    }
    // SSAO: single colour
    glGenFramebuffers(1, safe_addr(ssao_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo)
    ssao_tex = make_color_attachment(w, h, int(GL_RGBA8), GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0, GL_LINEAR)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("SSAO framebuffer incomplete")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    g_w = w
    g_h = h
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

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 10 deferred shading (cat on brick)", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    prog_shadow = create_shader_program(@@shadow_vs, @@shadow_fs)
    prog_gbuffer = create_shader_program(@@gbuffer_vs, @@gbuffer_fs)
    prog_ssao = create_shader_program(@@post_vs, @@ssao_fs)
    prog_light = create_shader_program(@@post_vs, @@lighting_fs)

    upload_cat()
    glGenVertexArrays(1, safe_addr(floor_vao))
    glBindVertexArray(floor_vao)
    glGenBuffers(1, safe_addr(floor_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo)
    glBufferData(GL_ARRAY_BUFFER, floor_verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<MeshVertex>)
    glGenBuffers(1, safe_addr(floor_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices, GL_STATIC_DRAW)
    glGenVertexArrays(1, safe_addr(quad_vao))
    glBindVertexArray(quad_vao)
    glGenBuffers(1, safe_addr(quad_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo)
    glBufferData(GL_ARRAY_BUFFER, quad, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<MeshVertex>)

    tex_cat_diff = load_image_from_file(CAT_DIFF, false)
    tex_cat_nor = load_image_from_file(CAT_NOR, false)
    tex_cat_arm = load_image_from_file(CAT_ARM, false)
    tex_brick_color = load_image_from_file(BRICK_COLOR, false)
    tex_brick_nor = load_image_from_file(BRICK_NOR, false)
    tex_brick_ao = load_image_from_file(BRICK_AO, false)
    tex_brick_rough = load_image_from_file(BRICK_ROUGH, false)
    tex_env = load_hdr_texture(HDRI)

    create_shadow_target()
    setup_targets(640, 480)
    print("deferred assets loaded: cat={cat_index_count} idx, env={tex_env}\n")
}

def draw_scene_geometry {
    u_model = identity_m4()
    u_material = 1.0
    gbuffer_vs_bind_uniform(prog_gbuffer)
    gbuffer_fs_bind_uniform(prog_gbuffer)
    glBindVertexArray(floor_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
    u_material = 0.0
    gbuffer_vs_bind_uniform(prog_gbuffer)
    gbuffer_fs_bind_uniform(prog_gbuffer)
    glBindVertexArray(cat_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cat_ebo)
    glDrawElements(GL_TRIANGLES, cat_index_count, GL_UNSIGNED_INT, null)
}

[export]
def update : bool {
    time += 1.0 / 60.0
    let t = time
    var dw, dh : int
    glfwGetFramebufferSize(window, dw, dh)
    let w = max(dw, 1)
    let h = max(dh, 1)
    if (w != g_w || h != g_h) {
        setup_targets(w, h)
    }

    let cam_angle = t * 0.3
    let cam = float3(cos(cam_angle) * 4.6, 2.3, sin(cam_angle) * 4.6)
    u_view = look_at_rh(cam, float3(0.0, 1.0, 0.0), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(50.0 * PI / 180.0, float(w) / float(h), 0.1, 60.0)
    u_cam_pos = cam
    let sun_yaw = t * 0.25
    let light_dir = normalize(float3(cos(sun_yaw) * 0.6, 1.05, sin(sun_yaw) * 0.6))
    u_light_dir = light_dir
    u_light_vp = ortho_rh(-4.0, 4.0, -4.0, 4.0, 0.1, 20.0) * look_at_rh(light_dir * 8.0, float3(0.0, 0.8, 0.0), float3(0, 1, 0))
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
    u_model = identity_m4()
    shadow_vs_bind_uniform(prog_shadow)
    glBindVertexArray(floor_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
    glBindVertexArray(cat_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cat_ebo)
    glDrawElements(GL_TRIANGLES, cat_index_count, GL_UNSIGNED_INT, null)
    glDisable(GL_POLYGON_OFFSET_FILL)

    // ===== Pass 2: geometry -> G-buffer (MRT) =====
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo)
    glViewport(0, 0, g_w, g_h)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LESS)
    glDisable(GL_CULL_FACE)
    glDisable(GL_BLEND)
    glUseProgram(prog_gbuffer)
    cat_albedo := tex_cat_diff
    cat_normal := tex_cat_nor
    cat_arm := tex_cat_arm
    floor_albedo := tex_brick_color
    floor_normal := tex_brick_nor
    floor_ao := tex_brick_ao
    floor_roughness := tex_brick_rough
    draw_scene_geometry()

    // ===== Pass 3: SSAO -> single-channel target =====
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo)
    glViewport(0, 0, g_w, g_h)
    glDisable(GL_DEPTH_TEST)
    glUseProgram(prog_ssao)
    ssao_g_normal := g_normal
    ssao_g_worldpos := g_worldpos
    post_vs_bind_uniform(prog_ssao)
    ssao_fs_bind_uniform(prog_ssao)
    glBindVertexArray(quad_vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)

    // ===== Pass 4: deferred lighting -> screen =====
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glViewport(0, 0, w, h)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glUseProgram(prog_light)
    lit_albedo := g_albedo
    lit_normal := g_normal
    lit_worldpos := g_worldpos
    lit_ssao := ssao_tex
    shadow_map := shadow_tex
    env_map := tex_env
    post_vs_bind_uniform(prog_light)
    lighting_fs_bind_uniform(prog_light)
    glBindVertexArray(quad_vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)

    glfwPollEvents()
    glfwSwapBuffers(window)
    return glfwWindowShouldClose(window) == 0
}

[export]
def shutdown {
    glfwDestroyWindow(window)
    glfwTerminate()
}

[export]
def main {
    init()
    while (update()) {
    }
    shutdown()
}
