options gen2
options persistent_heap
options indenting = 4

require opengl/opengl_boost public
require opengl/opengl_cache public
// No live_vars here: without it `@live` is inert metadata, which is what the
// playground wants -- there is no reload to persist across.
require daslib/math_boost public
require daslib/safe_addr public

// Offscreen render targets and the screen-space chain that turns the raw scene
// pass into the finished frame: ambient occlusion, bloom, sun shafts, filmic
// tone mapping, vignette, grain and FXAA.
//
// Everything is held to GLSL ES 3.00 / WebGL2. The one capability that is not
// guaranteed there is a float-renderable colour attachment, so the HDR targets
// are probed at creation with glCheckFramebufferStatus and silently fall back to
// RGBA8 (bloom then works off a pre-exposed LDR buffer -- dimmer, never broken).

let SHADOW_SIZE = 2048
let BLOOM_LEVELS = 4

// Render-target handles live across a live reload, so init() reuses them
// instead of orphaning a set of GL objects the reloaded script can no longer
// name.
var @live fx_width = 0
var @live fx_height = 0
var @live fx_hdr = false

var @live scene_fbo = 0u
var @live scene_color = 0u
var @live scene_nd = 0u
var @live scene_depth_rb = 0u

var @live shadow_fbo = 0u
var @live shadow_tex = 0u

var @live ldr_fbo = 0u
var @live ldr_tex = 0u

var @live ao_fbo = 0u
var @live ao_tex = 0u
var @live ao_blur_fbo = 0u
var @live ao_blur_tex = 0u

var @live shaft_fbo = 0u
var @live shaft_tex = 0u
var @live occl_fbo = 0u
var @live occl_tex = 0u

var @live bloom_fbo : array<uint>
var @live bloom_tex : array<uint>
var @live bloom_w : array<int>
var @live bloom_h : array<int>

var @live empty_vao = 0u

// ============================================================================
// Post-process shader interface
// ============================================================================

var @inout p_uv : float2

var @uniform @stage = 0 p_tex0 : sampler2D
var @uniform @stage = 1 p_tex1 : sampler2D
var @uniform @stage = 2 p_tex2 : sampler2D
var @uniform p_texel : float2
var @uniform p_params : float4
var @uniform p_params2 : float4
var @uniform p_tint : float3

var @out p_FragColor : float4

// One attributeless triangle covering the viewport; cheaper than a quad and
// avoids the diagonal seam.
[vertex_program]
def vs_post {
    let id = gl_VertexIndex
    p_uv = float2(float((id << 1) & 2), float(id & 2))
    gl_Position = float4(p_uv * 2.0 - float2(1.0), 0.0, 1.0)
}

// --- Bright pass: soft-knee threshold, so highlights ramp in instead of popping

[fragment_program]
def fs_bright {
    let c = texture(p_tex0, p_uv).xyz
    let luma = dot(c, float3(0.2126, 0.7152, 0.0722))
    let threshold = p_params.x
    let knee = p_params.y
    let soft = clamp(luma - threshold + knee, 0.0, 2.0 * knee)
    let contrib = max(soft * soft / (4.0 * knee + 0.0001), luma - threshold) / max(luma, 0.0001)
    p_FragColor = float4(c * contrib, 1.0)
}

// --- Progressive downsample (13-tap box, the standard dual-filter kernel)

[fragment_program]
def fs_down {
    let t = p_texel
    var sum = texture(p_tex0, p_uv).xyz * 0.125
    sum += texture(p_tex0, p_uv + float2(-t.x, -t.y)).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(0.0, -t.y)).xyz * 0.125
    sum += texture(p_tex0, p_uv + float2(t.x, -t.y)).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(-t.x, 0.0)).xyz * 0.125
    sum += texture(p_tex0, p_uv + float2(t.x, 0.0)).xyz * 0.125
    sum += texture(p_tex0, p_uv + float2(-t.x, t.y)).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(0.0, t.y)).xyz * 0.125
    sum += texture(p_tex0, p_uv + float2(t.x, t.y)).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(-t.x, -t.y) * 0.5).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(t.x, -t.y) * 0.5).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(-t.x, t.y) * 0.5).xyz * 0.0625
    sum += texture(p_tex0, p_uv + float2(t.x, t.y) * 0.5).xyz * 0.0625
    p_FragColor = float4(sum, 1.0)
}

