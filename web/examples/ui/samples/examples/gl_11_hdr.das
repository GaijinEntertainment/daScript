options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 11 - HDR + Bloom (OpenGL / WebGL2 tutorial)
//
// The ladder's first floating-point render target. Everything so far drew into an
// 8-bit-per-channel framebuffer where colour is clamped to [0, 1] -- a value of 4.0
// and a value of 1.0 are stored identically (both "white"). This rung renders the
// scene into a 16-bit-float target (RGBA16F) where bright surfaces keep their true,
// above-one intensity, then builds a Karis-style bloom from those bright pixels and
// tonemaps the result back down to the screen. It is a faithful port of the dasVulkan
// HDR rung, expressed with WebGL2 framebuffers.
//
// The scene is 05's thousand-cube swarm, but ~1 cube in 9 is flagged "emissive" and
// multiplies its lit colour by EMISSIVE_BOOST (= 8x) -- well above 1.0. In an LDR
// target those cubes would just clip to white; in the HDR target they carry their
// real brightness into the post-processing chain.
//
// Five passes, each a fullscreen post step except the first:
//
//   1. SCENE     -> HDR target. Draw the instanced swarm into hdr_color (RGBA16F).
//   2. BRIGHT    -> bloom mip 0 (half res). Soft-knee threshold isolates the bright
//                   pixels; a Karis weight (1/(1+luma)) tames fireflies so the bloom
//                   pyramid stays in a filterable range.
//   3. DOWN x4   -> mips 1..4. A 5-tap "Karis-bilinear" filter halves the resolution
//                   each step, growing the blur radius cheaply.
//   4. UP x4     -> mips 3..0, additively blended. A 9-tap tent upsamples each mip
//                   onto the larger one, accumulating a smooth, wide glow.
//   5. COMPOSITE -> screen. Add bloom mip 0 onto the HDR scene, ACES-tonemap to bring
//                   the >1 range into [0, 1], and gamma-encode for the sRGB display.
//
// The float framebuffer is the headline; the bloom pyramid is what it enables. Every
// shader line is daslang, lowered to GLSL by dasGlsl. One homogeneous program: main()
// is the desktop windowed viewer; on the web the wasm run path drives init / update /
// shutdown once per browser frame.

// ===== scene shader globals =====
// per-vertex (binding 0): pos + normal
var @in @location = 0 a_pos : float3
var @in @location = 1 a_normal : float3
// per-instance (binding 1): offset + colour + phase + emissive flag (1 = glows)
var @in @location = 2 a_offset : float3
var @in @location = 3 a_color : float3
var @in @location = 4 a_phase : float
var @in @location = 5 a_emissive : float
var @uniform u_view : float4x4
var @uniform u_proj : float4x4
var @uniform u_cam_pos : float3
var @uniform u_time : float
var @inout w_pos : float3
var @inout w_normal : float3
var @inout w_color : float3
var @inout w_emissive : float
// shared fragment output, written by every pass
var @out f_FragColor : float4

[vertex_program]
def scene_vs {
    let t = u_time
    // per-instance breathing + gentle spin about Y, desynchronised by the phase
    let breathe = 0.7 + 0.2 * sin(t * 1.5 + a_phase * 6.2832)
    let local = a_pos * breathe
    let ang = t * 0.25 + a_phase * 6.2832
    let c = cos(ang)
    let s = sin(ang)
    let rotated = float3(c * local.x - s * local.z, local.y, s * local.x + c * local.z)
    let world = rotated + a_offset
    gl_Position = u_proj * u_view * float4(world, 1.0)
    w_pos = world
    w_normal = float3(c * a_normal.x - s * a_normal.z, a_normal.y, s * a_normal.x + c * a_normal.z)
    w_color = a_color
    w_emissive = a_emissive
}

let EMISSIVE_BOOST = 8.0          // glowing cubes hit ~8x -- well above the bright-pass knee
let LIT_AMBIENT = 0.25
let LIT_KEY = 0.7

