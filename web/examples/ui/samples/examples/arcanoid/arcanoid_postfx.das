options gen2
options persistent_heap
options indenting = 4

module arcanoid_postfx public

require opengl/opengl_boost public
require opengl/opengl_cache public
require live_host public
require live/live_vars public
require daslib/math_boost public
require daslib/safe_addr public

// The offscreen scene target and the screen-space chain that finishes a frame: ambient
// occlusion on the board, bloom, a composite, and a CRT present pass - curvature, scanlines,
// an aperture-grille mask, chromatic fringing, vignette and flicker.

let BLOOM_LEVELS = 4

var @live fx_width = 0
var @live fx_height = 0
var @live fx_hdr = false

var @live scene_fbo = 0u
var @live scene_color = 0u
var @live scene_nd = 0u
var @live scene_depth_rb = 0u

var @live ldr_fbo = 0u
var @live ldr_tex = 0u

var @live ao_fbo = 0u
var @live ao_tex = 0u
var @live ao_blur_fbo = 0u
var @live ao_blur_tex = 0u

var @live bloom_fbo : array<uint>
var @live bloom_tex : array<uint>
var @live bloom_w : array<int>
var @live bloom_h : array<int>

var @live empty_vao = 0u

// --- Post-process shader interface

var @inout p_uv : float2

var @uniform @stage = 0 p_tex0 : sampler2D
var @uniform @stage = 1 p_tex1 : sampler2D
var @uniform @stage = 2 p_tex2 : sampler2D
var @uniform p_texel : float2
var @uniform p_params : float4
var @uniform p_params2 : float4
var @uniform p_params3 : float4

var @out p_FragColor : float4

[vertex_program]
def vs_post {
    let id = gl_VertexIndex
    p_uv = float2(float((id << 1) & 2), float(id & 2))
    gl_Position = float4(p_uv * 2.0 - float2(1.0), 0.0, 1.0)
}

// --- Bright pass with a soft knee

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

// --- 13-tap dual-filter downsample

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

// --- Tent upsample, additively blended onto the next larger level; p_params.x is the
// radius, p_params.y the intensity

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

// --- Ambient occlusion: hemisphere samples in view space, reconstructed from the linear
// depth in the normal target's alpha, at half resolution; the blur hides the sample noise

def private view_from_uv(uv : float2; depth : float) : float3 {
    let ndc = uv * 2.0 - float2(1.0)
    return float3(ndc.x * p_params2.x * depth, ndc.y * p_params2.y * depth, -depth)
}

def private hash_noise(p : float2) : float {
    return fract_local(sin(dot(floor(p), float2(12.9898, 78.233))) * 43758.545)
}

def private fract_local(x : float) : float {
    return x - floor(x)
}

//! the scene writes (normal.xy, depth high byte, depth low byte); the background stays at depth 1
def private nd_depth(nd : float4) : float {
    return nd.z + nd.w / 255.0
}

[fragment_program]
def fs_ssao {
    let nd = texture(p_tex0, p_uv)
    let depth01 = nd_depth(nd)
    let depth = depth01 * p_params2.z
    if (depth01 >= 0.999) {
        p_FragColor = float4(1.0)
        return
    }
    let origin = view_from_uv(p_uv, depth)
    let nxy = nd.xy * 2.0 - float2(1.0)
    let nrm = normalize(float3(nxy, sqrt(saturate(1.0 - dot(nxy, nxy)))))
    let radius = p_params.x
    let rot = hash_noise(p_uv * p_params.z) * 6.2831853
    var occlusion = 0.0
    let taps = 12
    for (i in range(taps)) {
        let fi = float(i)
        let ang = rot + fi * 2.399963
        let rad = radius * sqrt((fi + 0.5) / float(taps))
        let dir = float3(cos(ang) * rad, sin(ang) * rad, 0.0)
        var sample_pos = origin + dir
        sample_pos += nrm * rad * 0.65
        let su = float2(sample_pos.x / (p_params2.x * -sample_pos.z), sample_pos.y / (p_params2.y * -sample_pos.z))
        let suv = su * 0.5 + float2(0.5)
        if (suv.x < 0.0 || suv.x > 1.0 || suv.y < 0.0 || suv.y > 1.0) {
            continue
        }
        let sample_depth = nd_depth(texture(p_tex0, suv)) * p_params2.z
        let delta = -sample_pos.z - sample_depth
        let range_check = saturate(radius / max(abs(-origin.z - sample_depth), 0.0001))
        if (delta > 0.02 * p_params2.w) {
            occlusion += range_check
        }
    }
    let ao = saturate(1.0 - occlusion / float(taps) * p_params.y)
    p_FragColor = float4(ao, ao, ao, 1.0)
}

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

// --- Composite: occlusion, bloom, exposure

