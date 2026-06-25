options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/safe_addr

// 01 - The Rotating Triangle (OpenGL / WebGL2 tutorial)
//
// The canonical hello-triangle: three vertices, per-vertex black/yellow/red,
// interpolated across the face -- with every line of the shader written in
// daslang (the shared `shader_lingua_franca` builtins), lowered to GLSL by
// dasGlsl. A per-frame `u_angle` uniform spins it, so the loop has a real GPU
// parameter, and `u_aspect` keeps it square at any window/canvas size.
//
// This is ONE homogeneous program: `main()` is the desktop windowed viewer
// (init/update/shutdown driver); on the web the wasm run path calls
// init/update/shutdown directly and drives update() once per browser frame
// (the Context persists across frames). Same .das, both platforms.

var @in @location v_position : float2
var @in @location v_color : float3
var @uniform u_angle : float
var @uniform u_aspect : float
var @inout f_color : float3
var @out f_FragColor : float4

[vertex_program]
def vs_main {
    f_color = v_color
    let c = cos(u_angle)
    let s = sin(u_angle)
    let p = v_position
    let r = float2(p.x * c - p.y * s, p.x * s + p.y * c)
    gl_Position = float4(r.x / u_aspect, r.y, 0.0, 1.0)
}

[fragment_program]
def fs_main {
    f_FragColor = float4(f_color, 1.0)
}

var program : uint
var vao : uint
var vbo : uint
var window : GLFWwindow?
var angle : float = 0.0

[vertex_buffer]
struct Vertex {
    xy : float2
    rgb : float3
}

// A symmetric, centered triangle (mirrors the dasVulkan 01 coordinates; apex up
// in GL's y-up clip space). u_aspect (below) keeps it from stretching on a
// non-square window/canvas.
let vertices = [Vertex(
    xy=float2(0.0, 0.6), rgb=float3(0.0, 0.0, 0.0)), Vertex(
    xy=float2(-0.6, -0.6), rgb=float3(1.0, 1.0, 0.0)), Vertex(
    xy=float2(0.6, -0.6), rgb=float3(1.0, 0.0, 0.0)
)];

def create_gl_objects {
    program = create_shader_program(@@vs_main, @@fs_main)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<Vertex>)
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 01 rotating triangle", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    create_gl_objects()
}

[export]
def update : bool {
    angle += 0.02
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let h = max(display_h, 1)
    u_angle = angle
    u_aspect = float(display_w) / float(h)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.1, 0.1, 0.12, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glUseProgram(program)
    vs_main_bind_uniform(program)
    fs_main_bind_uniform(program)
    glBindVertexArray(vao)
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

// Desktop driver. On the web this is never called -- the run path drives the
// three lifecycle functions directly and persists the Context across frames.
[export]
def main {
    init()
    while (update()) {
    }
    shutdown()
}