[fragment_program]
def scene_fs {
    let n = normalize(w_normal)
    let v = normalize(u_cam_pos - w_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))
    let key = max(dot(n, l), 0.0) * LIT_KEY + LIT_AMBIENT
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.5)
    // lit_lo stays in [0, 1]; emissive cubes multiply by EMISSIVE_BOOST, so the float
    // target carries values up to ~8 that the bright pass will threshold.
    let lit_lo = w_color * key + float3(0.25, 0.35, 0.55) * rim * 0.4
    let lit = lit_lo * (1.0 + w_emissive * (EMISSIVE_BOOST - 1.0))
    f_FragColor = float4(lit, 1.0)
}

// ===== fullscreen post shader globals =====
// One screen-covering triangle (3 verts, no index buffer); v_uv runs [0, 1] over the
// visible square. All four post passes share this vertex shader.
var @in @location = 0 q_pos : float2
var @inout v_uv : float2
// src0 is the only sampler the bright / down / up passes read (texture unit 0). The
// composite pass adds src_bloom on unit 1 -- @stage = N drives the GL bind unit, so two
// samplers can be read at once.
var @uniform @stage = 0 src0 : sampler2D
var @uniform @stage = 1 src_bloom : sampler2D
var @uniform u_threshold : float            // bright pass: pixels below this luma are killed
var @uniform u_soft_knee : float            // bright pass: soft-knee width around the threshold
var @uniform u_src_rcp : float2             // down / up: 1 / source-mip size, the sample offset
var @uniform u_bloom_intensity : float      // composite: how loud bloom is over the scene

[vertex_program]
def post_vs {
    gl_Position = float4(q_pos, 0.0, 1.0)
    v_uv = q_pos * 0.5 + float2(0.5, 0.5)
}

// Bright pass: Frostbite soft-knee threshold gives a Hermite blend from 0 (luma well
// below threshold) to 1 (above threshold + knee) instead of a hard step, so moving
// highlights do not alias. The Karis weight 1/(1+luma) crushes an 8x emissive value
// down near 1.0 so the bloom pyramid stays in a range bilinear filtering can smooth.
[fragment_program]
def bright_fs {
    let hdr = texture(src0, v_uv).xyz
    let luma = dot(hdr, float3(0.2126, 0.7152, 0.0722))   // BT.709 luminance
    let knee = max(u_soft_knee, 0.0001)
    let tt = clamp((luma - u_threshold + knee) / (2.0 * knee), 0.0, 1.0)
    let curve = tt * tt * (3.0 - 2.0 * tt)
    let karis_weight = 1.0 / (1.0 + luma)
    f_FragColor = float4(hdr * karis_weight * curve, 1.0)
}

// Down pass: 5-tap Karis-bilinear. The centre weighs 0.5, four diagonal taps at +-1
// source-texel weigh 0.125 each; bilinear filtering makes each diagonal act as a 2x2
// box, so 5 GPU samples cover a 16-texel footprint of the input mip.
[fragment_program]
def down_fs {
    let o = u_src_rcp
    let c = texture(src0, v_uv).xyz
    let lt = texture(src0, v_uv + float2(-o.x, -o.y)).xyz
    let rt = texture(src0, v_uv + float2(o.x, -o.y)).xyz
    let lb = texture(src0, v_uv + float2(-o.x, o.y)).xyz
    let rb = texture(src0, v_uv + float2(o.x, o.y)).xyz
    let sum = c * 0.5 + (lt + rt + lb + rb) * 0.125
    f_FragColor = float4(sum, 1.0)
}

// Up pass: 9-tap tent (1-2-1 / 2-4-2 / 1-2-1, sum 16) reads the smaller mip and the
// host additively blends it onto the larger one, so the glow widens as it climbs back
// up the pyramid.
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
    f_FragColor = float4(tent, 1.0)
}

