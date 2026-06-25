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

// Scalar float literals in non-constructor contexts (init, comparison,
// assignment, division). The `:g` formatter drops the decimal point on
// integer-valued floats (4.0 -> "4"), which GLSL ES 3.00 (WebGL2) rejects as an
// int in these positions; the emitter must re-attach ".0". See glsl_float_literal.
[fragment_program(name="floatlit_fs_glsl")]
def floatlit_fragment {
    var acc = 0.0
    if (b_vcolor.x > 4.0) {
        acc = 2.0
    }
    b_frag_color = float4(acc / 256.0, 0.0, 0.0, 1.0)
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
    t |> run("integer-valued float literals keep a decimal point (GLSL ES strictness)") <| @(t : T?) {
        t |> success(has(floatlit_fs_glsl, "= 0.0"), "scalar float init keeps the decimal point")
        t |> success(has(floatlit_fs_glsl, "> 4.0"), "comparison literal keeps the decimal point")
        t |> success(has(floatlit_fs_glsl, "= 2.0"), "assignment literal keeps the decimal point")
        t |> success(has(floatlit_fs_glsl, "256.0"), "divisor literal keeps the decimal point")
        // negative control: the bare-int form `acc = 0;` / `> 4)` must not appear
        t |> success(!has(floatlit_fs_glsl, "= 0;"), "no bare-int float initializer")
        t |> success(!has(floatlit_fs_glsl, "> 4)"), "no bare-int comparison literal")
    }
}
