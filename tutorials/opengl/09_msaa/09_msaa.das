options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 09 - Multisample Anti-Aliasing (OpenGL / WebGL2 tutorial)
//
// The second GL-idiom-substitution rung. dasVulkan's MSAA tutorial draws into a 4x
// multi-sample colour attachment and names a 1x "resolve attachment" in the render
// pass; the GPU averages the samples down at the end. WebGL2 has no resolve
// attachments, so this rung uses the canonical GL substitute: render into a
// multisample framebuffer, then RESOLVE WITH A BLIT (`glBlitFramebuffer` from a
// multisample read framebuffer averages the N samples per pixel into one).
//
// MSAA's edge smoothing is nearly invisible on a cube (12 long edges) and dramatic on
// a dense thin silhouette -- so the test mesh is a SPIKY BALL: a smooth UV-sphere core
// studded with 150 flat-shaded pyramidal spikes in Fibonacci-distributed directions, a
// sea-urchin whose every spike is a high-frequency silhouette edge. To make "is MSAA
// on?" unmistakable, the SAME ball is rendered twice -- once 1x, once 4x MSAA -- and
// the screen shows the LEFT HALF of the 1x image beside the RIGHT HALF of the resolved
// 4x image. The spikes crossing the centre line go from stair-stepped (left) to silky
// (right). Watch the silhouette crawl with jaggies on the left as the ball spins.
//
// The shader is byte-for-byte tutorial 04's: MSAA is a framebuffer concern, invisible
// to the vertex and fragment programs.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web the
// wasm run path drives init/update/shutdown once per browser frame.

// ===== shaders (identical to tutorial 04 -- MSAA is invisible here) =====
var @in @location a_pos : float3
var @in @location a_normal : float3
var @in @location a_uv : float2
var @uniform u_model : float4x4
var @uniform u_view : float4x4
var @uniform u_proj : float4x4
var @uniform u_cam_pos : float3
var @uniform u_time : float
var @uniform u_tex : sampler2D
var @inout w_pos : float3
var @inout w_normal : float3
var @inout w_uv : float2
var @out f_FragColor : float4

[vertex_program]
def vs_main {
    let world = u_model * float4(a_pos, 1.0)
    gl_Position = u_proj * u_view * world
    w_pos = world.xyz
    w_normal = (u_model * float4(a_normal, 0.0)).xyz
    w_uv = a_uv
}

[fragment_program]
def fs_main {
    let n = normalize(w_normal)
    let v = normalize(u_cam_pos - w_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))            // warm key light from above-front
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3            // ambient floor 0.3
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.0)        // silhouette glow -- pops the spike edges
    let rim_color = float3(0.4, 0.6, 1.0)                // cool cyan
    let scrolled = float2(w_uv.x, w_uv.y + u_time * 0.05)
    let albedo = texture(u_tex, scrolled).xyz
    let lit = albedo * key + rim_color * rim
    f_FragColor = float4(lit, 1.0)
}

// ===== GL objects =====

var program : uint
var vao : uint
var vbo : uint
var ebo : uint
var texture : uint
var index_count = 0
// two full-size render targets: a 1x path and an N-sample MSAA path
var fbo_1x, color_1x, depth_1x : uint
var fbo_msaa, color_msaa, depth_msaa : uint
var target_w = 0
var target_h = 0
var samples = 4
var window : GLFWwindow?
var time : float = 0.0

let FBW = 640
let FBH = 480

[vertex_buffer]
struct Vertex {
    xyz : float3
    normal : float3
    uv : float2
}

let SPIKE_COUNT = 150
let CORE_R = 0.5
let SPIKE_TIP = 0.95
let SPIKE_BASE_AT = 0.46
let SPIKE_BASE_R = 0.05
let SPHERE_STACKS = 18
let SPHERE_SECTORS = 26

