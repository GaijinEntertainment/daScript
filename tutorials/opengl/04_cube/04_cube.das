options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 04 - The Synthwave Cube (OpenGL / WebGL2 tutorial)
//
// The first 3D-geometry rung. A textured, lit, breathing cube spins while the
// camera orbits it. This introduces the pieces the fullscreen-fragment rungs
// (01-03) skipped: a real vertex buffer with per-vertex (pos, normal, uv), the
// model / view / projection matrix chain (mat4s built host-side with
// daslib/math_boost and packed into a std140 uniform block), depth testing +
// back-face culling, and a texture sampled in the fragment shader.
//
// The matrices + lighting params live in one `struct Camera` declared `@uniform`:
// dasGlsl lowers it to a `layout(std140) uniform` interface block and the host
// uploads it through a single uniform buffer, the GL/WebGL2-portable way to feed
// a shader a block of constants (and the same std140 path the dasVulkan rail uses).
//
// A faithful port of the dasVulkan rung (same look): the cube wears a procedural
// "synthwave horizon" texture -- deep-purple-to-magenta sky with a banded neon
// sun, a magenta horizon line, and a dark-indigo ground with a cyan perspective
// grid -- generated CPU-side at init (no asset file). The fragment scrolls the UV
// vertically over time so the texture drifts like a CRT, and a key + rim light
// pair shades it. Every line of the shader is daslang, lowered to GLSL by dasGlsl.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web
// the wasm run path drives init/update/shutdown once per browser frame.

var @in @location a_pos : float3
var @in @location a_normal : float3
var @in @location a_uv : float2

// The camera's per-frame transform + lighting params, packed into one std140 UBO. A struct @uniform
// emits a `layout(std140) uniform u_cam_ubo { ... } u_cam;` interface block (the GL/WebGL2 portable
// floor) and the host bind uploads it through a single uniform-buffer, instead of one glUniform* call
// per field. cam_pos (float3) sits right before time (float) on purpose: that float3-then-scalar pair
// is the case naive packing gets wrong, and exercises the std140 layout walk's per-field offsets.
struct Camera {
    model   : float4x4
    view    : float4x4
    proj    : float4x4
    cam_pos : float3
    time    : float
}
var @uniform u_cam : Camera
var @uniform u_tex : sampler2D
var @inout w_pos : float3
var @inout w_normal : float3
var @inout w_uv : float2
var @out f_FragColor : float4

[vertex_program]
def vs_main {
    let world = u_cam.model * float4(a_pos, 1.0)
    gl_Position = u_cam.proj * u_cam.view * world
    w_pos = world.xyz
    // model is rotation + uniform scale only, so a (normal, 0) multiply transforms
    // the normal correctly without an inverse-transpose.
    w_normal = (u_cam.model * float4(a_normal, 0.0)).xyz
    w_uv = a_uv
}

[fragment_program]
def fs_main {
    let n = normalize(w_normal)
    let v = normalize(u_cam.cam_pos - w_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))            // warm key light from above-front
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3            // ambient floor 0.3
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.0)        // silhouette glow
    let rim_color = float3(0.4, 0.6, 1.0)                // cool cyan
    let scrolled = float2(w_uv.x, w_uv.y + u_cam.time * 0.05)
    let albedo = texture(u_tex, scrolled).xyz
    let lit = albedo * key + rim_color * rim
    f_FragColor = float4(lit, 1.0)
}

var program : uint
var vao : uint
var vbo : uint
var ebo : uint
var texture : uint
var window : GLFWwindow?
var time : float = 0.0

[vertex_buffer]
struct Vertex {
    xyz : float3
    normal : float3
    uv : float2
}