// --- Tent upsample, additively blended onto the next larger level

// p_params.x is the filter RADIUS, p_params.y the output intensity. They have to
// stay separate: folding intensity into the radius makes a strength of zero a
// zero-width tap that still contributes at full brightness.
[fragment_program]
def fs_up {
    let t = p_texel * p_params.x
    var sum = texture(p_tex0, p_uv).xyz * 4.0
    sum += texture(p_tex0, p_uv + float2(-t.x, 0.0)).xyz * 2.0
    sum += texture(p_tex0, p_uv + float2(t.x, 0.0)).xyz * 2.0
    sum += texture(p_tex0, p_uv + float2(0.0, -t.y)).xyz * 2.0
    sum += texture(p_tex0, p_uv + float2(0.0, t.y)).xyz * 2.0
    sum += texture(p_tex0, p_uv + float2(-t.x, -t.y)).xyz
    sum += texture(p_tex0, p_uv + float2(t.x, -t.y)).xyz
    sum += texture(p_tex0, p_uv + float2(-t.x, t.y)).xyz
    sum += texture(p_tex0, p_uv + float2(t.x, t.y)).xyz
    p_FragColor = float4(sum * (1.0 / 16.0) * p_params.y, 1.0)
}

// --- Ambient occlusion
//
// Hemisphere sampling in view space, reconstructed from the linear depth packed
// in the scene normal target's alpha. Half resolution; the blur below hides the
// sample noise.

def private view_from_uv(uv : float2; depth : float) : float3 {
    let ndc = uv * 2.0 - float2(1.0)
    return float3(ndc.x * p_params2.x * depth, ndc.y * p_params2.y * depth, -depth)
}

[fragment_program]
def fs_ssao {
    let nd = texture(p_tex0, p_uv)
    let depth = nd.w * p_params2.z
    if (nd.w >= 0.999) {
        p_FragColor = float4(1.0)
        return
    }
    let origin = view_from_uv(p_uv, depth)
    let nrm = normalize(nd.xyz * 2.0 - float3(1.0))
    let radius = p_params.x
    let rot = hash_noise(p_uv * p_params.z) * 6.2831853

    var occlusion = 0.0
    let taps = 12
    for (i in range(taps)) {
        let fi = float(i)
        // Golden-angle spiral, rotated per pixel, with the radius growing as
        // sqrt so samples stay area-uniform across the hemisphere.
        let ang = rot + fi * 2.399963
        let rad = radius * sqrt((fi + 0.5) / float(taps))
        let dir = float3(cos(ang) * rad, sin(ang) * rad, 0.0)
        var sample_pos = origin + dir
        sample_pos += nrm * rad * 0.65

        let clip = p_params2.w
        let su = float2(sample_pos.x / (p_params2.x * -sample_pos.z), sample_pos.y / (p_params2.y * -sample_pos.z))
        let suv = su * 0.5 + float2(0.5)
        if (suv.x < 0.0 || suv.x > 1.0 || suv.y < 0.0 || suv.y > 1.0) {
            continue
        }
        let sample_depth = texture(p_tex0, suv).w * p_params2.z
        let delta = -sample_pos.z - sample_depth
        let range_check = saturate(radius / max(abs(-origin.z - sample_depth), 0.0001))
        if (delta > 0.02 * clip) {
            occlusion += range_check
        }
    }
    let ao = saturate(1.0 - occlusion / float(taps) * p_params.y)
    p_FragColor = float4(ao, ao, ao, 1.0)
}

def private hash_noise(p : float2) : float {
    return (p.x * 0.0 + fract_local(sin(dot(floor(p), float2(12.9898, 78.233))) * 43758.545))
}

def private fract_local(x : float) : float {
    return x - floor(x)
}

// --- Cross-shaped AO blur (cheap, and the AO signal is already low frequency)

[fragment_program]
def fs_ao_blur {
    var sum = 0.0
    for (y in range(-2, 3)) {
        for (x in range(-2, 3)) {
            sum += texture(p_tex0, p_uv + float2(float(x), float(y)) * p_texel).x
        }
    }
    p_FragColor = float4(float3(sum * (1.0 / 25.0)), 1.0)
}

