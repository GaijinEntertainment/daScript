options gen2
require glfw/glfw_boost
require opengl/opengl_boost
require math
require daslib/math_boost
require daslib/safe_addr

// 08 - The Shadow Map (OpenGL / WebGL2 tutorial)
//
// The canonical two-pass shadow-map dance, faithfully ported from the dasVulkan
// shadow rung. A rotating cube hovers over a checkered floor while a directional
// light orbits the scene -- and the cube casts a real, soft-edged shadow that
// sweeps across the floor as the sun moves.
//
// The whole trick is to render the scene TWICE:
//
//   1. The SHADOW PASS renders the scene's depth from the LIGHT'S point of view
//      into an offscreen depth texture (no colour, no lighting -- just how far each
//      caster is from the light). After this pass the shadow map holds, for every
//      light-space (x, y), the distance to the closest caster.
//
//   2. The MAIN PASS renders the scene from the camera, and for each fragment asks
//      the shadow map "is anything closer to the light here than I am?" -- if yes,
//      the fragment is in shadow. The lookup is a `sampler2DShadow` +
//      `textureCompare(s, uv, ref)`: a hardware depth comparison that returns a
//      [0, 1] result (1 = lit, 0 = shadowed), 2x2-filtered by the GPU for free.
//
// Three rails make it work and read cleanly:
//
//   * ONE depth texture, two roles -- depth attachment in pass 1, `sampler2DShadow`
//     in pass 2. It is created with `GL_TEXTURE_COMPARE_MODE = COMPARE_REF_TO_TEXTURE`
//     so `texture()` against it performs the depth compare instead of a plain read.
//   * A DEPTH-ONLY FRAMEBUFFER -- a framebuffer object with a depth attachment and
//     the colour draw buffer set to `GL_NONE`. The shadow pass writes no colour.
//   * DEPTH BIAS -- `glPolygonOffset` during the shadow pass plus a small N.L-scaled
//     fragment bias, or every grazing surface self-shadows ("shadow acne").
//
// One homogeneous program: `main()` is the desktop windowed viewer; on the web the
// wasm run path drives init/update/shutdown once per browser frame.

// ===== shared shader globals =====
// Both passes' vertex shaders read u_light_vp + u_model; the main pass adds the
// camera matrices, the light direction, the camera position, the shadow sampler,
// and a per-draw u_is_floor flag (0 = cube, 1 = floor) that selects the material.
var @uniform u_light_vp : float4x4
var @uniform u_model : float4x4
var @uniform u_view : float4x4
var @uniform u_proj : float4x4
var @uniform u_light_dir : float3
var @uniform u_cam_pos : float3
var @uniform u_is_floor : float
var @uniform shadow_map : sampler2DShadow

var @in @location = 0 a_pos : float3
var @in @location = 1 a_normal : float3
// varyings: world-space position + normal for lighting, plus the position projected
// into light space (pre-divide) for the shadow lookup
var @inout v_world_pos : float3
var @inout v_world_normal : float3
var @inout v_light_space : float4
var @out f_FragColor : float4

// ===== shadow pass =====
// Render every caster's depth from the light's view. `gl_Position = light_vp * model
// * pos` is all the geometry work; the fragment shader writes nothing useful (the
// framebuffer has no colour attachment), it just keeps the program complete.

[vertex_program]
def shadow_vs {
    gl_Position = u_light_vp * u_model * float4(a_pos, 1.0)
}

[fragment_program]
def shadow_fs {
    // depth-only framebuffer: this colour is discarded (draw buffer is GL_NONE)
    f_FragColor = float4(1.0, 1.0, 1.0, 1.0)
}

// ===== main pass =====

[vertex_program]
def main_vs {
    let world = u_model * float4(a_pos, 1.0)
    v_world_pos = world.xyz
    // model is rotation + uniform scale only, so a (normal, 0) multiply is correct
    v_world_normal = (u_model * float4(a_normal, 0.0)).xyz
    v_light_space = u_light_vp * world
    gl_Position = u_proj * u_view * world
}

let SHADOW_DIM_F = 1024.0

// 3x3 PCF: average 9 taps over a one-texel radius. Each `textureCompare` is already
// a hardware 2x2 depth compare, so the effective footprint is ~36 samples -- soft
// shadow edges instead of a hard binary line, at low cost.
def pcf_shadow(uv : float2; ref : float) : float {
    let texel = 1.0 / SHADOW_DIM_F
    var sum = 0.0
    for (j in range(-1, 2)) {
        for (i in range(-1, 2)) {
            let off = float2(i, j) * texel
            sum += textureCompare(shadow_map, uv + off, ref)
        }
    }
    return sum * (1.0 / 9.0)
}