[fragment_program]
def fs_composite {
    var color = texture(p_tex0, p_uv).xyz
    let ao = texture(p_tex1, p_uv).x
    let bloom = texture(p_tex2, p_uv).xyz
    color *= lerp(1.0, ao, p_params.z)
    color += bloom * p_params.y
    color *= p_params.x
    p_FragColor = float4(saturate(color), 1.0)
}

// --- Present, plain: the composite straight to the back buffer

[fragment_program]
def fs_present_plain {
    p_FragColor = float4(texture(p_tex0, p_uv).xyz, 1.0)
}

// --- Present, CRT
//
// p_params  = (curvature, scanline strength, mask strength, fringe)
// p_params2 = (vignette, flicker, time, brightness)
// p_params3 = (screen width, screen height, scanline count, corner rounding)

def private crt_warp(uv : float2; curvature : float) : float2 {
    let c = uv * 2.0 - float2(1.0)
    let r2 = dot(c, c)
    let warped = c * (1.0 + curvature * r2)
    return warped * 0.5 + float2(0.5)
}

[fragment_program]
def fs_present_crt {
    let curvature = p_params.x
    let uv = crt_warp(p_uv, curvature)
    // outside the tube face: black, with a rounded corner
    let edge = float2(abs(uv.x - 0.5), abs(uv.y - 0.5)) - float2(0.5 - p_params3.w)
    let corner = length(max(edge, float2(0.0))) - p_params3.w
    if (corner > 0.0) {
        p_FragColor = float4(0.0, 0.0, 0.0, 1.0)
        return
    }
    // chromatic fringing grows toward the edge
    let radial = uv - float2(0.5)
    let fringe = p_params.w * dot(radial, radial)
    let r = texture(p_tex0, uv + radial * fringe).x
    let g = texture(p_tex0, uv).y
    let b = texture(p_tex0, uv - radial * fringe).z
    var color = float3(r, g, b)
    // scanlines follow the tube face (warped uv) at a count well under the pixel rows, so the
    // beam profile is resolved rather than aliased into rings
    let line = uv.y * p_params3.z
    let scan = 0.5 + 0.5 * cos(line * 6.2831853)
    color *= 1.0 - p_params.y * scan * scan
    // aperture grille: three phosphor columns per triad, in screen pixels so it never moires
    let px = p_uv.x * p_params3.x
    let cell = floor(fract_local(px / 3.0) * 3.0)
    var mask = float3(1.0)
    if (cell < 0.5) {
        mask = float3(1.0, 0.55, 0.55)
    } elif (cell < 1.5) {
        mask = float3(0.55, 1.0, 0.55)
    } else {
        mask = float3(0.55, 0.55, 1.0)
    }
    color *= lerp(float3(1.0), mask, p_params.z)
    // the mask and the lines eat light; give it back, then vignette and a mains-frequency flicker
    color *= p_params2.w
    let vig = 1.0 - saturate(dot(radial, radial) * p_params2.x)
    color *= vig * vig
    color *= 1.0 + p_params2.y * sin(p_params2.z * 120.0 * 6.2831853)
    // a soft edge on the tube face instead of a hard cut
    let rim = saturate(-corner * 60.0)
    p_FragColor = float4(color * rim, 1.0)
}

var bright_program : uint
var down_program : uint
var up_program : uint
var ssao_program : uint
var ao_blur_program : uint
var composite_program : uint
var present_plain_program : uint
var present_crt_program : uint

def create_postfx_programs() {
    bright_program = cache_shader_program(vs_post`shader_text, fs_bright`shader_text)
    down_program = cache_shader_program(vs_post`shader_text, fs_down`shader_text)
    up_program = cache_shader_program(vs_post`shader_text, fs_up`shader_text)
    ssao_program = cache_shader_program(vs_post`shader_text, fs_ssao`shader_text)
    ao_blur_program = cache_shader_program(vs_post`shader_text, fs_ao_blur`shader_text)
    composite_program = cache_shader_program(vs_post`shader_text, fs_composite`shader_text)
    present_plain_program = cache_shader_program(vs_post`shader_text, fs_present_plain`shader_text)
    present_crt_program = cache_shader_program(vs_post`shader_text, fs_present_crt`shader_text)
}

def private make_texture(w, h : int; internal_format : uint; format, data_type, filter : uint) : uint {
    var tex = 0u
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_2D, tex)
    glTexImage2D(GL_TEXTURE_2D, 0, int(internal_format), w, h, 0, format, data_type, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    return tex
}

def private drop_texture(var tex : uint&) {
    if (tex != 0u) {
        glDeleteTextures(1, unsafe(addr(tex)))
        tex = 0u
    }
}

