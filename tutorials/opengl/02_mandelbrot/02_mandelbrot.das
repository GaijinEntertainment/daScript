options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/safe_addr

// 02 - The Mandelbrot Set (OpenGL / WebGL2 tutorial)
//
// The Vulkan rung computes the set in a [compute_shader], one invocation per
// pixel, into a storage image. WebGL2 has no compute / no storage images, so
// the portable GL idiom is a FULLSCREEN FRAGMENT shader: draw two triangles
// covering the whole viewport and let the fragment stage run the escape-time
// loop once per pixel. Same math, same daslang shader language -- a different
// backend and a different "one thread per pixel" vehicle.
//
// Each fragment maps its clip-space position to a complex number c, iterates
// z = z*z + c until |z| > 2 or it hits MAX_ITER, and colors the pixel from the
// escape count. A per-frame `u_zoom` uniform breathes the view in and out, so
// the loop has a real GPU parameter; `u_aspect` keeps the set from stretching.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web
// the wasm run path drives init/update/shutdown once per browser frame.

var @in @location v_position : float2
var @uniform u_aspect : float
var @uniform u_zoom : float
var @inout f_uv : float2
var @out f_FragColor : float4

[vertex_program]
def vs_main {
    f_uv = v_position
    gl_Position = float4(v_position, 0.0, 1.0)
}

[fragment_program]
def fs_main {
    // The seahorse valley, near the boundary of the set, where the filigree
    // detail lives. Fixed center, so it is an inline constant rather than a
    // uniform; only the zoom animates.
    let center = float2(-0.745, 0.115)
    let cx = center.x + f_uv.x * u_aspect * u_zoom
    let cy = center.y + f_uv.y * u_zoom
    var zx = 0.0
    var zy = 0.0
    var n = 0
    let MAX_ITER = 256
    while (n < MAX_ITER) {
        let x2 = zx * zx
        let y2 = zy * zy
        if (x2 + y2 > 4.0) {
            break
        }
        let nzx = x2 - y2 + cx
        zy = 2.0 * zx * zy + cy
        zx = nzx
        n++
    }
    if (n == MAX_ITER) {
        // inside the set: black
        f_FragColor = float4(0.0, 0.0, 0.0, 1.0)
    } else {
        // outside: an arithmetic ramp on the escape count (no trig)
        let t = float(n) / float(MAX_ITER)
        f_FragColor = float4(t, t * 0.5, 1.0 - t, 1.0)
    }
}

var program : uint
var vao : uint
var vbo : uint
var ebo : uint
var window : GLFWwindow?
var phase : float = 0.0

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
    phase += 0.01
    // Ping-pong the zoom between a wide view (1.3) and deep filigree (0.03),
    // easing toward the close-up so it dwells on the detail.
    let s = sin(phase) * 0.5 + 0.5
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let h = max(display_h, 1)
    u_aspect = float(display_w) / float(h)
    u_zoom = 0.03 + (1.3 - 0.03) * (s * s)
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