[fragment_program]
def main_fs {
    let n = normalize(v_world_normal)
    let l = normalize(u_light_dir)
    let ndotl = max(dot(n, l), 0.0)
    let v = normalize(u_cam_pos - v_world_pos)

    // Light-space NDC = clip.xyz / clip.w. OpenGL NDC is [-1, 1] on all three axes,
    // so remap xy to [0, 1] for the shadow-map UV and z to [0, 1] for the depth
    // reference (matching the [0, 1] depth the shadow pass stored).
    let w = max(v_light_space.w, 0.0001)
    let ndc = v_light_space.xyz / float3(w, w, w)
    let shadow_uv = ndc.xy * 0.5 + float2(0.5, 0.5)
    let ref_depth = ndc.z * 0.5 + 0.5

    // N.L-scaled bias on top of the pipeline's polygon offset: grazing faces push the
    // reference further from the light so the compare stops mis-firing on lit
    // surfaces, with a floor so even head-on faces get a touch of bias.
    let bias = max(0.0025 * (1.0 - ndotl), 0.0008)
    var lit = pcf_shadow(shadow_uv, ref_depth - bias)
    // Faces turned away from the light are in shadow regardless of the depth test;
    // skipping the lookup avoids bias artifacts at N.L ~ 0 where numerical noise
    // flips the comparison.
    if (ndotl < 0.05) {
        lit = 0.0
    }

    // Hemisphere ambient: warm-ish sky overhead blended to a dim ground bounce, by n.y.
    let amb_t = n.y * 0.5 + 0.5
    let ambient = lerp(float3(0.10, 0.09, 0.08), float3(0.30, 0.34, 0.42), float3(amb_t, amb_t, amb_t))
    let sun = float3(1.30, 1.15, 0.90)

    // Albedo: a checkerboard on the floor (parity of the integer cell), a warm solid
    // on the cube. u_is_floor is set per draw by the host.
    var albedo : float3
    var spec_strength = 0.5
    if (u_is_floor > 0.5) {
        let gx = floor(v_world_pos.x * 0.5 + 100.0)
        let gz = floor(v_world_pos.z * 0.5 + 100.0)
        let cell = gx + gz
        let parity = cell - floor(cell * 0.5) * 2.0
        let checker = parity < 0.5 ? 0.85 : 0.30
        albedo = float3(checker, checker, checker) * float3(0.80, 0.82, 0.88)
        spec_strength = 0.15
    } else {
        albedo = float3(0.85, 0.45, 0.30)
    }

    // Blinn-Phong specular, killed in shadow along with the diffuse.
    let hvec = normalize(l + v)
    let spec = pow(max(dot(n, hvec), 0.0), 48.0) * lit * spec_strength
    let diffuse = sun * ndotl * lit
    let col = ambient * albedo + albedo * diffuse + float3(spec, spec, spec)
    f_FragColor = float4(col, 1.0)
}

// ===== GL objects =====

var prog_shadow : uint
var prog_main : uint
var cube_vao, cube_vbo, cube_ebo : uint
var floor_vao, floor_vbo, floor_ebo : uint
var shadow_fbo, shadow_tex : uint
var window : GLFWwindow?
var time : float = 0.0

let SHADOW_DIM = 1024
let FLOOR_HALF = 5.0
let CUBE_HOVER = 1.5

[vertex_buffer]
struct SceneVertex {
    pos : float3
    normal : float3
}

