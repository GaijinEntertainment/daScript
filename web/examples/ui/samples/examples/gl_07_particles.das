options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 07 - The Particle Swarm (OpenGL / WebGL2 tutorial)
//
// The first GL-idiom-substitution rung. The dasVulkan particle tutorial runs the
// simulation in a COMPUTE shader: a storage buffer of particles is rewritten each
// frame (gravity pull toward the origin, explicit-Euler integrated), then the same
// buffer is drawn as instanced point sprites. WebGL2 has no compute shaders and no
// storage buffers -- so this rung shows the canonical GL substitute: TRANSFORM
// FEEDBACK. A vertex shader reads each particle's state as vertex attributes,
// integrates it, and its outputs are captured straight back into a buffer instead of
// being rasterised. Two buffers ping-pong (read A, write B; next frame read B, write
// A); a second program then draws the freshly-written buffer as glowing point sprites.
//
// The three rails that make a "compute pass" out of a vertex shader:
//
//   * TRANSFORM-FEEDBACK VARYINGS -- the sim vertex shader's `@out tf_pos / tf_vel /
//     tf_life` are registered with `glTransformFeedbackVaryings` (BEFORE linking) so
//     the GPU writes them, interleaved, into the bound feedback buffer.
//   * RASTERIZER DISCARD -- `glEnable(GL_RASTERIZER_DISCARD)` turns the sim draw into
//     a pure compute step: vertices are processed and captured, but nothing is drawn.
//   * PING-PONG -- the state buffer can't be read and written in the same draw, so two
//     buffers alternate roles each frame.
//
// One state layout serves both passes: a `[vertex_buffer] struct Particle` (pos, vel,
// life) gives attribute offsets 0 / 12 / 24, stride 28 -- and the transform-feedback
// capture order (tf_pos, tf_vel, tf_life) writes exactly that interleave, so the same
// VAO reads the captured bytes back as the next frame's input and as the render input.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web the
// wasm run path drives init/update/shutdown once per browser frame.

// ===== shared state attributes (read by BOTH the sim and the render vertex shader) =====
var @in @location = 0 a_pos : float3
var @in @location = 1 a_vel : float3
var @in @location = 2 a_life : float

// ===== simulation pass: a vertex shader used as a compute step =====
// Its outputs are captured by transform feedback into the destination state buffer.
var @out tf_pos : float3
var @out tf_vel : float3
var @out tf_life : float
var @uniform u_dt : float

[vertex_program(name = "sim_src")]
def sim_vs {
    // central attractor: a softened 1/r^2 pull toward the origin (the 0.05 softening keeps
    // particles that pass close to the origin from blowing up), explicit-Euler integrated.
    let r2 = dot(a_pos, a_pos)
    let r_mag = sqrt(r2 + 0.05)
    let inv_r3 = 1.0 / (r_mag * r_mag * r_mag)
    let accel = a_pos * (-inv_r3 * 1.8)
    let new_vel = a_vel + accel * u_dt
    let new_pos = a_pos + new_vel * u_dt
    tf_pos = new_pos
    tf_vel = new_vel
    tf_life = a_life            // life is a colour driver only, never decremented
}

// The sim program still needs a fragment shader to link; with RASTERIZER_DISCARD it
// never runs, so it just writes a constant.
var @out f_FragColor : float4

[fragment_program(name = "sim_dummy_src")]
def sim_dummy_fs {
    f_FragColor = float4(0.0, 0.0, 0.0, 1.0)
}

// ===== render pass: one glowing point sprite per particle =====
let PARTICLE_PX = 9.0           // screen-space splat width in pixels
var @uniform u_view : float4x4
var @uniform u_proj : float4x4
var @inout r_speed : float
var @inout r_life : float

[vertex_program(name = "render_vs_src")]
def render_vs {
    gl_Position = u_proj * u_view * float4(a_pos, 1.0)
    gl_PointSize = PARTICLE_PX  // vertex-stage builtin: the rasteriser splat width for GL_POINTS
    r_speed = sqrt(dot(a_vel, a_vel))
    r_life = a_life
}

[fragment_program(name = "render_fs_src")]
def render_fs {
    // gl_PointCoord is the [0,1]^2 UV inside the rasterised point; distance from the centre
    // gives a circular mask + soft cubic falloff -- a point sprite without uploading a texture.
    let fc = gl_PointCoord - float2(0.5, 0.5)
    let d = sqrt(dot(fc, fc)) * 2.0
    if (d > 1.0) {
        discard()               // outside the inscribed disc: punch a hole through the square
    }
    let core = 1.0 - clamp(d, 0.0, 1.0)
    let glow = core * core * core                       // hot core + halo
    // colour by speed: cool teal for slow particles, hot magenta for fast
    let cool = float3(0.20, 0.70, 1.00)
    let hot = float3(1.00, 0.30, 0.55)
    let s = clamp(r_speed * 0.4, 0.0, 1.0)
    let base = lerp(cool, hot, float3(s, s, s))
    // premultiplied colour with alpha = glow; ONE/ONE additive blend gives a pure additive glow
    let intensity = glow * r_life
    f_FragColor = float4(base * intensity, intensity)
}

// ===== GL objects =====

