options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/safe_addr

// 03 - Signed-Distance-Field Raymarcher (OpenGL / WebGL2 tutorial)
//
// The dasVulkan rung raymarches the scene in a [compute_shader], one invocation
// per pixel, into a storage image. WebGL2 has no compute, so -- like the
// Mandelbrot rung -- the portable GL idiom is a FULLSCREEN FRAGMENT shader: a
// clip-space quad, and the whole raymarch runs once per fragment. The scene is
// pure math: a smooth-blended sphere + orbiting torus floating above a checkered
// plane, shaded with key + ambient + soft shadow + rim + atmospheric fog. No
// vertex buffer beyond the quad, no triangles, no textures.
//
// The colour scheme and shading are a faithful port of the Vulkan rung -- only
// the vehicle (compute -> fragment) and the pixel->ray mapping change. The
// raymarch exercises a wide intrinsic surface (length / normalize / cross / dot /
// floor / pow / exp / cos / sin / clamp / abs / min / max / mix), user-defined
// helper functions, swizzles, and `for`/`break` control flow -- all lowered to
// GLSL by dasGlsl.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web
// the wasm run path drives init/update/shutdown once per browser frame.

var @in @location v_position : float2
var @uniform u_time : float
var @uniform u_aspect : float
var @inout f_uv : float2
var @out f_FragColor : float4

let MAX_STEPS = 96          // raymarch iteration budget per pixel
let MAX_DIST = 60.0         // far plane: distance at which we declare a miss
let HIT_EPS = 0.001         // distance threshold that counts as a surface hit
let SHADOW_K = 32.0         // soft-shadow penumbra sharpness (IQ recipe)

// IQ's polynomial smooth union: blends two SDFs over a soft radius k. Reduces to
// min(a,b) when the surfaces are far apart, but curves the meeting band where
// they overlap -- the visual signature of distance-field rendering.
def private smin(a, b, k : float) : float {
    let h = max(k - abs(a - b), 0.0) / k
    return min(a, b) - h * h * h * k * (1.0 / 6.0)
}

// SDF primitives. Each returns the signed distance from p to the surface.
def private sd_sphere(p : float3; r : float) : float {
    return length(p) - r
}

def private sd_torus(p : float3; major, minor : float) : float {
    let q = float2(length(p.xz) - major, p.y)
    return length(q) - minor
}

def private sd_plane(p : float3; h : float) : float {
    return p.y - h
}

// 2D rotation around the origin; used to spin the torus's local frame about Y.
def private rot2(p : float2; ang : float) : float2 {
    let c = cos(ang)
    let s = sin(ang)
    return float2(c * p.x - s * p.y, s * p.x + c * p.y)
}

// The scene SDF. Returns (distance_to_nearest_surface, material_id) so the shader
// can branch on the material at the hit. material 0 = blob (sphere smin torus),
// material 1 = ground.
def private map(p : float3; t : float) : float2 {
    let d_plane = sd_plane(p, -0.5)
    let d_sphere = sd_sphere(p - float3(0.0, 0.2 * sin(t * 1.5), 0.0), 0.55)
    // torus orbits the sphere by rotating its local xz frame with time
    let xz = rot2(float2(p.x, p.z), t * 0.8)
    let pt = float3(xz.x, p.y, xz.y)
    let d_torus = sd_torus(pt - float3(1.0, 0.0, 0.0), 0.32, 0.12)
    let d_blob = smin(d_sphere, d_torus, 0.3)
    if (d_plane < d_blob) {
        return float2(d_plane, 1.0)
    }
    return float2(d_blob, 0.0)
}

// Scalar overload for the shadow march (no material needed).
def private map_d(p : float3; t : float) : float {
    return map(p, t).x
}

// Forward-difference normal at p: 4 SDF taps (1 baseline + 3 axis-displaced),
// normalized. Cheaper than the 6-tap central difference; the slight +eps bias is
// invisible at this eps and distance threshold.
def private get_normal(p : float3; t : float) : float3 {
    let eps = 0.001
    let d = map_d(p, t)
    let nx = map_d(p + float3(eps, 0.0, 0.0), t) - d
    let ny = map_d(p + float3(0.0, eps, 0.0), t) - d
    let nz = map_d(p + float3(0.0, 0.0, eps), t) - d
    return normalize(float3(nx, ny, nz))
}

// March from ro toward rd. Returns (t_hit, material). t_hit >= MAX_DIST means a
// miss (sky). Three exits: (1) HIT_EPS hit -> set mat + break, (2) far-plane
// escape -> break, (3) step-budget exhausted -> force a miss (else dist could
// land just under MAX_DIST and the caller would shade garbage at material 0).
def private march(ro, rd : float3; t : float) : float2 {
    var dist = 0.0
    var mat = 0.0
    var hit = 0
    for (_i in range(MAX_STEPS)) {
        let p = ro + rd * dist
        let m = map(p, t)
        if (m.x < HIT_EPS) {
            mat = m.y
            hit = 1
            break
        }
        dist += m.x
        if (dist > MAX_DIST) {
            break
        }
    }
    if (hit == 0) {
        dist = MAX_DIST + 1.0
    }
    return float2(dist, mat)
}