// ACES filmic fit (Narkowicz 2015): one rational polynomial that mimics the ACES
// shoulder/toe, mapping the unbounded HDR range into [0, 1]. daslang shaders need
// explicit float3 broadcasts -- there is no implicit scalar-to-vector across +/-.
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
    // The Vulkan rung wrote linear and let an sRGB swap target encode gamma in hardware.
    // WebGL2's default framebuffer does no sRGB encode, so gamma-encode here -- this is
    // the one real GL-vs-Vulkan delta for this rung.
    let g = 1.0 / 2.2
    let encoded = pow(mapped, float3(g, g, g))
    f_FragColor = float4(encoded, 1.0)
}

// ===== GL objects =====

var prog_scene : uint
var prog_bright : uint
var prog_down : uint
var prog_up : uint
var prog_composite : uint
var scene_vao, scene_vbo, inst_vbo, scene_ebo : uint
var quad_vao, quad_vbo : uint
var hdr_fbo, hdr_color, hdr_depth : uint
var window : GLFWwindow?
var time : float = 0.0

// Fixed internal HDR resolution (matches the 640x480 canvas, so the composite maps 1:1).
let HDR_W = 640
let HDR_H = 480
let N_INSTANCES = 1000
let BLOOM_MIPS = 5

// bloom mip chain: mip i lives at (HDR >> (i+1)) -- mip 0 is half res, then halving.
var bloom_tex : uint[BLOOM_MIPS]
var bloom_fbo : uint[BLOOM_MIPS]
var bloom_w : int[BLOOM_MIPS]
var bloom_h : int[BLOOM_MIPS]

[vertex_buffer]
struct Vertex {
    xyz : float3
    normal : float3
}

[vertex_buffer]
struct Instance {
    offset : float3
    color : float3
    phase : float
    emissive : float
}

[vertex_buffer]
struct QuadVertex {
    xy : float2
}

// faceted unit cube (half-extent 0.5), per-face normals, 24 verts / 36 indices --
// same winding as tutorials 04 / 05, correct under GL_CULL_FACE GL_BACK.
let vertices = [Vertex(
    xyz=float3(0.5, 0.5, 0.5), normal=float3(0, 0, 1)), Vertex(
    xyz=float3(-0.5, 0.5, 0.5), normal=float3(0, 0, 1)), Vertex(
    xyz=float3(-0.5, -0.5, 0.5), normal=float3(0, 0, 1)), Vertex(
    xyz=float3(0.5, -0.5, 0.5), normal=float3(0, 0, 1)), Vertex(
    xyz=float3(0.5, 0.5, 0.5), normal=float3(1, 0, 0)), Vertex(
    xyz=float3(0.5, -0.5, 0.5), normal=float3(1, 0, 0)), Vertex(
    xyz=float3(0.5, -0.5, -0.5), normal=float3(1, 0, 0)), Vertex(
    xyz=float3(0.5, 0.5, -0.5), normal=float3(1, 0, 0)), Vertex(
    xyz=float3(0.5, 0.5, 0.5), normal=float3(0, 1, 0)), Vertex(
    xyz=float3(0.5, 0.5, -0.5), normal=float3(0, 1, 0)), Vertex(
    xyz=float3(-0.5, 0.5, -0.5), normal=float3(0, 1, 0)), Vertex(
    xyz=float3(-0.5, 0.5, 0.5), normal=float3(0, 1, 0)), Vertex(
    xyz=float3(-0.5, 0.5, 0.5), normal=float3(-1, 0, 0)), Vertex(
    xyz=float3(-0.5, 0.5, -0.5), normal=float3(-1, 0, 0)), Vertex(
    xyz=float3(-0.5, -0.5, -0.5), normal=float3(-1, 0, 0)), Vertex(
    xyz=float3(-0.5, -0.5, 0.5), normal=float3(-1, 0, 0)), Vertex(
    xyz=float3(-0.5, -0.5, -0.5), normal=float3(0, -1, 0)), Vertex(
    xyz=float3(0.5, -0.5, -0.5), normal=float3(0, -1, 0)), Vertex(
    xyz=float3(0.5, -0.5, 0.5), normal=float3(0, -1, 0)), Vertex(
    xyz=float3(-0.5, -0.5, 0.5), normal=float3(0, -1, 0)), Vertex(
    xyz=float3(0.5, -0.5, -0.5), normal=float3(0, 0, -1)), Vertex(
    xyz=float3(-0.5, -0.5, -0.5), normal=float3(0, 0, -1)), Vertex(
    xyz=float3(-0.5, 0.5, -0.5), normal=float3(0, 0, -1)), Vertex(
    xyz=float3(0.5, 0.5, -0.5), normal=float3(0, 0, -1)
)];