let vertices = [Vertex(
    xyz=float3(1, 1, 1), normal=float3(0, 0, 1), uv=float2(0, 0)), Vertex(
    xyz=float3(-1, 1, 1), normal=float3(0, 0, 1), uv=float2(1, 0)), Vertex(
    xyz=float3(-1, -1, 1), normal=float3(0, 0, 1), uv=float2(1, 1)), Vertex(
    xyz=float3(1, -1, 1), normal=float3(0, 0, 1), uv=float2(0, 1)), Vertex(
    xyz=float3(1, 1, 1), normal=float3(1, 0, 0), uv=float2(0, 0)), Vertex(
    xyz=float3(1, -1, 1), normal=float3(1, 0, 0), uv=float2(1, 0)), Vertex(
    xyz=float3(1, -1, -1), normal=float3(1, 0, 0), uv=float2(1, 1)), Vertex(
    xyz=float3(1, 1, -1), normal=float3(1, 0, 0), uv=float2(0, 1)), Vertex(
    xyz=float3(1, 1, 1), normal=float3(0, 1, 0), uv=float2(0, 0)), Vertex(
    xyz=float3(1, 1, -1), normal=float3(0, 1, 0), uv=float2(1, 0)), Vertex(
    xyz=float3(-1, 1, -1), normal=float3(0, 1, 0), uv=float2(1, 1)), Vertex(
    xyz=float3(-1, 1, 1), normal=float3(0, 1, 0), uv=float2(0, 1)), Vertex(
    xyz=float3(-1, 1, 1), normal=float3(-1, 0, 0), uv=float2(0, 0)), Vertex(
    xyz=float3(-1, 1, -1), normal=float3(-1, 0, 0), uv=float2(1, 0)), Vertex(
    xyz=float3(-1, -1, -1), normal=float3(-1, 0, 0), uv=float2(1, 1)), Vertex(
    xyz=float3(-1, -1, 1), normal=float3(-1, 0, 0), uv=float2(0, 1)), Vertex(
    xyz=float3(-1, -1, -1), normal=float3(0, -1, 0), uv=float2(0, 0)), Vertex(
    xyz=float3(1, -1, -1), normal=float3(0, -1, 0), uv=float2(1, 0)), Vertex(
    xyz=float3(1, -1, 1), normal=float3(0, -1, 0), uv=float2(1, 1)), Vertex(
    xyz=float3(-1, -1, 1), normal=float3(0, -1, 0), uv=float2(0, 1)), Vertex(
    xyz=float3(1, -1, -1), normal=float3(0, 0, -1), uv=float2(0, 0)), Vertex(
    xyz=float3(-1, -1, -1), normal=float3(0, 0, -1), uv=float2(1, 0)), Vertex(
    xyz=float3(-1, 1, -1), normal=float3(0, 0, -1), uv=float2(1, 1)), Vertex(
    xyz=float3(1, 1, -1), normal=float3(0, 0, -1), uv=float2(0, 1)
)];

let indices = fixed_array<int>(
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20)

let TEX_DIM = 256

// 256x256 RGBA8 synthwave horizon, generated CPU-side -- no asset file. Deep-
// purple-to-magenta sky with a banded neon sun, a magenta horizon line, and a
// dark-indigo ground with a cyan perspective grid receding to the vanishing point.
def gen_synthwave_texture(w, h : int) : array<uint8> {
    var pixels : array<uint8>
    pixels |> resize(w * h * 4)
    let horizon = h / 2 - 8                    // horizon a bit above center
    let sun_cx = w / 2
    let sun_cy = horizon - 26
    let sun_r = 42
    for (y in range(h)) {
        for (x in range(w)) {
            var r = 0
            var g = 0
            var b = 0
            if (y < horizon) {
                // sky: deep purple at top, magenta near horizon
                let t = float(y) / float(horizon)
                r = int(40.0 + t * 215.0)
                g = int(6.0 + t * 64.0)
                b = int(80.0 + t * 175.0)
                // neon sun disk with horizontal scanline bands
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
                // bright magenta horizon line
                r = 255
                g = 90
                b = 210
            } else {
                // ground: dark indigo with cyan perspective grid
                r = 6
                g = 6
                b = 26
                let dist = y - horizon - 4
                let max_dist = h - horizon - 4
                let scale = float(dist) / float(max_dist)
                // horizontal lines, spacing widens with distance from horizon
                let spacing = max(2, int(2.0 + scale * 18.0))
                if (dist % spacing < 1) {
                    r = 0
                    g = 220
                    b = 255
                }
                // vertical lines converging to (w/2, horizon)
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

def create_gl_objects {
    program = create_shader_program(@@vs_main, @@fs_main)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Vertex>)
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)
    var pixels <- gen_synthwave_texture(TEX_DIM, TEX_DIM)
    texture = load_image_from_bytes(TEX_DIM, TEX_DIM, unsafe(addr(pixels[0])))
    delete pixels
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 04 synthwave cube", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    create_gl_objects()
}

[export]
def update : bool {
    time += 1.0 / 60.0
    let t = time
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let h = max(display_h, 1)
    let aspect = float(display_w) / float(h)

    // camera orbits the cube; pass its world position to the fragment for the rim
    let cam_pos = float3(cos(t * 0.5) * 4.0, 1.5, sin(t * 0.5) * 4.0)
    // breathing scale + tumble about a tilted axis
    let breathe = 1.0 + 0.05 * sin(t * 2.0)
    let rot = quat_from_unit_vec_ang(normalize(float3(1.0, 1.0, 0.0)), t * 0.6)
    u_cam.model = compose(float3(0, 0, 0), rot, float3(breathe))
    u_cam.view = look_at_rh(cam_pos, float3(0, 0, 0), float3(0, 1, 0))
    u_cam.proj = perspective_rh_opengl(45.0 * PI / 180.0, aspect, 0.1, 50.0)
    u_cam.cam_pos = cam_pos
    u_cam.time = t

    glViewport(0, 0, display_w, display_h)
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
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, null)
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
