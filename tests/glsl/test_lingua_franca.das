options gen2
options indenting = 4
options no_unused_function_arguments = false

// Emission gate for the shader_lingua_franca union builtins (the dasOpenGL<->dasVulkan
// harmonization arc). Each [vertex_program]/[fragment_program] bakes its emitted GLSL into a
// compile-time `const string` global; we assert the GL lowering of the three builtins grown into
// the shared surface. No GPU — pure compile-time emission, so this is the CI regression lock the
// arc's union growth previously lacked.
//   gl_VertexIndex -> gl_VertexID   (the unified Vulkan spelling, renamed on the GL way out)
//   fwidth         -> fwidth(...)     (name-identical passthrough)
//   gl_FragCoord   -> gl_FragCoord    (name-identical passthrough)

require dastest/testing_boost public
require glsl/glsl_opengl
require strings

var @in @location lf_v_pos : float2
var @out lf_frag_color : float4

[vertex_program(name="lf_vs_glsl")]
def lf_vertex {
    gl_Position = float4(lf_v_pos, 0.0, 1.0)
    if (gl_VertexIndex == 0) {
        gl_Position = float4(0.0, 0.0, 0.0, 1.0)
    }
}

[fragment_program(name="lf_fs_glsl")]
def lf_fragment {
    let w = fwidth(gl_FragCoord.x)
    lf_frag_color = float4(w, gl_FragCoord.y, 0.0, 1.0)
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_lingua_franca_builtins(t : T?) {
    t |> run("gl_VertexIndex lowers to gl_VertexID on the GL rail") <| @(t : T?) {
        t |> success(has(lf_vs_glsl, "gl_VertexID"), "vertex shader emits gl_VertexID")
        t |> success(!has(lf_vs_glsl, "gl_VertexIndex"), "the Vulkan spelling gl_VertexIndex is fully renamed away")
    }
    t |> run("fwidth + gl_FragCoord lower name-identically") <| @(t : T?) {
        t |> success(has(lf_fs_glsl, "fwidth("), "fragment shader emits fwidth(...)")
        t |> success(has(lf_fs_glsl, "gl_FragCoord"), "fragment shader emits gl_FragCoord")
    }
    t |> run("both stages carry the desktop-core version line") <| @(t : T?) {
        t |> success(has(lf_vs_glsl, "#version 330 core"), "vertex shader has #version 330 core")
        t |> success(has(lf_fs_glsl, "#version 330 core"), "fragment shader has #version 330 core")
    }
}