// --- Sun shafts
//
// Extract the parts of the frame that are sky (linear depth at the far plane)
// and near the sun on screen, then radially blur that buffer away from the sun
// position. Classic screen-space light scattering; no extra geometry.

[fragment_program]
def fs_occlusion {
    let nd = texture(p_tex1, p_uv)
    let sky = step(0.995, nd.w)
    let c = texture(p_tex0, p_uv).xyz
    let d = distance(p_uv, p_params.xy)
    let falloff = saturate(1.0 - d * p_params.z)
    p_FragColor = float4(c * sky * falloff * falloff, 1.0)
}

[fragment_program]
def fs_shafts {
    let sun_uv = p_params.xy
    let density = p_params.z
    var uv = p_uv
    let delta = (uv - sun_uv) * (density / 24.0)
    var illum = 1.0
    var sum = float3(0.0)
    for (_i in range(24)) {
        uv -= delta
        sum += texture(p_tex0, uv).xyz * illum
        illum *= p_params.w
    }
    p_FragColor = float4(sum * (1.0 / 24.0), 1.0)
}

// --- Composite: AO, bloom, shafts, exposure, ACES tone map, grade, vignette

def private aces_tonemap(x : float3) : float3 {
    let a = 2.51
    let b = 0.03
    let c = 2.43
    let d = 0.59
    let e = 0.14
    return saturate((x * (a * x + float3(b))) / (x * (c * x + float3(d)) + float3(e)))
}

[fragment_program]
def fs_composite {
    var color = texture(p_tex0, p_uv).xyz
    let ao = texture(p_tex1, p_uv).x
    let bloom = texture(p_tex2, p_uv).xyz

    // AO only darkens ambient-lit areas; applying it to the whole signal eats
    // the sun highlights and reads as dirt.
    color *= lerp(1.0, ao, p_params.z)
    color += bloom * p_params.y
    color *= p_params.x

    var mapped = aces_tonemap(color)

    // Grade: lift the shadows toward the section's fog colour, push contrast.
    mapped = lerp(mapped, mapped * mapped * (float3(3.0) - 2.0 * mapped), p_params.w)
    mapped = lerp(mapped, p_tint * dot(mapped, float3(0.2126, 0.7152, 0.0722)), p_params2.x)

    p_FragColor = float4(mapped, 1.0)
}

// --- FXAA + vignette + grain, straight to the back buffer

def private luma(c : float3) : float {
    return dot(c, float3(0.299, 0.587, 0.114))
}

[fragment_program]
def fs_present {
    let t = p_texel
    let rgb_m = texture(p_tex0, p_uv).xyz
    let l_nw = luma(texture(p_tex0, p_uv + float2(-t.x, -t.y)).xyz)
    let l_ne = luma(texture(p_tex0, p_uv + float2(t.x, -t.y)).xyz)
    let l_sw = luma(texture(p_tex0, p_uv + float2(-t.x, t.y)).xyz)
    let l_se = luma(texture(p_tex0, p_uv + float2(t.x, t.y)).xyz)
    let l_m = luma(rgb_m)

    let l_min = min(l_m, min(min(l_nw, l_ne), min(l_sw, l_se)))
    let l_max = max(l_m, max(max(l_nw, l_ne), max(l_sw, l_se)))

    var dir = float2(-((l_nw + l_ne) - (l_sw + l_se)), ((l_nw + l_sw) - (l_ne + l_se)))
    let reduce = max((l_nw + l_ne + l_sw + l_se) * 0.03125, 0.0078125)
    let rcp = 1.0 / (min(abs(dir.x), abs(dir.y)) + reduce)
    dir = clamp(dir * rcp, float2(-8.0), float2(8.0)) * t

    let rgb_a = 0.5 * (texture(p_tex0, p_uv + dir * (1.0 / 3.0 - 0.5)).xyz
        + texture(p_tex0, p_uv + dir * (2.0 / 3.0 - 0.5)).xyz)
    let rgb_b = rgb_a * 0.5 + 0.25 * (texture(p_tex0, p_uv + dir * -0.5).xyz
        + texture(p_tex0, p_uv + dir * 0.5).xyz)
    let l_b = luma(rgb_b)
    var color = ((l_b < l_min) || (l_b > l_max)) ? rgb_a : rgb_b

    // Vignette, then a touch of chromatic falloff at the very edge.
    let center = p_uv - float2(0.5)
    let r2 = dot(center, center)
    color *= 1.0 - saturate(r2 * p_params.x) * p_params.y

    // Animated grain, scaled down in the bright parts so it stays filmic.
    let grain = fract_local(sin(dot(p_uv * p_params2.y + float2(p_params.w), float2(12.9898, 78.233))) * 43758.545) - 0.5
    color += float3(grain * p_params.z * (1.0 - luma(color) * 0.6))

    p_FragColor = float4(color, 1.0)
}

