options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 06 - The Skybox (OpenGL / WebGL2 tutorial)
//
// The first cubemap rung. A procedural sky surrounds the camera, and a mirror
// sphere on a checkered floor reflects it -- so the same `samplerCube` is sampled
// two ways: as a background (the skybox) and as an environment map (the reflection).
// A faithful port of the dasVulkan skybox rung, same procedural sky + reflective
// sphere + floor.
//
// Two new ideas after 04's single 2D texture:
//
//   1. The CUBEMAP. `samplerCube` is sampled by a 3-vector *direction* instead of a
//      2D UV. The skybox is a unit cube centred on the camera; each vertex position
//      doubles as the world-space sampling direction, so the fragment shader samples
//      the sky with `texture(u_sky, normalize(dir))` and never needs a UV. The mirror
//      sphere samples the same cubemap along the reflected view vector. Six 256x256
//      faces are generated on the CPU and uploaded into one `GL_TEXTURE_CUBE_MAP`.
//
//   2. The DEPTH=1.0 TRICK. After the projection multiply the skybox vertex shader
//      snaps `clip.z = clip.w`, so the post-divide NDC z is exactly 1.0 -- every
//      skybox fragment lives at the far plane. Drawn last, with the depth test set to
//      LESS_OR_EQUAL and depth writes off, the skybox fills only the pixels the
//      foreground (sphere + floor) did not already cover. One background draw, no
//      overdraw of what's in front of it.
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web the
// wasm run path drives init/update/shutdown once per browser frame.

// ===== shared shader globals =====
// skybox draws with the rotation-only view (so the cube stays glued to the camera);
// the sphere + floor draw with the full view (they live in the world).
var @uniform u_view_rot : float4x4
var @uniform u_view_full : float4x4
var @uniform u_proj : float4x4
var @uniform u_cam_pos : float3
var @uniform u_material : float4                       // rgb = metal tint, a = reflectivity
var @uniform u_sky : samplerCube

// vertex inputs: pos is shared (location 0); the scene shaders also read a normal
var @in @location = 0 a_pos : float3
var @in @location = 1 a_normal : float3
// varyings: the skybox passes the cube position as a direction; the scene passes
// world position + normal for the reflection
var @inout v_dir : float3
var @inout v_wpos : float3
var @inout v_wnormal : float3
var @out f_FragColor : float4

// ===== skybox shaders =====

[vertex_program]
def skybox_vs {
    // the cube position is the sampling direction; the rotation-only view keeps the
    // cube centred on the camera, so the position is the world direction to the vertex
    v_dir = a_pos
    let clip = u_proj * u_view_rot * float4(a_pos, 1.0)
    // snap z to w so post-divide NDC z = 1.0 -- every skybox fragment is at the far plane
    gl_Position = float4(clip.x, clip.y, clip.w, clip.w)
}

[fragment_program]
def skybox_fs {
    let col = texture(u_sky, normalize(v_dir)).xyz
    f_FragColor = float4(col, 1.0)
}

// ===== scene (sphere + floor) shaders =====
// one shared vertex shader; the fragment shaders diverge (mirror metal vs checker)

[vertex_program]
def scene_vs {
    v_wpos = a_pos
    v_wnormal = a_normal
    gl_Position = u_proj * u_view_full * float4(a_pos, 1.0)
}

// GLSL-style reflect(I, N) = I - 2*dot(N,I)*N, I the incident (eye->surface) direction.
def reflect_dir(i, n : float3) : float3 {
    return i - n * (2.0 * dot(n, i))
}

[fragment_program]
def sphere_fs {
    let n = normalize(v_wnormal)
    let v = normalize(u_cam_pos - v_wpos)                    // surface -> eye
    let ndotv = max(dot(n, v), 0.0)
    let env = texture(u_sky, reflect_dir(-v, n)).xyz         // reflect the eye ray, sample the sky
    let tint = u_material.xyz
    let f0 = tint * u_material.w
    // Schlick metal Fresnel: tinted at normal incidence, ramping to white at grazing
    let fres = pow(1.0 - ndotv, 5.0)
    let reflectance = lerp(f0, float3(1.0, 1.0, 1.0), float3(fres, fres, fres))
    f_FragColor = float4(env * reflectance, 1.0)
}

