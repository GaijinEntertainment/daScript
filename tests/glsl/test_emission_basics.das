options gen2
options indenting = 4
options no_unused_function_arguments = false

// Baseline emission gate for the GLSL generator's general surface (untested except by use before
// this arc). Pins the version line, the @in/@out/@inout declaration lowering, and the entry-point
// body shape for a minimal vertex+fragment pair. Compile-time only (no GPU). A forward regression
// lock so an emitter change that reshapes the prelude/declarations/body is caught in CI.

require dastest/testing_boost public
require glsl/glsl_opengl
require strings

var @in @location b_pos : float2
var @in @location b_color : float3
var @inout b_vcolor : float3
var @out b_frag_color : float4

[vertex_program(name="basics_vs_glsl")]
def basics_vertex {
    b_vcolor = b_color
    gl_Position = float4(b_pos, 0.0, 1.0)
}

[fragment_program(name="basics_fs_glsl")]
def basics_fragment {
    b_frag_color = float4(b_vcolor, 1.0)
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_glsl_emission_basics(t : T?) {
    t |> run("vertex shader emission shape") <| @(t : T?) {
        t |> success(has(basics_vs_glsl, "#version 330 core"), "version line")
        t |> success(has(basics_vs_glsl, "in vec2 b_pos"), "vec2 attribute declaration")
        t |> success(has(basics_vs_glsl, "in vec3 b_color"), "vec3 attribute declaration")
        t |> success(has(basics_vs_glsl, "out vec3 b_vcolor"), "@inout varying lowers to out on the vertex side")
        t |> success(has(basics_vs_glsl, "void main()"), "entry point")
        t |> success(has(basics_vs_glsl, "gl_Position"), "writes gl_Position")
    }
    t |> run("fragment shader emission shape") <| @(t : T?) {
        t |> success(has(basics_fs_glsl, "#version 330 core"), "version line")
        t |> success(has(basics_fs_glsl, "in vec3 b_vcolor"), "@inout varying lowers to in on the fragment side")
        t |> success(has(basics_fs_glsl, "out vec4 b_frag_color"), "fragment output declaration")
        t |> success(has(basics_fs_glsl, "void main()"), "entry point")
    }
}