let indices = fixed_array<int>(
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20)

// fullscreen triangle covering NDC [-1, 1]^2; v_uv is derived from the position.
let quad = [QuadVertex(xy = float2(-1, -1)), QuadVertex(xy = float2(3, -1)), QuadVertex(xy = float2(-1, 3))];

// Build N_INSTANCES rows on a 3D Lissajous curve (7 azimuthal vs 3 elevation lobes vs
// radial breathing) so the swarm fills a balanced volume; hue cycles three times; phase
// sweeps [0, 1) to desynchronise the per-cube animation. Every 9th cube is emissive.
def gen_instances(n : int) : array<Instance> {
    var result : array<Instance>
    result |> resize(n)
    for (i in range(n)) {
        let fi = float(i) / float(n)
        let theta = fi * 2.0 * PI * 7.0
        let phi = fi * 2.0 * PI * 3.0
        let r = 5.0 + 1.5 * sin(fi * 2.0 * PI * 5.0)
        let x = r * cos(theta) * cos(phi * 0.5)
        let y = r * sin(phi)
        let z = r * sin(theta) * cos(phi * 0.5)
        let hue = fi * 6.0 * PI
        let cr = 0.5 + 0.5 * cos(hue)
        let cg = 0.5 + 0.5 * cos(hue + 2.0944)
        let cb = 0.5 + 0.5 * cos(hue + 4.1888)
        let emissive = (i % 9 == 0) ? 1.0 : 0.0
        result[i] = Instance(offset = float3(x, y, z), color = float3(cr, cg, cb), phase = fi, emissive = emissive)
    }
    return <- result
}

// One RGBA16F colour texture wired to a fresh framebuffer; returns (texture, fbo).
// Used for the scene target (with a depth attachment added separately) and every bloom
// mip. LINEAR + CLAMP_TO_EDGE so the down/up taps filter and never wrap across edges.
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