[fragment_program]
def floor_fs {
    let n = normalize(v_wnormal)
    let gx = floor(v_wpos.x * 0.5 + 100.0)
    let gz = floor(v_wpos.z * 0.5 + 100.0)
    // checkerboard parity (gx+gz) mod 2 via floor -- no mod builtin needed
    let cell = gx + gz
    let parity = cell - floor(cell * 0.5) * 2.0
    let checker = parity < 0.5 ? 0.85 : 0.28
    let base = float3(checker, checker, checker) * float3(0.72, 0.74, 0.80)
    let v = normalize(u_cam_pos - v_wpos)
    let env = texture(u_sky, reflect_dir(-v, n)).xyz
    let fres = pow(1.0 - max(dot(n, v), 0.0), 4.0) * 0.4     // faint sky reflection on the polish
    let col = lerp(base, env, float3(fres, fres, fres))
    f_FragColor = float4(col, 1.0)
}

// ===== GL objects =====

var prog_sky : uint
var prog_sphere : uint
var prog_floor : uint
var sky_vao, sky_vbo, sky_ebo : uint
var sphere_vao, sphere_vbo, sphere_ebo : uint
var floor_vao, floor_vbo, floor_ebo : uint
var sky_tex : uint
var sphere_index_count : int
var window : GLFWwindow?
var time : float = 0.0

let FACE_DIM = 256
let N_FACES = 6
let SPHERE_LAT = 48
let SPHERE_LON = 64
let SPHERE_CY = 1.05                                    // sphere centre height (just above the floor)
let FLOOR_HALF = 9.0

[vertex_buffer]
struct SkyVertex {
    pos : float3
}

[vertex_buffer]
struct SceneVertex {
    pos : float3
    normal : float3
}

// 8 corners of a unit cube; the position is the sampling direction. Rendered with
// culling off, so winding does not matter -- the camera sits at the cube centre.
let cube_corners = [SkyVertex(
    pos=float3(-1, -1, -1)), SkyVertex(
    pos=float3(1, -1, -1)), SkyVertex(
    pos=float3(1, 1, -1)), SkyVertex(
    pos=float3(-1, 1, -1)), SkyVertex(
    pos=float3(-1, -1, 1)), SkyVertex(
    pos=float3(1, -1, 1)), SkyVertex(
    pos=float3(1, 1, 1)), SkyVertex(
    pos=float3(-1, 1, 1)
)];

let cube_indices = fixed_array<int>(
    1, 5, 6, 1, 6, 2,            // +X
    0, 3, 7, 0, 7, 4,            // -X
    2, 6, 7, 2, 7, 3,            // +Y
    0, 4, 5, 0, 5, 1,            // -Y
    4, 7, 6, 4, 6, 5,            // +Z
    0, 1, 2, 0, 2, 3)           // -Z

// ===== procedural sky =====
// A continuous function of direction -- a two-tone vertical gradient (warm horizon to
// zenith blue) with a ground falloff, plus a sun disc + glow. Each cube face just
// rasterises this same function over its own outgoing-direction patch, so the seams
// between faces are invisible.

let SUN_DIR : float3 = normalize(float3(0.45, 0.55, 0.85))

def mix3(a, b : float3; t : float) : float3 {
    return a + (b - a) * t
}

def sample_sky(dir : float3) : float3 {
    let altitude = clamp(dir.y, -1.0, 1.0)
    let elevation = clamp(altitude * 0.5 + 0.5, 0.0, 1.0)
    let zenith = float3(0.18, 0.32, 0.70)
    let horizon = float3(0.88, 0.62, 0.45)
    var col = mix3(horizon, zenith, elevation)
    if (altitude < 0.0) {
        let ground = float3(0.10, 0.07, 0.05)
        let g = clamp(-altitude * 1.6, 0.0, 1.0)
        col = mix3(col, ground, g)
    }
    let sun_dot = clamp(dot(dir, SUN_DIR), 0.0, 1.0)
    let glow = pow(sun_dot, 32.0) * 0.9
    col += float3(1.0, 0.85, 0.65) * glow
    if (sun_dot > 0.998) {
        col = float3(1.0, 0.95, 0.85)                  // the sun disc itself
    }
    return clamp(col, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0))
}