// Unit cube, 24 split vertices (one face-normal per corner) -- the same winding as
// tutorial 04, proven correct under GL_BACK culling + perspective_rh_opengl. The
// model matrix scales + hovers + spins it; here it spans [-1, 1] in object space.
let cube_verts = [SceneVertex(
    pos=float3(1, 1, 1), normal=float3(0, 0, 1)), SceneVertex(
    pos=float3(-1, 1, 1), normal=float3(0, 0, 1)), SceneVertex(
    pos=float3(-1, -1, 1), normal=float3(0, 0, 1)), SceneVertex(
    pos=float3(1, -1, 1), normal=float3(0, 0, 1)), SceneVertex(
    pos=float3(1, 1, 1), normal=float3(1, 0, 0)), SceneVertex(
    pos=float3(1, -1, 1), normal=float3(1, 0, 0)), SceneVertex(
    pos=float3(1, -1, -1), normal=float3(1, 0, 0)), SceneVertex(
    pos=float3(1, 1, -1), normal=float3(1, 0, 0)), SceneVertex(
    pos=float3(1, 1, 1), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(1, 1, -1), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(-1, 1, -1), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(-1, 1, 1), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(-1, 1, 1), normal=float3(-1, 0, 0)), SceneVertex(
    pos=float3(-1, 1, -1), normal=float3(-1, 0, 0)), SceneVertex(
    pos=float3(-1, -1, -1), normal=float3(-1, 0, 0)), SceneVertex(
    pos=float3(-1, -1, 1), normal=float3(-1, 0, 0)), SceneVertex(
    pos=float3(-1, -1, -1), normal=float3(0, -1, 0)), SceneVertex(
    pos=float3(1, -1, -1), normal=float3(0, -1, 0)), SceneVertex(
    pos=float3(1, -1, 1), normal=float3(0, -1, 0)), SceneVertex(
    pos=float3(-1, -1, 1), normal=float3(0, -1, 0)), SceneVertex(
    pos=float3(1, -1, -1), normal=float3(0, 0, -1)), SceneVertex(
    pos=float3(-1, -1, -1), normal=float3(0, 0, -1)), SceneVertex(
    pos=float3(-1, 1, -1), normal=float3(0, 0, -1)), SceneVertex(
    pos=float3(1, 1, -1), normal=float3(0, 0, -1)
)];

let cube_indices = fixed_array<int>(
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20)

// Floor quad in the XZ plane at y=0, normal up, sized to FLOOR_HALF -- big enough
// that the cube's shadow lands inside it. Drawn with an identity model matrix.
let floor_verts = [SceneVertex(
    pos=float3(-5.0, 0.0, -5.0), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(5.0, 0.0, -5.0), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(5.0, 0.0, 5.0), normal=float3(0, 1, 0)), SceneVertex(
    pos=float3(-5.0, 0.0, 5.0), normal=float3(0, 1, 0)
)];

let floor_indices = fixed_array<int>(0, 1, 2, 0, 2, 3)

def identity_m4 : float4x4 {
    var m : float4x4
    m[0] = float4(1, 0, 0, 0)
    m[1] = float4(0, 1, 0, 0)
    m[2] = float4(0, 0, 1, 0)
    m[3] = float4(0, 0, 0, 1)
    return m
}


// Create the offscreen depth texture + framebuffer that the shadow pass writes and
// the main pass samples. The depth texture's compare mode is what makes a
// `sampler2DShadow` lookup return the hardware compare result.
def create_shadow_target {
    glGenTextures(1, safe_addr(shadow_tex))
    glBindTexture(GL_TEXTURE_2D, shadow_tex)
    glTexImage2D(GL_TEXTURE_2D, 0, int(GL_DEPTH_COMPONENT24), SHADOW_DIM, SHADOW_DIM, 0,
        GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, null)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    // hardware depth comparison: texture(sampler2DShadow, vec3(uv, ref)) returns
    // (ref <= stored_depth) ? 1 : 0, LINEAR-filtered into a [0,1] PCF result.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL)

    glGenFramebuffers(1, safe_addr(shadow_fbo))
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0)
    // depth-only: no colour buffer to draw to or read from
    var none_buf = uint(GL_NONE)
    glDrawBuffers(1, safe_addr(none_buf))
    glReadBuffer(uint(GL_NONE))
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        panic("shadow framebuffer incomplete")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
}

def create_gl_objects {
    prog_shadow = create_shader_program(@@shadow_vs, @@shadow_fs)
    prog_main = create_shader_program(@@main_vs, @@main_fs)

    // cube mesh (pos + normal)
    glGenVertexArrays(1, safe_addr(cube_vao))
    glBindVertexArray(cube_vao)
    glGenBuffers(1, safe_addr(cube_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo)
    glBufferData(GL_ARRAY_BUFFER, cube_verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<SceneVertex>)
    glGenBuffers(1, safe_addr(cube_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_indices, GL_STATIC_DRAW)

    // floor mesh (pos + normal)
    glGenVertexArrays(1, safe_addr(floor_vao))
    glBindVertexArray(floor_vao)
    glGenBuffers(1, safe_addr(floor_vbo))
    glBindBuffer(GL_ARRAY_BUFFER, floor_vbo)
    glBufferData(GL_ARRAY_BUFFER, floor_verts, GL_STATIC_DRAW)
    bind_vertex_buffer(null, type<SceneVertex>)
    glGenBuffers(1, safe_addr(floor_ebo))
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_ebo)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floor_indices, GL_STATIC_DRAW)

    create_shadow_target()
}

