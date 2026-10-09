options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

[test]
def test_the_boost_layer_compiles_for_the_webgl_target(t : T?) {
    // the web build compiles opengl_boost against the GLES3 binding, which has no GL 4.5 named-object API
    let fixture = "{get_das_root()}/modules/dasOpenGL/tests/_vertex_buffer_fixture.das"
    var output : string
    let rc = run_and_capture([das_exe(), "-dasroot", get_das_root(), "-compile-only", fixture, "--", "--jit-target", "wasm32-unknown-emscripten"], output, 120.0)
    t |> equal(rc, 0, "the fixture compiles for wasm32-unknown-emscripten, got:\n{output}")
}