// Build the spiky ball: a smooth indexed UV-sphere core (per-vertex normal = direction)
// plus 150 flat-shaded square pyramids in Fibonacci directions. The thin base + far tip
// make every spike a sharp silhouette edge -- the high-frequency content MSAA exists to
// tame.
def build_spiky_geometry(var verts : array<Vertex>; var indices : array<int>) {
    verts |> reserve(2400)
    indices |> reserve(4800)
    var nv = 0
    let sector_step = 2.0 * PI / float(SPHERE_SECTORS)
    let stack_step = PI / float(SPHERE_STACKS)
    for (i in range(SPHERE_STACKS + 1)) {
        let stack_a = PI * 0.5 - float(i) * stack_step
        let xy = cos(stack_a)
        let zz = sin(stack_a)
        for (j in range(SPHERE_SECTORS + 1)) {
            let sector_a = float(j) * sector_step
            let dir = float3(xy * cos(sector_a), zz, xy * sin(sector_a))
            let uv = float2(float(j) / float(SPHERE_SECTORS), float(i) / float(SPHERE_STACKS))
            verts |> push(Vertex(xyz = dir * CORE_R, normal = dir, uv = uv))
            nv ++
        }
    }
    for (i in range(SPHERE_STACKS)) {
        var k1 = i * (SPHERE_SECTORS + 1)
        var k2 = k1 + SPHERE_SECTORS + 1
        for (_j in range(SPHERE_SECTORS)) {
            if (i != 0) {
                indices |> push(k1)
                indices |> push(k2)
                indices |> push(k1 + 1)
            }
            if (i != SPHERE_STACKS - 1) {
                indices |> push(k1 + 1)
                indices |> push(k2)
                indices |> push(k2 + 1)
            }
            k1 ++
            k2 ++
        }
    }
    let golden = PI * (3.0 - sqrt(5.0))
    for (s in range(SPIKE_COUNT)) {
        let yy = 1.0 - (float(s) + 0.5) / float(SPIKE_COUNT) * 2.0
        let rr = sqrt(max(1.0 - yy * yy, 0.0))
        let theta = float(s) * golden
        let dir = float3(rr * cos(theta), yy, rr * sin(theta))
        let up = abs(dir.y) > 0.95 ? float3(1.0, 0.0, 0.0) : float3(0.0, 1.0, 0.0)
        let tan0 = normalize(cross(up, dir))
        let bit0 = cross(dir, tan0)
        let tip = dir * SPIKE_TIP
        var ring : float3[4]
        for (k in range(4)) {
            let a = float(k) * (PI * 0.5)
            ring[k] = dir * SPIKE_BASE_AT + (tan0 * cos(a) + bit0 * sin(a)) * SPIKE_BASE_R
        }
        for (k in range(4)) {
            var v0 = ring[k]
            var v1 = ring[(k + 1) % 4]
            var fn = normalize(cross(v1 - v0, tip - v0))
            // force the normal + winding outward so back-face culling keeps a clean silhouette
            if (dot(fn, dir) < 0.0) {
                let tmp = v0
                v0 = v1
                v1 = tmp
                fn = float3(-fn.x, -fn.y, -fn.z)
            }
            verts |> push(Vertex(xyz = v0, normal = fn, uv = float2(0.5, 0.92)))
            nv ++
            verts |> push(Vertex(xyz = v1, normal = fn, uv = float2(0.5, 0.92)))
            nv ++
            verts |> push(Vertex(xyz = tip, normal = fn, uv = float2(0.5, 0.06)))
            nv ++
            indices |> push(nv - 3)
            indices |> push(nv - 2)
            indices |> push(nv - 1)
        }
    }
}

let TEX_DIM = 256

// 256x256 RGBA8 synthwave horizon, generated CPU-side -- the same texture the cube
// tutorials wear, here wrapping the spiky ball.
def gen_synthwave_texture(w, h : int) : array<uint8> {
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    let horizon = h / 2 - 8
    let sun_cx = w / 2
    let sun_cy = horizon - 26
    let sun_r = 42
    for (y in range(h)) {
        for (x in range(w)) {
            var r = 0
            var g = 0
            var b = 0
            if (y < horizon) {
                let t = float(y) / float(horizon)
                r = int(40.0 + t * 215.0)
                g = int(6.0 + t * 64.0)
                b = int(80.0 + t * 175.0)
                let dx = x - sun_cx
                let dy = y - sun_cy
                if (dx * dx + dy * dy < sun_r * sun_r) {
                    let band = (y / 4) % 2
                    if (band == 0) {
                        let glow = clamp(1.0 - float(dy + sun_r) / float(2 * sun_r), 0.0, 1.0)
                        r = 255
                        g = int(120.0 + glow * 100.0)
                        b = int(60.0 + glow * 80.0)
                    }
                }
            } elif (y < horizon + 4) {
                r = 255
                g = 90
                b = 210
            } else {
                r = 6
                g = 6
                b = 26
                let dist = y - horizon - 4
                let max_dist = h - horizon - 4
                let scale = float(dist) / float(max_dist)
                let spacing = max(2, int(2.0 + scale * 18.0))
                if (dist % spacing < 1) {
                    r = 0
                    g = 220
                    b = 255
                }
                let dx = abs(x - w / 2)
                let v_spacing = max(2, int(4.0 + scale * 40.0))
                if (dx % v_spacing < 2 && scale > 0.02) {
                    r = (r + 0) / 2
                    g = (g + 200) / 2
                    b = (b + 255) / 2
                }
            }
            let idx = (y * w + x) * 4
            pixels[idx + 0] = uint8(clamp(r, 0, 255))
            pixels[idx + 1] = uint8(clamp(g, 0, 255))
            pixels[idx + 2] = uint8(clamp(b, 0, 255))
            pixels[idx + 3] = 255u8
        }
    }
    return <- pixels
}

