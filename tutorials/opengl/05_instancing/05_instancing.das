options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 05 - The Cube Swarm (OpenGL / WebGL2 tutorial)
//
// A thousand cubes from one draw call. After 04's single textured cube, this rung
// introduces instancing: the same 36-index cube is fanned over N_INSTANCES copies
// by `glDrawElementsInstanced`, each copy reading its own world offset, colour and
// animation phase from a second, per-instance vertex-attribute stream.
//
// Two attribute streams share one VAO:
//   * per-vertex  (binding 0, locations 0-1): pos + normal, stepped once per vertex
//   * per-instance(binding 1, locations 2-4): offset + colour + phase, stepped once
//     per instance via `glVertexAttribDivisor(loc, 1)`
// The shader sees all five as plain `@in @location` inputs -- there is no special
// instancing keyword; instancing lives entirely on the host side, in the divisor.
//
// A faithful port of the dasVulkan rung (same Lissajous swarm + breathing): the
// instance buffer is static, built once at init from a 3D Lissajous curve seeded by
// instance index, and the swarm is animated entirely in the vertex shader by mixing
// each instance's offset with `u_time` and its own phase. Every line of the shader
// is daslang, lowered to GLSL by dasGlsl.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web the
// wasm run path drives init/update/shutdown once per browser frame.

// per-vertex inputs (binding 0)
var @in @location = 0 a_pos : float3
var @in @location = 1 a_normal : float3
// per-instance inputs (binding 1) -- explicit locations so they line up with the
// host-side bind_vertex_buffer(..., 2u) offset; the divisor makes them step per instance
var @in @location = 2 a_offset : float3
var @in @location = 3 a_color : float3
var @in @location = 4 a_phase : float
var @uniform u_view : float4x4
var @uniform u_proj : float4x4
var @uniform u_cam_pos : float3
var @uniform u_time : float
var @inout w_pos : float3
var @inout w_normal : float3
var @inout w_color : float3
var @out f_FragColor : float4

[vertex_program]
def vs_main {
    let t = u_time
    // per-instance breathing: each cube scales between ~0.5 and ~0.9 at its own phase offset
    let breathe = 0.7 + 0.2 * sin(t * 1.5 + a_phase * 6.2832)
    let local = a_pos * breathe
    // gentle per-instance spin about Y, desynchronised by the phase
    let ang = t * 0.25 + a_phase * 6.2832
    let c = cos(ang)
    let s = sin(ang)
    let rotated = float3(c * local.x - s * local.z, local.y, s * local.x + c * local.z)
    let world = rotated + a_offset
    gl_Position = u_proj * u_view * float4(world, 1.0)
    w_pos = world
    // the normal rotates by the same Y rotation; uniform scale only, so no inverse-transpose
    w_normal = float3(c * a_normal.x - s * a_normal.z, a_normal.y, s * a_normal.x + c * a_normal.z)
    w_color = a_color
}

[fragment_program]
def fs_main {
    let n = normalize(w_normal)
    let v = normalize(u_cam_pos - w_pos)
    let l = normalize(float3(0.5, 1.0, 0.3))            // warm key light from above-front
    let key = max(dot(n, l), 0.0) * 0.7 + 0.3            // ambient floor 0.3
    let rim = pow(1.0 - max(dot(n, v), 0.0), 2.5)        // silhouette glow
    let lit = w_color * key + float3(0.25, 0.35, 0.55) * rim * 0.4
    f_FragColor = float4(lit, 1.0)
}

var program : uint
var vao : uint
var vbo : uint
var inst_vbo : uint
var ebo : uint
var window : GLFWwindow?
var time : float = 0.0

// 1000 small cubes -- comfortably visible at 640x480 with a ~6-unit swarm radius
// and the camera orbiting at distance 14.
let N_INSTANCES = 1000

[vertex_buffer]
struct Vertex {
    xyz : float3
    normal : float3
}

// per-instance row: world offset + RGB tint + animation phase. All-float, so the
// [vertex_buffer] macro emits three GL_FLOAT attributes (3 + 3 + 1 components).
[vertex_buffer]
struct Instance {
    offset : float3
    color : float3
    phase : float
}

// faceted unit cube (half-extent 0.5), per-face normals, 24 verts / 36 indices --
// same winding as tutorial 04, which renders correctly under GL_CULL_FACE GL_BACK.
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

// Build N_INSTANCES rows of (offset, colour, phase). Offsets sample a Lissajous-style
// 3D curve (7 azimuthal lobes against 3 elevation lobes against radial breathing) so
// the swarm fills a balanced volume with no obvious axis alignment. Colours cycle the
// hue circle three times across the swarm; phase sweeps [0,1) so the per-cube breathing
// in the vertex shader is desynchronised.
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
        let hue = fi * 6.0 * PI                // three full hue cycles
        let cr = 0.5 + 0.5 * cos(hue)
        let cg = 0.5 + 0.5 * cos(hue + 2.0944)            // 2 pi / 3
        let cb = 0.5 + 0.5 * cos(hue + 4.1888)            // 4 pi / 3
        result[i] = Instance(offset = float3(x, y, z), color = float3(cr, cg, cb), phase = fi)
    }
    return <- result
}

def create_gl_objects {
    program = create_shader_program(@@vs_main, @@fs_main)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    // per-vertex geometry (binding 0) -> locations 0,1, divisor 0 (default)
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Vertex>)
    // per-instance attributes (binding 1) -> locations 2,3,4, divisor 1. Each
    // glVertexAttribPointer records the buffer bound *now* (inst_vbo) into the VAO,
    // so the two streams coexist; the divisor advances them once per instance.
    var instances <- gen_instances(N_INSTANCES)
    glGenBuffers(1, safe_addr(inst_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, inst_vbo)
    glBufferData(GL_ARRAY_BUFFER, instances, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Instance>, 2u)
    glVertexAttribDivisor(2u, 1u)
    glVertexAttribDivisor(3u, 1u)
    glVertexAttribDivisor(4u, 1u)
    delete instances
    glGenBuffers(1, safe_addr(ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW)
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 05 cube swarm", null, null)
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

    // camera orbits the swarm with a gentle vertical sway to show depth
    let camera_angle = t * 0.3
    let camera_r = 14.0
    let cam_pos = float3(camera_r * cos(camera_angle), sin(camera_angle * 2.0) * 2.0, camera_r * sin(camera_angle))
    u_view = look_at_rh(cam_pos, float3(0, 0, 0), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(60.0 * PI / 180.0, aspect, 0.1, 60.0)
    u_cam_pos = cam_pos
    u_time = t

    glViewport(0, 0, display_w, display_h)
    glClearColor(0.02, 0.0, 0.04, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glEnable(GL_CULL_FACE)
    glCullFace(GL_BACK)
    glUseProgram(program)
    vs_main_bind_uniform(program)
    fs_main_bind_uniform(program)
    glBindVertexArray(vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    // the instancing call: 36 indices fanned over N_INSTANCES instances
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, null, N_INSTANCES)
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