// Map a face index + (x,y) pixel to a unit direction. This matches the GL cubemap
// sampling convention (the major-axis table), so the sky the shader samples is the
// sky we baked. Faces are ordered +X, -X, +Y, -Y, +Z, -Z to match the
// GL_TEXTURE_CUBE_MAP_POSITIVE_X .. NEGATIVE_Z target order.
def face_direction(face, x, y, dim : int) : float3 {
    let s = (float(x) + 0.5) / float(dim) * 2.0 - 1.0
    let t = (float(y) + 0.5) / float(dim) * 2.0 - 1.0
    var d : float3
    if (face == 0) {                                   // +X
        d = float3(1.0, -t, -s)
    } elif (face == 1) {                               // -X
        d = float3(-1.0, -t, s)
    } elif (face == 2) {                               // +Y
        d = float3(s, 1.0, t)
    } elif (face == 3) {                               // -Y
        d = float3(s, -1.0, -t)
    } elif (face == 4) {                               // +Z
        d = float3(s, -t, 1.0)
    } else {                                           // -Z
        d = float3(-s, -t, -1.0)
    }
    return normalize(d)
}

// Build one face (dim*dim RGBA8) of the procedural sky.
def gen_sky_face(face, dim : int) : array<uint8> {
    var pixels : array<uint8>
    pixels |> resize(dim * dim * 4)
    for (y in range(dim)) {
        for (x in range(dim)) {
            let col = sample_sky(face_direction(face, x, y, dim))
            let p = (y * dim + x) * 4
            pixels[p + 0] = uint8(clamp(col.x * 255.0, 0.0, 255.0))
            pixels[p + 1] = uint8(clamp(col.y * 255.0, 0.0, 255.0))
            pixels[p + 2] = uint8(clamp(col.z * 255.0, 0.0, 255.0))
            pixels[p + 3] = 255u8
        }
    }
    return <- pixels
}

// Generate the six faces and upload them into one cubemap. Each face goes to its own
// GL_TEXTURE_CUBE_MAP_POSITIVE_X + face target; CLAMP_TO_EDGE on all three axes keeps
// the face edges from bleeding across the seams.
def create_skybox_cubemap(dim : int) : uint {
    var tex : uint
    glGenTextures(1, safe_addr(tex))
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex)
    for (face in range(N_FACES)) {
        var pixels <- gen_sky_face(face, dim)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + uint(face), 0, int(GL_RGBA), dim, dim, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, unsafe(addr(pixels[0])))
        delete pixels
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    return tex
}

// ===== sphere + floor geometry =====

// UV sphere centred at (0, SPHERE_CY, 0); each vertex is position + outward unit normal.
def gen_sphere(var verts : array<SceneVertex>; var indices : array<int>) {
    verts |> reserve((SPHERE_LAT + 1) * (SPHERE_LON + 1))
    indices |> reserve(SPHERE_LAT * SPHERE_LON * 6)
    for (i in range(SPHERE_LAT + 1)) {
        let theta = float(i) / float(SPHERE_LAT) * PI
        let st = sin(theta)
        let ct = cos(theta)
        for (j in range(SPHERE_LON + 1)) {
            let phi = float(j) / float(SPHERE_LON) * 2.0 * PI
            let nrm = float3(st * cos(phi), ct, st * sin(phi))
            verts |> push(SceneVertex(pos = nrm + float3(0.0, SPHERE_CY, 0.0), normal = nrm))
        }
    }
    let row = SPHERE_LON + 1
    for (i in range(SPHERE_LAT)) {
        for (j in range(SPHERE_LON)) {
            let a = i * row + j
            let b = a + row
            indices |> push(a); indices |> push(b); indices |> push(a + 1)
            indices |> push(a + 1); indices |> push(b); indices |> push(b + 1)
        }
    }
}

// Large floor quad in the XZ plane at y=0, normal up.
let floor_verts = [SceneVertex(
    pos=float3(-9.0, 0.0, -9.0), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(9.0, 0.0, -9.0), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(9.0, 0.0, 9.0), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(-9.0, 0.0, 9.0), normal=float3(0, 1, 0)
)];

let floor_indices = fixed_array<int>(0, 1, 2, 0, 2, 3)

