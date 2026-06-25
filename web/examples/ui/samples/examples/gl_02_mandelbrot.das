options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/safe_addr

// 02 - The Mandelbrot Set (OpenGL / WebGL2 tutorial)
//
// The dasVulkan rung computes the set in a [compute_shader], one invocation per
// pixel, into a storage image. WebGL2 has no compute / no storage images, so the
// portable GL idiom is a FULLSCREEN FRAGMENT shader: draw two triangles covering
// the whole viewport and let the fragment stage run the escape-time loop once per
// pixel. Same math, same daslang shader language -- a different backend and a
// different "one thread per pixel" vehicle.
//
// This is a faithful port of the Vulkan tutorial's animated viewer, itself a
// port of Inigo Quilez's smooth-coloured Mandelbrot zoom
// (https://www.shadertoy.com/view/ldf3DN): a single `u_time` uniform drives an
// oscillating zoom and a slow rotation about the seahorse-valley point
// (-0.745, 0.186). A large bailout radius + continuous (smooth) iteration count
// kills the banding; an IQ cosine palette colours it; 4x4 supersampling
// antialiases the boundary. Every line of the shader is daslang, lowered to GLSL
// by dasGlsl.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web
// the wasm run path drives init/update/shutdown once per browser frame.

var @in @location v_position : float2
var @uniform u_time : float
var @uniform u_aspect : float
var @uniform u_res : float2
var @inout f_uv : float2
var @out f_FragColor : float4

let MAX_ITER = 512
let ESCAPE2 = 65536.0       // B*B with the IQ bailout radius B = 256
let AA = 4                  // 4x4 supersampling: 16 colour samples per pixel

// complex multiply -- (a.x + i a.y) * (b.x + i b.y)
def private cmul(a, b : float2) : float2 {
    return float2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)
}

// IQ's cheap interior tests: skip iteration inside the main cardioid and the
// period-2 bulb.
def private is_interior(c : float2) : bool {
    let c2 = c.x * c.x + c.y * c.y
    let in_cardioid = 256.0 * c2 * c2 - 96.0 * c2 + 32.0 * c.x - 3.0 < 0.0
    let in_bulb = 16.0 * (c2 + 2.0 * c.x + 1.0) - 1.0 < 0.0
    return in_cardioid || in_bulb
}

// Escape-time iteration with a large bailout radius (needed for smooth colouring).
// Returns (iteration_count, exit_magnitude |z|^2). n == MAX_ITER with d <= ESCAPE2
// means the point did not escape (treat as interior).
def private mandel_smooth(c : float2) : float2 {
    var z = float2(0.0, 0.0)
    var n = 0
    while (n < MAX_ITER) {
        z = cmul(z, z) + c
        if (z.x * z.x + z.y * z.y > ESCAPE2) {
            break
        }
        n++
    }
    return float2(float(n), z.x * z.x + z.y * z.y)
}

// IQ's cosine palette evaluated at a continuous-iteration phase t.
def private palette(t : float) : float3 {
    let phase = 3.0 + t * 0.15
    return float3(0.5 + 0.5 * cos(phase),
                  0.5 + 0.5 * cos(phase + 0.6),
                  0.5 + 0.5 * cos(phase + 1.0))
}

// One supersample at complex point c: interior -> black, else continuous
// iteration count -> palette.
def private sample_at(c : float2) : float3 {
    if (is_interior(c)) {
        return float3(0.0, 0.0, 0.0)
    }
    let nd = mandel_smooth(c)
    if (nd.y > ESCAPE2) {
        let sn = nd.x - log2(log2(nd.y)) + 4.0
        return palette(sn)
    }
    return float3(0.0, 0.0, 0.0)
}

[vertex_program]
def vs_main {
    f_uv = v_position
    gl_Position = float4(v_position, 0.0, 1.0)
}

[fragment_program]
def fs_main {
    let t = u_time
    // animated zoom (oscillates) + slow rotation, both derived from time
    let zoo0 = 0.62 + 0.38 * cos(0.07 * t)
    let ang = 0.15 * (1.0 - zoo0) * t
    let coa = cos(ang)
    let sia = sin(ang)
    let zoo = pow(zoo0, 8.0)
    // base screen coords: x widened by aspect, y flipped to match the Vulkan
    // image orientation (y down). One screen pixel spans 2/res.y in this space.
    let base_px = f_uv.x * u_aspect
    let base_py = -f_uv.y
    let pix = 2.0 / u_res.y
    // average AA*AA subpixel colour samples; interior samples contribute black, so
    // the set boundary softens correctly.
    var col = float3(0.0, 0.0, 0.0)
    for (s in range(AA * AA)) {
        let ox = (float(s % AA) + 0.5) / float(AA) - 0.5
        let oy = (float(s / AA) + 0.5) / float(AA) - 0.5
        let px = base_px + ox * pix
        let py = base_py + oy * pix
        let xr = px * coa - py * sia
        let yr = px * sia + py * coa
        let c = float2(-0.745 + xr * zoo, 0.186 + yr * zoo)
        col += sample_at(c)
    }
    let avg = col * (1.0 / float(AA * AA))
    f_FragColor = float4(avg, 1.0)
}

var program : uint
var vao : uint
var vbo : uint
var ebo : uint
var window : GLFWwindow?
var time : float = 0.0

[vertex_buffer]
struct Vertex {
    xy : float2
}

// A fullscreen quad in clip space; the fragment shader does all the work.
let vertices = [Vertex(
    xy=float2(-1.0, -1.0)), Vertex(
    xy=float2(1.0, -1.0)), Vertex(
    xy=float2(1.0, 1.0)), Vertex(
    xy=float2(-1.0, 1.0)
)];

let indices = fixed_array<int>(0, 1, 2, 2, 3, 0)

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
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 02 mandelbrot", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    create_gl_objects()
}

[export]
def update : bool {
    time += 1.0 / 60.0
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let h = max(display_h, 1)
    u_time = time
    u_aspect = float(display_w) / float(h)
    u_res = float2(float(display_w), float(h))
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glUseProgram(program)
    vs_main_bind_uniform(program)
    fs_main_bind_uniform(program)
    glBindVertexArray(vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)
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