[export]
def init {
    if (glfwInit() == 0) {
        panic("can't init glfw")
    }
    glfwInitOpenGL(3, 3)
    window = glfwCreateWindow(640, 480, "OpenGL - 08 shadow map", null, null)
    if (window == null) {
        panic("can't create window")
    }
    glfwMakeContextCurrent(window)
    create_gl_objects()
}

// The cube's model matrix: hover + spin about a tilted axis, scaled to a tidy box.
def cube_model(t : float) : float4x4 {
    let rot = quat_from_unit_vec_ang(normalize(float3(0.3, 1.0, 0.2)), t * 0.7)
    return compose(float3(0.0, CUBE_HOVER, 0.0), rot, float3(0.55))
}

def draw_mesh(vao, ebo : uint; count : int) {
    glBindVertexArray(vao)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo)
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, null)
}

[export]
def update : bool {
    time += 1.0 / 60.0
    let t = time
    var display_w, display_h : int
    glfwGetFramebufferSize(window, display_w, display_h)
    let h = max(display_h, 1)
    let aspect = float(display_w) / float(h)

    // Directional light orbits in XZ, tilted up, so the cube's shadow sweeps the floor.
    let sun_yaw = t * 0.6
    let light_dir = normalize(float3(cos(sun_yaw), 1.4, sin(sun_yaw)))
    let light_eye = light_dir * 8.0
    let light_view = look_at_rh(light_eye, float3(0.0, 0.0, 0.0), float3(0, 1, 0))
    // Orthographic light projection sized to the scene (cube + floor). ortho_rh is the
    // OpenGL convention (z in [-1, 1]); the shader remaps z to [0, 1] for the lookup.
    let light_proj = ortho_rh(-4.0, 4.0, -4.0, 4.0, 0.1, 20.0)
    u_light_vp = light_proj * light_view
    u_light_dir = light_dir

    // Camera orbits the scene, slightly elevated.
    let cam_angle = t * 0.35
    let cam_pos = float3(cos(cam_angle) * 5.0, 3.2, sin(cam_angle) * 5.0)
    u_view = look_at_rh(cam_pos, float3(0.0, CUBE_HOVER * 0.5, 0.0), float3(0, 1, 0))
    u_proj = perspective_rh_opengl(50.0 * PI / 180.0, aspect, 0.1, 50.0)
    u_cam_pos = cam_pos

    // ===== Pass 1: shadow map from the light's point of view =====
    glViewport(0, 0, SHADOW_DIM, SHADOW_DIM)
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo)
    glClear(GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LESS)
    glDepthMask(true)
    glDisable(GL_CULL_FACE)
    // slope-scaled depth bias pushes caster depth away from the light -> no acne
    glEnable(GL_POLYGON_OFFSET_FILL)
    glPolygonOffset(2.0, 4.0)
    glUseProgram(prog_shadow)
    u_model = cube_model(t)
    shadow_vs_bind_uniform(prog_shadow)
    draw_mesh(cube_vao, cube_ebo, 36)
    u_model = identity_m4()
    shadow_vs_bind_uniform(prog_shadow)
    draw_mesh(floor_vao, floor_ebo, 6)
    glDisable(GL_POLYGON_OFFSET_FILL)

    // ===== Pass 2: main scene, sampling the shadow map =====
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    glViewport(0, 0, display_w, display_h)
    glClearColor(0.04, 0.05, 0.09, 1.0)
    glDepthMask(true)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LESS)
    glDisable(GL_CULL_FACE)
    glUseProgram(prog_main)
    shadow_map := shadow_tex
    u_model = cube_model(t)
    u_is_floor = 0.0
    main_vs_bind_uniform(prog_main)
    main_fs_bind_uniform(prog_main)
    draw_mesh(cube_vao, cube_ebo, 36)
    u_model = identity_m4()
    u_is_floor = 1.0
    main_vs_bind_uniform(prog_main)
    main_fs_bind_uniform(prog_main)
    draw_mesh(floor_vao, floor_ebo, 6)

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