// ============================================================================
// Program handles
// ============================================================================

var bright_program : uint
var down_program : uint
var up_program : uint
var ssao_program : uint
var ao_blur_program : uint
var occlusion_program : uint
var shafts_program : uint
var composite_program : uint
var present_program : uint

def create_postfx_programs() {
    bright_program = cache_shader_program(vs_post`shader_text, fs_bright`shader_text)
    down_program = cache_shader_program(vs_post`shader_text, fs_down`shader_text)
    up_program = cache_shader_program(vs_post`shader_text, fs_up`shader_text)
    ssao_program = cache_shader_program(vs_post`shader_text, fs_ssao`shader_text)
    ao_blur_program = cache_shader_program(vs_post`shader_text, fs_ao_blur`shader_text)
    occlusion_program = cache_shader_program(vs_post`shader_text, fs_occlusion`shader_text)
    shafts_program = cache_shader_program(vs_post`shader_text, fs_shafts`shader_text)
    composite_program = cache_shader_program(vs_post`shader_text, fs_composite`shader_text)
    present_program = cache_shader_program(vs_post`shader_text, fs_present`shader_text)
}

// ============================================================================
// Render targets
// ============================================================================

def private make_texture(w, h : int; internal_format : uint; format, data_type, filter : uint) : uint {
    var tex = 0u
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_2D, tex)
    glTexImage2D(GL_TEXTURE_2D, 0, int(internal_format), w, h, 0, format, data_type, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter)
    // GLES 3.0 has no CLAMP_TO_BORDER, so every target clamps to edge and the
    // shaders range-check explicitly where a border would have mattered.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    return tex
}

def private drop_texture(var tex : uint) {
    if (tex != 0u) {
        glDeleteTextures(1, unsafe(addr(tex)))
        tex = 0u
    }
}

def private drop_fbo(var fbo : uint) {
    if (fbo != 0u) {
        glDeleteFramebuffers(1, unsafe(addr(fbo)))
        fbo = 0u
    }
}

def private make_color_fbo(tex : uint) : uint {
    var fbo = 0u
    glGenFramebuffers(1, safe_addr(fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0)
    return fbo
}

// Build the two-attachment scene target at the requested colour format and
// report whether the driver will actually render to it. RGBA16F is not a
// guaranteed colour-renderable format on WebGL2 (it needs EXT_color_buffer_float),
// so the caller retries at RGBA8 rather than assuming.
def private try_build_scene(w, h : int; color_format, color_type : uint) : bool {
    scene_color = make_texture(w, h, color_format, GL_RGBA, color_type, GL_LINEAR)
    scene_nd = make_texture(w, h, color_format, GL_RGBA, color_type, GL_NEAREST)
    glGenRenderbuffers(1, safe_addr(scene_depth_rb))
    glBindRenderbuffer(GL_RENDERBUFFER, scene_depth_rb)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h)

    glGenFramebuffers(1, safe_addr(scene_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_color, 0)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, scene_nd, 0)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, scene_depth_rb)
    var targets = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1)
    glDrawBuffers(2, unsafe(addr(targets[0])))
    let ok = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    if (!ok) {
        drop_fbo(scene_fbo)
        drop_texture(scene_color)
        drop_texture(scene_nd)
        glDeleteRenderbuffers(1, safe_addr(scene_depth_rb))
        scene_depth_rb = 0u
    }
    return ok
}