def private drop_fbo(var fbo : uint&) {
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

def ensure_render_targets(w, h : int) {
    if (w <= 0 || h <= 0) {
        return
    }
    if (empty_vao == 0u) {
        glGenVertexArrays(1, safe_addr(empty_vao))
    }
    if (w == fx_width && h == fx_height && scene_fbo != 0u) {
        return
    }
    destroy_render_targets()
    fx_width = w
    fx_height = h
    fx_hdr = try_build_scene(w, h, GL_RGBA16F, GL_HALF_FLOAT)
    if (!fx_hdr) {
        to_log(LOG_WARNING, "arcanoid: no float colour attachment, post chain runs at 8 bits\n")
        if (!try_build_scene(w, h, GL_RGBA8, GL_UNSIGNED_BYTE)) {
            to_log(LOG_ERROR, "arcanoid: scene framebuffer is incomplete\n")
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

def fullscreen_pass() {
    glBindVertexArray(empty_vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)
}

def private bind_target(fbo : uint; w, h : int) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo)
    glViewport(0, 0, w, h)
}

def begin_scene_pass(clear_color : float3) {
    bind_target(scene_fbo, fx_width, fx_height)
    var targets = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1)
    glDrawBuffers(2, unsafe(addr(targets[0])))
    glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0)
    glClearDepthf(1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    // the normal-depth target clears to a camera-facing normal at depth 1: background, to the occlusion pass
    var nd_clear = fixed_array(0.5, 0.5, 1.0, 0.0)
    glClearBufferfv(GL_COLOR, 1, unsafe(addr(nd_clear[0])))
    glEnable(GL_DEPTH_TEST)
    glDepthMask(true)
    glDisable(GL_BLEND)
}

//! blended geometry and the HUD draw to the colour target alone: a blend into the
//! normal-depth target would corrupt the occlusion it feeds
def begin_color_only_span() {
    var targets = fixed_array(GL_COLOR_ATTACHMENT0)
    glDrawBuffers(1, unsafe(addr(targets[0])))
}

//! a helper that ends in bind_ffp (the ttf text draw) unbinds the framebuffer; this puts the
//! scene target back, colour only, so the next 2D draw lands under the post chain too
def rebind_scene_target() {
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo)
    begin_color_only_span()
}

def end_color_only_span() {
    var targets = fixed_array(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1)
    glDrawBuffers(2, unsafe(addr(targets[0])))
}

struct PostSettings {
    ssao : bool = true
    crt : bool = true
    exposure : float = 1.0
    bloom_threshold : float = 0.6
    bloom_knee : float = 0.3
    bloom_strength : float = 0.35
    ao_radius : float = 0.6
    ao_strength : float = 1.2
    ao_apply : float = 0.8
    tan_half_fov : float2 = float2(0.5, 0.5)
    z_far : float = 100.0
    curvature : float = 0.08
    scanline : float = 0.28
    mask : float = 0.22
    fringe : float = 0.006
    vignette : float = 0.6
    flicker : float = 0.012
    brightness : float = 1.55
    scanline_count : float = 480.0
    corner : float = 0.04
    time : float = 0.0
    debug_view : int = 0            // 1: the occlusion buffer, 2: the normal-depth target
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

//! the scene is drawn: run the chain and present to the back buffer
def run_postfx(cfg : PostSettings) {
    if (scene_fbo == 0u) {
        return
    }
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
    glDisable(GL_BLEND)
    glDepthMask(false)
    if (cfg.ssao) {
        run_ssao(cfg)
    }
    run_bloom(cfg)

    glUseProgram(composite_program)
    bind_target(ldr_fbo, fx_width, fx_height)
    p_tex0 := scene_color
    p_tex1 := ao_blur_tex
    p_tex2 := bloom_tex[0]
    p_params = float4(cfg.exposure, cfg.bloom_strength, cfg.ssao ? cfg.ao_apply : 0.0, 0.0)
    vs_post_bind_uniform(composite_program)
    fs_composite_bind_uniform(composite_program)
    fullscreen_pass()

    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glViewport(0, 0, fx_width, fx_height)
    p_tex0 := ldr_tex
    if (cfg.debug_view == 1) {
        p_tex0 := ao_blur_tex
    } elif (cfg.debug_view == 2) {
        p_tex0 := scene_nd
    }
    if (cfg.crt && cfg.debug_view == 0) {
        glUseProgram(present_crt_program)
        let lines = cfg.scanline_count
        p_params = float4(cfg.curvature, cfg.scanline, cfg.mask, cfg.fringe)
        p_params2 = float4(cfg.vignette, cfg.flicker, cfg.time, cfg.brightness)
        p_params3 = float4(float(fx_width), float(fx_height), lines, cfg.corner)
        vs_post_bind_uniform(present_crt_program)
        fs_present_crt_bind_uniform(present_crt_program)
    } else {
        glUseProgram(present_plain_program)
        vs_post_bind_uniform(present_plain_program)
        fs_present_plain_bind_uniform(present_plain_program)
    }
    fullscreen_pass()

    glDepthMask(true)
    glEnable(GL_DEPTH_TEST)
    glEnable(GL_CULL_FACE)
}
