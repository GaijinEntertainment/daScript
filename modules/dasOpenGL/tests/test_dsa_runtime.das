options gen2
options no_aot

require dastest/testing_boost public
require daslib/safe_addr
require glfw/glfw_boost
require opengl/opengl_boost

struct Payload {
    a : float4
    b : int4
}

//! Runs `blk` on a hidden GL 4.5 window, or skips when no such context opens on this box.
def private with_gl_context(t : T?; blk : block<() : void>) {
    if (glfwInit() == 0) {
        t |> skip("glfw does not initialize here")
        return
    }
    glfwInitOpenGL(4, 5)
    glfwWindowHint(int(GLFW_VISIBLE), 0)
    let window = glfwCreateWindow(64, 64, "dsa runtime", null, null)
    if (window == null) {
        glfwTerminate()
        t |> skip("no GL 4.5 context on this box")
        return
    }
    glfwMakeContextCurrent(window)
    while (glGetError() != 0u) {
        pass
    }
    invoke(blk)
    glfwDestroyWindow(window)
    glfwTerminate()
}

def private buffer_param(ssbo : uint; pname : uint) : int {
    var value : int
    glGetNamedBufferParameteriv(ssbo, pname, safe_addr(value))
    return value
}

//! the round trip every mode must pass: a CPU write read back, through whatever the mode maps
def private check_ssbo_round_trip(t : T?; mode : string) {
    var sent = Payload(a = float4(1., 2., 3., 4.), b = int4(5, 6, 7, 8))
    let ssbo = create_ssbo(sent)
    t |> success(glIsBuffer(ssbo), "{mode}: create_ssbo makes a buffer")
    t |> equal(glGetError(), 0u, "{mode}: create_ssbo leaves no GL error")
    t |> equal(buffer_param(ssbo, GL_BUFFER_MAPPED) != 0, dsa_enabled(), "{mode}: only the DSA buffer stays mapped")
    t |> equal(glGetError(), 0u, "{mode}: the mapped query leaves no GL error")
    sent.a = float4(9., 10., 11., 12.)
    write_ssbo(ssbo, sent)
    var got : Payload
    read_ssbo(ssbo, got)
    t |> equal(got.a, sent.a, "{mode}: write_ssbo then read_ssbo")
    t |> equal(got.b, sent.b, "{mode}: the untouched field survives")
    t |> equal(glGetError(), 0u, "{mode}: the round trip leaves no GL error")
    delete_ssbo(ssbo)
    t |> success(!glIsBuffer(ssbo), "{mode}: delete_ssbo frees the name")
    t |> equal(glGetError(), 0u, "{mode}: delete_ssbo leaves no GL error")
}

def private check_texture_round_trip(t : T?; mode : string) {
    var pixels = fixed_array<uint8>(255u8, 0u8, 0u8, 255u8, 0u8, 255u8, 0u8, 255u8, 0u8, 0u8, 255u8, 255u8, 255u8, 255u8, 255u8, 255u8)
    let tex = load_image_from_bytes(2, 2, unsafe(addr<void?>(pixels[0])))
    t |> success(glIsTexture(tex), "{mode}: load_image_from_bytes makes a texture")
    t |> equal(getTexture2DSize(tex), int2(2, 2), "{mode}: getTexture2DSize reads the level")
    var name = tex
    glDeleteTextures(1, safe_addr(name))
    t |> equal(glGetError(), 0u, "{mode}: the texture round trip leaves no GL error")
}

let VS_SRC = "#version 450\nvoid main() \{ gl_Position = vec4(0.0); \}\n"
let FS_SRC = "#version 450\nout vec4 color;\nvoid main() \{ color = vec4(1.0); \}\n"

//! the flush must mint and fill a uniform buffer even for a program without that block
def private check_std140_flush(t : T?; mode : string) {
    let program = create_shader_program(VS_SRC, FS_SRC)
    std140_reset(program, "Probe", 32)
    std140_set(program, "Probe", 0u, float4(1., 2., 3., 4.))
    std140_flush(program, "Probe", 0)
    var ubo : int
    glGetIntegeri_v(GL_UNIFORM_BUFFER_BINDING, 0u, safe_addr(ubo))
    t |> success(ubo == 0, "{mode}: no block in the program means nothing bound to the point")
    std140_release(program)
    glDeleteProgram(program)
    t |> equal(glGetError(), 0u, "{mode}: the flush leaves no GL error")
}

[test]
def test_dsa_off_without_a_context(t : T?) {
    use_dsa(false)
    t |> success(!dsa_enabled(), "use_dsa(false) needs no context")
}

[test]
def test_dsa_runtime_paths(t : T?) {
    with_gl_context(t) {
        use_dsa(false)
        check_ssbo_round_trip(t, "legacy")
        check_texture_round_trip(t, "legacy")
        check_std140_flush(t, "legacy")
        t |> success(try_use_dsa(), "try_use_dsa accepts a 4.5 context")
        t |> success(dsa_enabled(), "the switch is on after try_use_dsa")
        check_ssbo_round_trip(t, "dsa")
        check_texture_round_trip(t, "dsa")
        check_std140_flush(t, "dsa")
        t |> equal(glGetError(), 0u, "no GL error across both modes")
        use_dsa(false)
    }
}