// (Re)create both render targets at w x h: a 1x path (single-sample colour + depth) and
// an N-sample MSAA path (multisample colour + depth). Both are renderbuffers -- never
// sampled, only blitted out. The multisample-resolve blit requires source and
// destination rectangles to match in size, so the targets track the live display size.
def setup_targets(w, h : int) {
    if (fbo_1x != 0u) {
        glDeleteFramebuffers(1, safe_addr(fbo_1x))
        glDeleteFramebuffers(1, safe_addr(fbo_msaa))
        glDeleteRenderbuffers(1, safe_addr(color_1x))
        glDeleteRenderbuffers(1, safe_addr(depth_1x))
        glDeleteRenderbuffers(1, safe_addr(color_msaa))
        glDeleteRenderbuffers(1, safe_addr(depth_msaa))
    }
    // 1x path
    glGenRenderbuffers(1, safe_addr(color_1x))
    glBindRenderbuffer(GL_RENDERBUFFER, color_1x)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, w, h)
    glGenRenderbuffers(1, safe_addr(depth_1x))
    glBindRenderbuffer(GL_RENDERBUFFER, depth_1x)
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h)
    glGenFramebuffers(1, safe_addr(fbo_1x))
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_1x)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_1x)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_1x)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("1x framebuffer incomplete")
    }
    // N-sample MSAA path
    glGenRenderbuffers(1, safe_addr(color_msaa))
    glBindRenderbuffer(GL_RENDERBUFFER, color_msaa)
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, w, h)
    glGenRenderbuffers(1, safe_addr(depth_msaa))
    glBindRenderbuffer(GL_RENDERBUFFER, depth_msaa)
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT16, w, h)
    glGenFramebuffers(1, safe_addr(fbo_msaa))
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_msaa)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_msaa)
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_msaa)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("MSAA framebuffer incomplete")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    target_w = w
    target_h = h
}

def create_gl_objects {
    program = create_shader_program(@@vs_main, @@fs_main)
    var verts : array<Vertex>
    var indices : array<int>
    build_spiky_geometry(verts, indices)
    index_count = length(indices)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Vertex>)
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)
    delete verts
    delete indices

    var pixels <- gen_synthwave_texture(TEX_DIM, TEX_DIM)
    texture = load_image_from_bytes(TEX_DIM, TEX_DIM, unsafe(addr(pixels[0])))
    delete pixels

    var max_samples = 1
    glGetIntegerv(GL_MAX_SAMPLES, safe_addr(max_samples))
    samples = min(4, max_samples)
    setup_targets(FBW, FBH)
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(FBW, FBH, "OpenGL - 09 MSAA", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    create_gl_objects()
}

// draw the spiky ball into the currently-bound framebuffer at target_w x target_h
def draw_ball {
    glViewport(0, 0, target_w, target_h)
    glClearColor(0.02, 0.01, 0.05, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    glUseProgram(program)
    u_tex := texture
    vs_main_bind_uniform(program)
    fs_main_bind_uniform(program)
    glBindVertexArray(vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, null)
}

[export]
def update : bool {
    time += 1.0 / 60.0
    let t = time
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let dw = max(display_w, 2)
    let dh = max(display_h, 1)
    let aspect = float(dw) / float(dh)
    if (dw != target_w || dh != target_h) {
        setup_targets(dw, dh)
    }

    // spinning ball, slowly orbiting camera -- the spin makes the 1x silhouette crawl
    let cam_pos = float3(cos(t * 0.25) * 2.8, 0.9, sin(t * 0.25) * 2.8)
    let rot = quat_from_unit_vec_ang(normalize(float3(0.2, 1.0, 0.1)), t * 0.5)
    u_model = compose(float3(0, 0, 0), rot, float3(1.0))
    u_view = look_at_rh(cam_pos, float3(0, 0, 0), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(45.0 * PI / 180.0, aspect, 0.1, 50.0)
    u_cam_pos = cam_pos
    u_time = t

    // Render the SAME ball twice: once into the 1x target, once into the MSAA target.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_1x)
    draw_ball()
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_msaa)
    draw_ball()

    // Compose the comparison: LEFT half of the screen from the 1x image, RIGHT half from
    // the resolved MSAA image. Both blits are 1:1 (same-size source/dest rectangles), as
    // a multisample-resolve blit requires.
    let half = target_w / 2
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0u)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_1x)
    glBlitFramebuffer(0, 0, half, target_h, 0, 0, half, target_h, GL_COLOR_BUFFER_BIT, GL_NEAREST)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_msaa)
    glBlitFramebuffer(half, 0, target_w, target_h, half, 0, target_w, target_h, GL_COLOR_BUFFER_BIT, GL_NEAREST)

    // a bright divider down the seam: left of it is 1x (jaggy), right is 4x MSAA (smooth)
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glEnable(GL_SCISSOR_TEST)
    glScissor(half - 1, 0, 3, target_h)
    glClearColor(0.95, 0.85, 0.2, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glDisable(GL_SCISSOR_TEST)

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