// Soft shadow via IQ's penumbra estimate: track min(h/dist) along the shadow ray;
// small ratios mean the ray grazed close to a surface (deep penumbra).
def private soft_shadow(ro, rd : float3; t : float) : float {
    var res = 1.0
    var dist = 0.05       // small offset to dodge self-shadowing
    for (_i in range(48)) {
        let p = ro + rd * dist
        let h = map_d(p, t)
        if (h < 0.001) {
            res = 0.0
            break
        }
        res = min(res, SHADOW_K * h / dist)
        dist += clamp(h, 0.02, 1.0)
        if (dist > 10.0) {
            break
        }
    }
    return clamp(res, 0.0, 1.0)
}

// Vertical sky gradient (warm horizon -> cool zenith) with a tiny solar disc.
def private sky_color(rd, sun_dir : float3) : float3 {
    let h = clamp(rd.y * 0.5 + 0.5, 0.0, 1.0)
    let horizon = float3(0.94, 0.78, 0.62)    // warm peach
    let zenith = float3(0.45, 0.62, 0.85)     // soft blue
    var col = lerp(horizon, zenith, float3(h, h, h))
    let sun = max(dot(rd, sun_dir), 0.0)
    col += float3(1.0, 0.85, 0.6) * pow(sun, 64.0)
    return col
}

// Checkered ground albedo: parity of (floor(x) + floor(z)) picks one of two tones.
def private ground_color(p : float3) : float3 {
    let cx = floor(p.x)
    let cz = floor(p.z)
    let sum = cx + cz
    let parity = sum - 2.0 * floor(0.5 * sum)        // (cx + cz) mod 2 as a float
    let a = float3(0.95, 0.92, 0.85)
    let b = float3(0.30, 0.34, 0.38)
    return lerp(a, b, float3(parity, parity, parity))
}

// Blob albedo: IQ-style cosine palette pulsing with time, lifted toward the top.
def private blob_color(p : float3; t : float) : float3 {
    let phase = t * 0.6
    let r = 0.6 + 0.4 * cos(phase + 0.0)
    let g = 0.45 + 0.4 * cos(phase + 0.8)
    let b = 0.4 + 0.4 * cos(phase + 1.6)
    let lift = 0.85 + 0.15 * clamp(p.y + 0.5, 0.0, 1.0)
    return float3(r, g, b) * lift
}

// Reinhard tone map. Compresses HDR overshoot (rim glow + sun disc) into [0,1].
def private tonemap(c : float3) : float3 {
    let one = float3(1.0, 1.0, 1.0)
    return c / (one + c)
}

[vertex_program]
def vs_main {
    f_uv = v_position
    gl_Position = float4(v_position, 0.0, 1.0)
}

[fragment_program]
def fs_main {
    let t = u_time
    // screen-normalized coords: x widened by aspect, y in [-1,1] (+y up in clip)
    let px = f_uv.x * u_aspect
    let py = f_uv.y

    // camera orbits the origin at fixed radius with a gentle vertical sway
    let cam_ang = t * 0.4
    let cam_dist = 3.0
    let cam_y = 0.8 + 0.2 * sin(t * 0.3)
    let ro = float3(cos(cam_ang) * cam_dist, cam_y, sin(cam_ang) * cam_dist)
    let target = float3(0.0, 0.0, 0.0)

    let fwd = normalize(target - ro)
    let right = normalize(cross(float3(0.0, 1.0, 0.0), fwd))
    let up = cross(fwd, right)

    let focal = 1.6       // pinhole focal length; larger = narrower FOV
    let rd = normalize(fwd * focal + right * px + up * py)

    let sun_dir = normalize(float3(-0.4, 0.8, -0.5))

    let hit = march(ro, rd, t)
    var col = float3(0.0, 0.0, 0.0)
    if (hit.x >= MAX_DIST) {
        col = sky_color(rd, sun_dir)
    } else {
        let p = ro + rd * hit.x
        let n = get_normal(p, t)
        var albedo = float3(0.5, 0.5, 0.5)
        if (hit.y > 0.5) {
            albedo = ground_color(p)
        } else {
            albedo = blob_color(p, t)
        }
        let key = max(dot(n, sun_dir), 0.0)
        let sh = soft_shadow(p + n * 0.001, sun_dir, t)
        let v = normalize(ro - p)
        let rim = pow(1.0 - max(dot(n, v), 0.0), 3.0)
        let amb = 0.25 + 0.25 * (n.y * 0.5 + 0.5)        // hemisphere ambient
        let lit = albedo * (amb + key * sh * 0.85) + float3(1.0, 0.9, 0.75) * rim * 0.2
        // atmospheric fog: scene fades into the sky color with distance
        let f = clamp(1.0 - exp(-0.02 * hit.x * hit.x), 0.0, 1.0)
        col = lerp(lit, sky_color(rd, sun_dir), float3(f, f, f))
    }

    col = tonemap(col)
    f_FragColor = float4(col, 1.0)
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
    window = glfwCreateWindow(640, 480, "OpenGL - 03 sdf raymarcher", null, null)
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