def create_gl_objects {
    prog_sky = create_shader_program(@@skybox_vs, @@skybox_fs)
    prog_sphere = create_shader_program(@@scene_vs, @@sphere_fs)
    prog_floor = create_shader_program(@@scene_vs, @@floor_fs)

    // skybox cube (positions only)
    glGenVertexArrays(1, safe_addr(sky_vao))
    glBindVertexArray(sky_vao)
    glGenBuffers(1, safe_addr(sky_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, sky_vbo)
    glBufferData(GL_ARRAY_BUFFER, cube_corners, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<SkyVertex>)
    glGenBuffers(1, safe_addr(sky_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_indices, GL_STATIC_DRAW)

    // sphere (pos + normal)
    var sverts : array<SceneVertex>
    var sidx : array<int>
    gen_sphere(sverts, sidx)
    sphere_index_count = length(sidx)
    glGenVertexArrays(1, safe_addr(sphere_vao))
    glBindVertexArray(sphere_vao)
    glGenBuffers(1, safe_addr(sphere_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo)
    glBufferData(GL_ARRAY_BUFFER, sverts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<SceneVertex>)
    glGenBuffers(1, safe_addr(sphere_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sidx, GL_STATIC_DRAW)
    delete sverts
    delete sidx

    // floor quad (pos + normal)
    glGenVertexArrays(1, safe_addr(floor_vao))
    glBindVertexArray(floor_vao)
    glGenBuffers(1, safe_addr(floor_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo)
    glBufferData(GL_ARRAY_BUFFER, floor_verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<SceneVertex>)
    glGenBuffers(1, safe_addr(floor_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices, GL_STATIC_DRAW)

    sky_tex = create_skybox_cubemap(FACE_DIM)
    u_sky := sky_tex
}

// Metals the sphere morphs through: float4(tint.rgb, reflectivity).
let METAL_PRESETS = [
    float4(0.95, 0.96, 0.98, 1.00),   // chrome
    float4(1.00, 0.78, 0.34, 0.95),   // gold
    float4(0.95, 0.55, 0.38, 0.92),   // copper
    float4(0.70, 0.72, 0.76, 0.78)];  // brushed steel

// Lerp through the metal presets, ~3 s each, looping.
def cycle_metal(t : float) : float4 {
    let n = length(METAL_PRESETS)
    let u = t / 3.0
    let idx = int(floor(u)) % n
    let nxt = (idx + 1) % n
    let f = u - floor(u)
    return METAL_PRESETS[idx] + (METAL_PRESETS[nxt] - METAL_PRESETS[idx]) * f
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 06 skybox", null, null)
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

    // camera orbits the sphere, slightly elevated, gently swaying
    let camera_angle = t * 0.4
    let cam_pos = float3(cos(camera_angle) * 4.5, 2.2 + sin(camera_angle * 2.0) * 0.6, sin(camera_angle) * 4.5)
    let target = float3(0.0, 0.9, 0.0)
    u_view_full = look_at_rh(cam_pos, target, float3(0, 1, 0))
    // rotation-only view: eye at the origin so the skybox cube stays centred on the camera
    u_view_rot = look_at_rh(float3(0, 0, 0), normalize(target - cam_pos), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(70.0 * PI / 180.0, aspect, 0.1, 50.0)
    u_cam_pos = cam_pos
    u_material = cycle_metal(t)

    glViewport(0, 0, display_w, display_h)
    glDepthMask(true)                                  // glClear honours the depth mask -- restore before clearing
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)

    // foreground first, writing depth: mirror sphere, then checker floor
    glDepthFunc(GL_LESS)
    glUseProgram(prog_sphere)
    scene_vs_bind_uniform(prog_sphere)
    sphere_fs_bind_uniform(prog_sphere)
    glBindVertexArray(sphere_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_ebo)
    glDrawElements(GL_TRIANGLES, sphere_index_count, GL_UNSIGNED_INT, null)

    glUseProgram(prog_floor)
    scene_vs_bind_uniform(prog_floor)
    floor_fs_bind_uniform(prog_floor)
    glBindVertexArray(floor_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, null)

    // skybox last: z=1.0 + LEQUAL + no depth write fills only the uncovered background
    glDepthFunc(GL_LEQUAL)
    glDepthMask(false)
    glUseProgram(prog_sky)
    skybox_vs_bind_uniform(prog_sky)
    skybox_fs_bind_uniform(prog_sky)
    glBindVertexArray(sky_vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_ebo)
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
