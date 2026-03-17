options gen2

require glfw/glfw_live
require opengl/opengl_boost
require daslib/decs_boost
require daslib/decs_live
require daslib/safe_addr
require daslib/math_boost
require live_host

// --- Shaders ---

var @in @location v_position : float2
var @in @location v_color : float3
var @inout f_color : float3
var @out f_FragColor : float4

[vertex_program]
def vs_triangle {
    f_color = v_color
    gl_Position = float4(v_position, 0.0, 1.0)
}

[fragment_program]
def fs_triangle {
    f_FragColor = float4(f_color, 1.0)
}

// --- GL objects (re-created every init) ---

var program : uint
var vao : uint
var vbo : uint

[vertex_buffer]
struct Vertex {
    xy : float2
    rgb : float3
}

def create_gl_objects() {
    program = create_shader_program(@@vs_triangle, @@fs_triangle)
    glGenVertexArrays(1, safe_addr(vao))
    glBindVertexArray(vao)
    glGenBuffers(1, safe_addr(vbo))
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    // Allocate buffer (will be updated each frame)
    var dummy : Vertex[3]
    glBufferData(GL_ARRAY_BUFFER, dummy, GL_DYNAMIC_DRAW)
    bind_vertex_buffer(null, type<Vertex>)
}

def delete_gl_objects() {
    if (vao != 0u) {
        glDeleteVertexArrays(1, safe_addr(vao))
        vao = 0u
    }
    if (vbo != 0u) {
        glDeleteBuffers(1, safe_addr(vbo))
        vbo = 0u
    }
    // shader program is cached, no need to delete
}

// --- DECS: triangle state ---

[decs_template]
struct RotatingTriangle {
    eid : EntityId
    angle : float       // current rotation angle in radians
    speed : float       // radians per second
    radius : float      // distance from center
}

def make_triangle() {
    create_entity <| @(eid, cmp) {
        apply_decs_template(cmp, RotatingTriangle(
            eid = eid,
            angle = 0.0,
            speed = 1.5,
            radius = 0.6
        ))
    }
    commit()
}

// --- Update ---

def update_triangle(dt : float) {
    query() <| $(var angle : float&; speed : float) {
        angle += speed * dt
    }
}

def get_triangle_vertices(var vertices : Vertex[3]&) {
    query() <| $(angle : float; radius : float) {
        // Three vertices 120 degrees apart, rotated by angle
        let colors = fixed_array(float3(1.0, 0.2, 0.2), float3(0.2, 1.0, 0.2), float3(0.2, 0.2, 1.0))
        for (i in range(3)) {
            let a = angle + float(i) * (2.0 * PI / 3.0)
            vertices[i] = Vertex(xy = float2(cos(a), sin(a)) * radius, rgb = colors[i])
        }
    }
}

// --- Lifecycle ---

[export]
def init() {
    live_create_window("Rotating Triangle", 800, 600)
    create_gl_objects()
    // Only create entity on first run (DECS state survives reload)
    if (!is_reload()) {
        decs::restart()
        make_triangle()
    }
}

[export]
def update() {
    if (!live_begin_frame()) {
        return
    }
    update_triangle(get_dt())
    // Render
    var w, h : int
    live_get_framebuffer_size(w, h)
    glViewport(0, 0, w, h)
    glClearColor(0.15, 0.15, 0.2, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glUseProgram(program)
    vs_triangle_bind_uniform(program)
    fs_triangle_bind_uniform(program)
    // Upload triangle vertices
    var vertices : Vertex[3]
    get_triangle_vertices(vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    unsafe {
        glBufferSubData(GL_ARRAY_BUFFER, 0l, int64(typeinfo sizeof(vertices)), addr(vertices[0]))
    }
    glBindVertexArray(vao)
    glDrawArrays(GL_TRIANGLES, 0, 3)
    live_end_frame()
}

[export]
def shutdown() {
    delete_gl_objects()
    live_destroy_window()
}

// Dual-mode: also works with regular daslang.exe
[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
    }
    shutdown()
}