def create_scene_target {
    let cf = make_hdr_color(HDR_W, HDR_H)
    hdr_color = cf._0
    hdr_fbo = cf._1
    // depth needs a renderbuffer; the scene FBO is bound from make_hdr_color
    glGenRenderbuffers(1, safe_addr(hdr_depth))
    glBindRenderbuffer(GL_RENDERBUFFER, hdr_depth)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, HDR_W, HDR_H)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdr_depth)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("HDR scene framebuffer incomplete")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def create_bloom_chain {
    for (i in range(BLOOM_MIPS)) {
        bloom_w[i] = max(HDR_W >> (i + 1), 1)
        bloom_h[i] = max(HDR_H >> (i + 1), 1)
        let cf = make_hdr_color(bloom_w[i], bloom_h[i])
        bloom_tex[i] = cf._0
        bloom_fbo[i] = cf._1
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            panic("bloom mip framebuffer incomplete")
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def create_gl_objects {
    prog_scene = create_shader_program(@@scene_vs, @@scene_fs)
    prog_bright = create_shader_program(@@post_vs, @@bright_fs)
    prog_down = create_shader_program(@@post_vs, @@down_fs)
    prog_up = create_shader_program(@@post_vs, @@up_fs)
    prog_composite = create_shader_program(@@post_vs, @@composite_fs)

    // scene mesh: per-vertex geometry (binding 0) + per-instance stream (binding 1)
    glGenVertexArrays(1, safe_addr(scene_vao))
    glBindVertexArray(scene_vao)
    glGenBuffers(1, safe_addr(scene_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, scene_vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Vertex>)
    var instances <- gen_instances(N_INSTANCES)
    glGenBuffers(1, safe_addr(inst_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, inst_vbo)
    glBufferData(GL_ARRAY_BUFFER, instances, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Instance>, 2u)
    glVertexAttribDivisor(2u, 1u)
    glVertexAttribDivisor(3u, 1u)
    glVertexAttribDivisor(4u, 1u)
    glVertexAttribDivisor(5u, 1u)
    delete instances
    glGenBuffers(1, safe_addr(scene_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)

    // fullscreen triangle for the post passes
    glGenVertexArrays(1, safe_addr(quad_vao))
    glBindVertexArray(quad_vao)
    glGenBuffers(1, safe_addr(quad_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo)
    glBufferData(GL_ARRAY_BUFFER, quad, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<QuadVertex>)

    create_scene_target()
    create_bloom_chain()
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(HDR_W, HDR_H, "OpenGL - 11 HDR + bloom", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    create_gl_objects()
}

def draw_fullscreen {
    glBindVertexArray(quad_vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)
}

[export]
def update : bool {
    time += 1.0 / 60.0
    let t = time
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)

    // camera orbits the swarm with a gentle vertical sway; scene projection uses the
    // fixed HDR aspect since the scene renders into the HDR_W x HDR_H target.
    let camera_angle = t * 0.3
    let camera_r = 14.0
    let cam_pos = float3(camera_r * cos(camera_angle), sin(camera_angle * 2.0) * 2.0, camera_r * sin(camera_angle))
    u_view = look_at_rh(cam_pos, float3(0, 0, 0), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(60.0 * PI / 180.0, float(HDR_W) / float(HDR_H), 0.1, 60.0)
    u_cam_pos = cam_pos
    u_time = t

    // ===== Pass 1: scene into the HDR float target =====
    glBindFramebuffer(GL_FRAMEBUFFER, hdr_fbo)
    glViewport(0, 0, HDR_W, HDR_H)
    glClearColor(0.01, 0.0, 0.03, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    glDisable(GL_BLEND)
    glUseProgram(prog_scene)
    scene_vs_bind_uniform(prog_scene)
    scene_fs_bind_uniform(prog_scene)
    glBindVertexArray(scene_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene_ebo)
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, null, N_INSTANCES)

    // the post passes are 2D, no depth or culling
    glDisable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)

    // ===== Pass 2: bright-pass threshold into bloom mip 0 =====
    glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[0])
    glViewport(0, 0, bloom_w[0], bloom_h[0])
    glUseProgram(prog_bright)
    src0 := hdr_color
    u_threshold = 1.0
    u_soft_knee = 0.6
    post_vs_bind_uniform(prog_bright)
    bright_fs_bind_uniform(prog_bright)
    draw_fullscreen()

    // ===== Pass 3: downsample mip 0 -> 1 -> 2 -> 3 -> 4 =====
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

    // ===== Pass 4: upsample mip 4 -> 3 -> 2 -> 1 -> 0, additively blended =====
    glEnable(GL_BLEND)
    glBlendFunc(uint(GL_ONE), uint(GL_ONE))
    glUseProgram(prog_up)
    for (k in range(BLOOM_MIPS - 1)) {
        let i = BLOOM_MIPS - 1 - k        // i = 4, 3, 2, 1; writes onto mip i-1
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_fbo[i - 1])
        glViewport(0, 0, bloom_w[i - 1], bloom_h[i - 1])
        src0 := bloom_tex[i]
        u_src_rcp = float2(1.0 / float(bloom_w[i]), 1.0 / float(bloom_h[i]))
        post_vs_bind_uniform(prog_up)
        up_fs_bind_uniform(prog_up)
        draw_fullscreen()
    }
    glDisable(GL_BLEND)

    // ===== Pass 5: composite HDR scene + bloom -> ACES tonemap -> gamma -> screen =====
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
    glfwDestroyWindow(window)
    glfwTerminate()
}

// Desktop driver. On the web this is never called -- the run path drives the
// three lifecycle functions directly and persists the Context across frames.
[export]
def main {
    init()
    while (update()) {
    }
    shutdown()
}
