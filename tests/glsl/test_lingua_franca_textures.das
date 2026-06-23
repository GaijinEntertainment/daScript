options gen2
options indenting = 4
options no_unused_function_arguments = false

// Emission gate for the sampler-using lingua-franca builtins textureLod / texelFetch (the dasOpenGL
// <-> dasVulkan harmonization arc). Both moved into shader_lingua_franca and are GLSL name-identical
// (core + ES 3.0), so the GL emitter passes them through unchanged.
//
// A sampler emission test needs a @uniform sampler2D, whose generated <shader>_bind_uniform calls
// bind_sampler_2d / glGetUniformLocation / glUniformAny. The GL bind codegen emits those by BARE
// (unqualified) name, so the local dummy stubs below resolve them -- keeping this suite free of
// opengl/opengl_boost and its GL-runtime AOT module surface, exactly like the rest of tests/glsl.
// The test asserts only the emitted GLSL string (not the host bind), so the dummies are inert
// stand-ins; never invoked.

require dastest/testing_boost public
require glsl/glsl_opengl
require strings

// Host-bind dummies (see header): the sampler uniform's generated bind function calls these by name.
[unused_argument(unit, tex)] def bind_sampler_2d(unit : int; tex : uint) {}
[unused_argument(program, name)] def glGetUniformLocation(program : uint; name : string) : int { return 0 }
[unused_argument(location, value)] def glUniformAny(location : int; value : int) {}

var @in @location tx_pos : float2
var @inout tx_uv : float2
var @uniform tx_tex : sampler2D
var @out tx_color : float4

[vertex_program(name="tx_vs_glsl")]
def tx_vertex {
    tx_uv = tx_pos
    gl_Position = float4(tx_pos, 0.0, 1.0)
}

[fragment_program(name="tx_fs_glsl")]
def tx_fragment {
    let a = textureLod(tx_tex, tx_uv, 0.0)       // explicit-LOD sample
    let b = texelFetch(tx_tex, int2(0, 0), 0)    // unfiltered integer-coordinate fetch
    tx_color = a + b
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_lingua_franca_textures(t : T?) {
    t |> run("textureLod lowers name-identically on the GL rail") <| @(t : T?) {
        t |> success(has(tx_fs_glsl, "textureLod("), "fragment shader emits textureLod(...)")
    }
    t |> run("texelFetch lowers name-identically on the GL rail") <| @(t : T?) {
        t |> success(has(tx_fs_glsl, "texelFetch("), "fragment shader emits texelFetch(...)")
    }
    t |> run("the sampler uniform lowers to a sampler2D declaration") <| @(t : T?) {
        t |> success(has(tx_fs_glsl, "sampler2D"), "fragment shader declares the sampler2D uniform")
    }
}
