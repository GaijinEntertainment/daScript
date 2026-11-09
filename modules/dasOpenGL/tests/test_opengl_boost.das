options gen2
options no_aot
options persistent_heap

require dastest/testing_boost public
require daslib/defer
require daslib/fio
require daslib/safe_addr
require glfw/glfw_boost
require opengl/gl_binding_cache
require opengl/opengl_boost

//! The opengl_boost layer: the uniform binding cache, program
//! retirement, the DSA switch and the WebGL build of the layer.
//! Retirement asserts on heap_bytes_allocated, which needs
//! persistent_heap.

struct Payload {
    a : float4
    b : int4
}

var @uniform retirement_color : float4
var @out retirement_output : float4
[vertex_program]
def retirement_vs {gl_Position = float4(0.0, 0.0, 0.0, 1.0)}
[fragment_program]
def retirement_fs {retirement_output = retirement_color}

def private fill_and_retire(program : uint){
    gl_binding_begin()
    gl_use_program_filtered(program)
    glUniformBound(program, "retirement_color", float4(0.5))
    gl_binding_end()
    gl_binding_forget_program(program)
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

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

[test(description = "The uniform binding cache skips an identical payload, uploads when any component, the ownership epoch or the size changes, and keeps the new size.")]
def test_uniform_payload_and_epoch(t : T?){
    var entry = GlUniformBinding()
    var values = fixed_array(float4(1.0), float4(2.0))
    let data = unsafe(addr<void?>(values))
    let size = typeinfo sizeof(values)
    t |> success(gl_binding_value_changed(entry, 1ul, data, size), "first upload")
    t |> success(!gl_binding_value_changed(entry, 1ul, data, size), "identical array skipped")
    values[1].w = 3.0
    t |> success(gl_binding_value_changed(entry, 1ul, data, size), "last component compared")
    t |> success(gl_binding_value_changed(entry, 2ul, data, size), "new ownership interval uploads unchanged payload")
    t |> success(gl_binding_value_changed(entry, 2ul, data, 16), "size changes invalidate payload")
    t |> success(!gl_binding_value_changed(entry, 2ul, data, 16), "new size retained")
    delete entry
}

[test(description = "A binding scope relinquishes ownership when a panic inside it is recovered, and retiring a program frees its uniform snapshots without a GC, so twenty fill-and-retire rounds leave the heap where it was; skipped without a GL 3.3 context.")]
def retired_program_releases_nested_uniform_storage(t : T?){
    if (glfwInit() == 0){t |> skip("GLFW display unavailable"); return }
    glfwInitOpenGL(3, 3)
    glfwWindowHint(int(GLFW_VISIBLE), GLFW_FALSE)
    let window = glfwCreateWindow(32, 32, "Binding retirement", null, null)
    if (window == null){glfwTerminate(); t |> skip("OpenGL context unavailable"); return }
    glfwMakeContextCurrent(window)
    let program = create_shader_program(@@retirement_vs, @@retirement_fs)
    defer(){gl_binding_clear(); glDeleteProgram(program); glfwDestroyWindow(window); glfwTerminate()}
    t |> success(program != 0u)
    if (program == 0u){return }
    var recovered = false
    try {gl_binding_scope(true) $ {panic("scope failure")}} recover {recovered = true}
    t |> success(recovered)
    t |> success(!gl_binding_filter_active, "scope relinquishes ownership on recovered panic")
    if (gl_binding_filter_active){gl_binding_end()}
    fill_and_retire(program)
    let before = heap_bytes_allocated()
    for (_i in range(20)){fill_and_retire(program)}
    let after = heap_bytes_allocated()
    t |> equal(after, before, "retired uniform snapshots are freed without GC")
    t |> equal(glGetError(), uint(GL_NO_ERROR))
}

[test(description = "use_dsa(false) turns the DSA path off with no GL context current.")]
def test_dsa_off_without_a_context(t : T?) {
    use_dsa(false)
    t |> success(!dsa_enabled(), "use_dsa(false) needs no context")
}

[test(description = "On a GL 4.5 context the SSBO, texture and std140 helpers round-trip without a GL error on both the legacy and the DSA path, only the DSA buffer stays mapped, and try_use_dsa turns the DSA path on; skipped without such a context.")]
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

[test(description = "A program requiring opengl_boost compiles for the wasm32-unknown-emscripten target, where the GLES3 binding has no GL 4.5 named-object API.", timeout = 1200)]
def test_the_boost_layer_compiles_for_the_webgl_target(t : T?) {
    let fixture = "{get_das_root()}/modules/dasOpenGL/tests/_vertex_buffer_fixture.das"
    var output : string
    let rc = run_and_capture([das_exe(), "-dasroot", get_das_root(), "-no-module-cache",
                              "-compile-only", fixture, "--", "--jit-target",
                              "wasm32-unknown-emscripten"], output, 120.0)
    t |> equal(rc, 0, "the fixture compiles for wasm32-unknown-emscripten, got:\n{output}")
}