var sim_prog : uint
var render_prog : uint
var buf : uint[2]               // two state buffers, ping-ponged
var vao : uint[2]               // one VAO per buffer (reads it as a_pos/a_vel/a_life)
var cur = 0                     // which buffer holds the current state
var window : GLFWwindow?
var time : float = 0.0

let N_PARTICLES = 1024

[vertex_buffer]
struct Particle {
    pos : float3
    vel : float3
    life : float
}

// Seed N particles on a tilted ring of radius ~1.2 with tangential velocity. The ring
// decays slowly under the gravity integrator but stays lively; the initial ring is
// visually instructive -- you can watch the integrator perturb the orbit over time.
def gen_initial_particles(n : int) : array<Particle> {
    var result : array<Particle>
    result |> resize(n)
    let speed = 1.4
    for (i in range(n)) {
        let fi = float(i) / float(n)
        let theta = fi * 2.0 * PI
        let r = 1.2 + 0.15 * sin(fi * 2.0 * PI * 7.0)
        let x = r * cos(theta)
        let y = sin(fi * 2.0 * PI * 5.0) * 0.3
        let z = r * sin(theta)
        let vx = -speed * sin(theta)
        let vy = cos(fi * 2.0 * PI * 3.0) * 0.35
        let vz = speed * cos(theta)
        let life = 0.4 + 0.6 * fi
        result[i] = Particle(pos = float3(x, y, z), vel = float3(vx, vy, vz), life = life)
    }
    return <- result
}

// One state buffer + a VAO that reads it as the Particle attribute stream. Both ping-pong
// buffers are seeded identically so frame 0 reads valid data from either.
def make_state_buffer(seed : array<Particle>) : tuple<uint; uint> {
    var b : uint
    glGenBuffers(1, safe_addr(b))
    glBindBuffer(GL_ARRAY_BUFFER, b)
    glBufferData(GL_ARRAY_BUFFER, seed, GL_DYNAMIC_COPY)
    var v : uint
    glGenVertexArrays(1, safe_addr(v))
    glBindVertexArray(v)
    glBindBuffer(GL_ARRAY_BUFFER, b)
    bind_vertex_buffer(null, type<Particle>)
    return (b, v)
}

def create_gl_objects {
    // sim program: insert the transform-feedback varyings BEFORE linking, so the GPU
    // captures tf_pos / tf_vel / tf_life interleaved (matching the Particle layout).
    let vs = create_shader(sim_src, GL_VERTEX_SHADER)
    let fs = create_shader(sim_dummy_src, GL_FRAGMENT_SHADER)
    sim_prog = create_shader_program(vs, fs)
    var tf_names <- ["tf_pos", "tf_vel", "tf_life"]
    glTransformFeedbackVaryings(sim_prog, 3, unsafe(addr(tf_names[0])), GL_INTERLEAVED_ATTRIBS)
    if (!link_shader(sim_prog)) {
        panic("sim program link failed")
    }
    delete tf_names
    glDeleteShader(vs)
    glDeleteShader(fs)

    render_prog = create_shader_program(render_vs_src, render_fs_src)

    var seed <- gen_initial_particles(N_PARTICLES)
    let a = make_state_buffer(seed)
    buf[0] = a._0
    vao[0] = a._1
    let b = make_state_buffer(seed)
    buf[1] = b._0
    vao[1] = b._1
    delete seed
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 07 particle swarm", null, null)
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
    let nxt = 1 - cur
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let h = max(display_h, 1)
    let aspect = float(display_w) / float(h)

    // ===== Pass 1: simulate -- a vertex shader run as a compute step, captured by TF =====
    glEnable(GL_RASTERIZER_DISCARD)
    glUseProgram(sim_prog)
    u_dt = 1.0 / 60.0
    sim_vs_bind_uniform(sim_prog)
    glBindVertexArray(vao[cur])
    // WebGL2 forbids a buffer bound to TRANSFORM_FEEDBACK_BUFFER from also being bound to
    // GL_ARRAY_BUFFER -- clear the generic binding (setup left it on a state buffer) first.
    glBindBuffer(GL_ARRAY_BUFFER, 0u)
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0u, buf[nxt])
    glBeginTransformFeedback(GL_POINTS)
    glDrawArrays(GL_POINTS, 0, N_PARTICLES)
    glEndTransformFeedback()
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0u, 0u)
    glDisable(GL_RASTERIZER_DISCARD)

    // ===== Pass 2: render the freshly-written buffer as additive point sprites =====
    let cam_angle = t * 0.3
    let cam_r = 3.0
    let cam_pos = float3(cam_r * cos(cam_angle), sin(cam_angle * 2.0) * 0.5, cam_r * sin(cam_angle))
    u_view = look_at_rh(cam_pos, float3(0, 0, 0), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(60.0 * PI / 180.0, aspect, 0.1, 30.0)

    glViewport(0, 0, display_w, display_h)
    glClearColor(0.02, 0.02, 0.05, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    // additive glow: no depth test (every splat adds), premultiplied colour + ONE/ONE blend
    glDisable(GL_DEPTH_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(uint(GL_ONE), uint(GL_ONE))
    glUseProgram(render_prog)
    render_vs_bind_uniform(render_prog)
    render_fs_bind_uniform(render_prog)
    glBindVertexArray(vao[nxt])
    glDrawArrays(GL_POINTS, 0, N_PARTICLES)
    glDisable(GL_BLEND)

    cur = nxt
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