def destroy_render_targets() {
    drop_fbo(scene_fbo)
    drop_texture(scene_color)
    drop_texture(scene_nd)
    if (scene_depth_rb != 0u) {
        glDeleteRenderbuffers(1, safe_addr(scene_depth_rb))
        scene_depth_rb = 0u
    }
    drop_fbo(ldr_fbo)
    drop_texture(ldr_tex)
    drop_fbo(ao_fbo)
    drop_texture(ao_tex)
    drop_fbo(ao_blur_fbo)
    drop_texture(ao_blur_tex)
    drop_fbo(shaft_fbo)
    drop_texture(shaft_tex)
    drop_fbo(occl_fbo)
    drop_texture(occl_tex)
    for (fbo, tex in bloom_fbo, bloom_tex) {
        drop_fbo(fbo)
        drop_texture(tex)
    }
    bloom_fbo |> clear()
    bloom_tex |> clear()
    bloom_w |> clear()
    bloom_h |> clear()
    fx_width = 0
    fx_height = 0
}

def private ensure_shadow_target() {
    if (shadow_fbo != 0u) {
        return
    }
    shadow_tex = make_texture(SHADOW_SIZE, SHADOW_SIZE, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, GL_LINEAR)
    // Hardware PCF: the sampler returns the comparison result, so a single
    // textureCompare tap is already 2x2 filtered.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL)

    glGenFramebuffers(1, safe_addr(shadow_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0)
    var none_target = fixed_array(0u)
    glDrawBuffers(1, unsafe(addr(none_target[0])))
    glReadBuffer(uint(GL_NONE))
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def ensure_render_targets(w, h : int) {
    if (w <= 0 || h <= 0) {
        return
    }
    if (empty_vao == 0u) {
        glGenVertexArrays(1, safe_addr(empty_vao))
    }
    ensure_shadow_target()
    if (w == fx_width && h == fx_height && scene_fbo != 0u) {
        return
    }
    destroy_render_targets()
    fx_width = w
    fx_height = h

    fx_hdr = try_build_scene(w, h, GL_RGBA16F, GL_HALF_FLOAT)
    if (!fx_hdr) {
        to_log(LOG_WARNING, "river_run: no float colour attachment, post chain runs at 8 bits\n")
        if (!try_build_scene(w, h, GL_RGBA8, GL_UNSIGNED_BYTE)) {
            to_log(LOG_ERROR, "river_run: scene framebuffer is incomplete\n")
            return
        }
    }
    let color_format = (fx_hdr ? GL_RGBA16F : GL_RGBA8)
    let color_type = (fx_hdr ? GL_HALF_FLOAT : GL_UNSIGNED_BYTE)

    ldr_tex = make_texture(w, h, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR)
    ldr_fbo = make_color_fbo(ldr_tex)

    let aw = max(w / 2, 1)
    let ah = max(h / 2, 1)
    ao_tex = make_texture(aw, ah, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR)
    ao_fbo = make_color_fbo(ao_tex)
    ao_blur_tex = make_texture(aw, ah, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR)
    ao_blur_fbo = make_color_fbo(ao_blur_tex)

    let sw = max(w / 4, 1)
    let sh = max(h / 4, 1)
    occl_tex = make_texture(sw, sh, color_format, GL_RGBA, color_type, GL_LINEAR)
    occl_fbo = make_color_fbo(occl_tex)
    shaft_tex = make_texture(sw, sh, color_format, GL_RGBA, color_type, GL_LINEAR)
    shaft_fbo = make_color_fbo(shaft_tex)

    var lw = w
    var lh = h
    bloom_tex |> reserve(BLOOM_LEVELS)
    bloom_fbo |> reserve(BLOOM_LEVELS)
    bloom_w |> reserve(BLOOM_LEVELS)
    bloom_h |> reserve(BLOOM_LEVELS)
    for (_i in range(BLOOM_LEVELS)) {
        lw = max(lw / 2, 1)
        lh = max(lh / 2, 1)
        let tex = make_texture(lw, lh, color_format, GL_RGBA, color_type, GL_LINEAR)
        bloom_tex |> push(tex)
        bloom_fbo |> push(make_color_fbo(tex))
        bloom_w |> push(lw)
        bloom_h |> push(lh)
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

// ============================================================================
// Pass driver
// ============================================================================

def fullscreen_pass() {
    glBindVertexArray(empty_vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)
}

def private bind_target(fbo : uint; w, h : int) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo)
    glViewport(0, 0, w, h)
}

def begin_shadow_pass() {
    bind_target(shadow_fbo, SHADOW_SIZE, SHADOW_SIZE)
    glClear(GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthMask(true)
    glDisable(GL_BLEND)
    // Front-face culling in the shadow pass pushes the depth samples to the far
    // side of each hull, which removes acne on the lit side for free.
    glEnable(GL_CULL_FACE)
    glCullFace(GL_FRONT)
}

def end_shadow_pass() {
    glCullFace(GL_BACK)
}

def begin_scene_pass(clear_color : float3) {
    bind_target(scene_fbo, fx_width, fx_height)
    var targets = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1)
    glDrawBuffers(2, unsafe(addr(targets[0])))
    glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthMask(true)
    glDisable(GL_BLEND)
}

// Alpha-blended geometry must not reach the normal/depth target -- blending
// into it would corrupt the AO and shaft reconstruction -- so the transparent
// span narrows the draw-buffer set to colour only.
def begin_transparent_span() {
    var targets = fixed_array(GL_COLOR_ATTACHMENT0)
    glDrawBuffers(1, unsafe(addr(targets[0])))
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    glDepthMask(false)
}

def end_transparent_span() {
    glDepthMask(true)
    glDisable(GL_BLEND)
    var targets = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1)
    glDrawBuffers(2, unsafe(addr(targets[0])))
}

struct PostSettings {
    exposure : float = 1.0
    bloom_threshold : float = 1.55
    bloom_knee : float = 0.35
    bloom_strength : float = 0.42
    ao_radius : float = 0.85
    ao_strength : float = 1.15
    ao_apply : float = 0.85
    shaft_uv : float2 = float2(0.5, 0.5)
    shaft_on_screen : float = 0.0
    shaft_density : float = 0.85
    shaft_decay : float = 0.965
    shaft_strength : float = 0.65
    contrast : float = 0.22
    grade_tint : float3 = float3(1.0)
    grade_amount : float = 0.0
    vignette : float = 1.35
    vignette_strength : float = 0.55
    grain : float = 0.035
    tan_half_fov : float2 = float2(0.5, 0.5)
    z_far : float = 500.0
    time : float = 0.0
}

def private run_ssao(cfg : PostSettings) {
    glUseProgram(ssao_program)
    bind_target(ao_fbo, max(fx_width / 2, 1), max(fx_height / 2, 1))
    p_tex0 := scene_nd
    p_params = float4(cfg.ao_radius, cfg.ao_strength, float(fx_width) * 0.37, 0.0)
    p_params2 = float4(cfg.tan_half_fov.x, cfg.tan_half_fov.y, cfg.z_far, 1.0)
    vs_post_bind_uniform(ssao_program)
    fs_ssao_bind_uniform(ssao_program)
    fullscreen_pass()

    glUseProgram(ao_blur_program)
    bind_target(ao_blur_fbo, max(fx_width / 2, 1), max(fx_height / 2, 1))
    p_tex0 := ao_tex
    p_texel = float2(2.0 / float(fx_width), 2.0 / float(fx_height))
    vs_post_bind_uniform(ao_blur_program)
    fs_ao_blur_bind_uniform(ao_blur_program)
    fullscreen_pass()
}

def private run_bloom(cfg : PostSettings) {
    glUseProgram(bright_program)
    bind_target(bloom_fbo[0], bloom_w[0], bloom_h[0])
    p_tex0 := scene_color
    p_params = float4(cfg.bloom_threshold, cfg.bloom_knee, 0.0, 0.0)
    vs_post_bind_uniform(bright_program)
    fs_bright_bind_uniform(bright_program)
    fullscreen_pass()

    glUseProgram(down_program)
    for (i in range(1, BLOOM_LEVELS)) {
        bind_target(bloom_fbo[i], bloom_w[i], bloom_h[i])
        p_tex0 := bloom_tex[i - 1]
        p_texel = float2(1.0 / float(bloom_w[i - 1]), 1.0 / float(bloom_h[i - 1]))
        vs_post_bind_uniform(down_program)
        fs_down_bind_uniform(down_program)
        fullscreen_pass()
    }

    // Additive tent upsample back down the chain; each level widens the halo.
    glUseProgram(up_program)
    glEnable(GL_BLEND)
    glBlendFunc(uint(GL_ONE), uint(GL_ONE))
    for (step in range(BLOOM_LEVELS - 1)) {
        let i = BLOOM_LEVELS - 1 - step
        bind_target(bloom_fbo[i - 1], bloom_w[i - 1], bloom_h[i - 1])
        p_tex0 := bloom_tex[i]
        p_texel = float2(1.0 / float(bloom_w[i]), 1.0 / float(bloom_h[i]))
        p_params = float4(1.35, 1.0, 0.0, 0.0)
        vs_post_bind_uniform(up_program)
        fs_up_bind_uniform(up_program)
        fullscreen_pass()
    }
    glDisable(GL_BLEND)
}

def private run_shafts(cfg : PostSettings) {
    let sw = max(fx_width / 4, 1)
    let sh = max(fx_height / 4, 1)

    glUseProgram(occlusion_program)
    bind_target(occl_fbo, sw, sh)
    p_tex0 := scene_color
    p_tex1 := scene_nd
    p_params = float4(cfg.shaft_uv.x, cfg.shaft_uv.y, 0.9, 0.0)
    vs_post_bind_uniform(occlusion_program)
    fs_occlusion_bind_uniform(occlusion_program)
    fullscreen_pass()

    glUseProgram(shafts_program)
    bind_target(shaft_fbo, sw, sh)
    p_tex0 := occl_tex
    p_params = float4(cfg.shaft_uv.x, cfg.shaft_uv.y, cfg.shaft_density, cfg.shaft_decay)
    vs_post_bind_uniform(shafts_program)
    fs_shafts_bind_uniform(shafts_program)
    fullscreen_pass()
}

// Fold the shafts into the bloom buffer so the composite stays a three-texture
// read; they are both additive glows and share the same upsample filtering.
def private add_shafts_to_bloom(cfg : PostSettings) {
    glUseProgram(up_program)
    glEnable(GL_BLEND)
    glBlendFunc(uint(GL_ONE), uint(GL_ONE))
    bind_target(bloom_fbo[0], bloom_w[0], bloom_h[0])
    p_tex0 := shaft_tex
    p_texel = float2(4.0 / float(fx_width), 4.0 / float(fx_height))
    p_params = float4(1.2, cfg.shaft_strength * 1.6, 0.0, 0.0)
    vs_post_bind_uniform(up_program)
    fs_up_bind_uniform(up_program)
    fullscreen_pass()
    glDisable(GL_BLEND)
}

def run_postfx(cfg : PostSettings) {
    if (scene_fbo == 0u) {
        return
    }
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glDepthMask(false)

    run_ssao(cfg)
    run_bloom(cfg)
    if (cfg.shaft_on_screen > 0.0) {
        run_shafts(cfg)
        add_shafts_to_bloom(cfg)
    }

    glUseProgram(composite_program)
    bind_target(ldr_fbo, fx_width, fx_height)
    p_tex0 := scene_color
    p_tex1 := ao_blur_tex
    p_tex2 := bloom_tex[0]
    p_params = float4(cfg.exposure, cfg.bloom_strength, cfg.ao_apply, cfg.contrast)
    p_params2 = float4(cfg.grade_amount, 0.0, 0.0, 0.0)
    p_tint = cfg.grade_tint
    vs_post_bind_uniform(composite_program)
    fs_composite_bind_uniform(composite_program)
    fullscreen_pass()

    glUseProgram(present_program)
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glViewport(0, 0, fx_width, fx_height)
    p_tex0 := ldr_tex
    p_texel = float2(1.0 / float(fx_width), 1.0 / float(fx_height))
    p_params = float4(cfg.vignette, cfg.vignette_strength, cfg.grain, cfg.time)
    p_params2 = float4(0.0, float(fx_height), 0.0, 0.0)
    vs_post_bind_uniform(present_program)
    fs_present_bind_uniform(present_program)
    fullscreen_pass()

    glDepthMask(true)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)
}
