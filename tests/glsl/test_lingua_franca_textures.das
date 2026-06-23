options gen2
options indenting = 4
options no_unused_function_arguments = false

// Emission gate for the sampler-using lingua-franca builtins (the dasOpenGL <-> dasVulkan
// harmonization arc). Covers textureLod / texelFetch (sampler2D, GLSL name-identical) plus the
// portable sampler dimensionalities that moved into shader_lingua_franca: samplerCube /
// sampler2DArray (sampled via texture(), name-identical) and sampler2DShadow (sampled via
// textureCompare(), which has NO GL builtin -- the emitter rewrites it to texture(s, vec3(uv, ref))).
//
// A sampler emission test needs @uniform samplers, whose generated <shader>_bind_uniform calls
// bind_sampler_* / glGetUniformLocation / glUniformAny. The GL bind codegen emits those by BARE
// (unqualified) name, so the local dummy stubs below resolve them -- keeping this suite free of
// opengl/opengl_boost and its GL-runtime AOT module surface, exactly like the rest of tests/glsl.
// The test asserts only the emitted GLSL string (not the host bind), so the dummies are inert
// stand-ins; never invoked.

require dastest/testing_boost public
require glsl/glsl_opengl
require strings

// Host-bind dummies (see header): each sampler uniform's generated bind function calls these by name.
[unused_argument(unit, tex)] def bind_sampler_2d(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_cube(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_2d_array(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_2d_shadow(unit : int; tex : uint) {}
[unused_argument(program, name)] def glGetUniformLocation(program : uint; name : string) : int { return 0 }
[unused_argument(location, value)] def glUniformAny(location : int; value : int) {}

var @in @location tx_pos : float2
var @inout tx_uv : float2
var @uniform tx_tex    : sampler2D
var @uniform tx_cube   : samplerCube
var @uniform tx_array  : sampler2DArray
var @uniform tx_shadow : sampler2DShadow
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

// Cube + 2D-array: both sampled via the dimension-agnostic texture() with a float3 coordinate
// (cubemap direction / (u, v, layer)). float3(...) emits as vec3(...) -- that is NOT the shadow path.
var @out tx2_color : float4
[fragment_program(name="tx2_fs_glsl")]
def tx2_fragment {
    let c = texture(tx_cube, float3(tx_uv, 1.0))
    let d = texture(tx_array, float3(tx_uv, 0.0))
    tx2_color = c + d
}

// Depth-compare (shadow): textureCompare must rewrite to texture(s, vec3(uv, ref)). This fragment's
// ONLY vec3 is that wrap (the output is a vec4), so a vec3( in the emission pins the lowering.
var @out tx_shadow_color : float4
[fragment_program(name="tx_shadow_fs_glsl")]
def tx_shadow_fragment {
    let s = textureCompare(tx_shadow, tx_uv, 0.5)
    tx_shadow_color = float4(s, s, s, 1.0)
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
    t |> run("samplerCube lowers to a samplerCube uniform sampled via texture()") <| @(t : T?) {
        t |> success(has(tx2_fs_glsl, "samplerCube") && has(tx2_fs_glsl, "texture("),
            "fragment declares samplerCube and samples it with texture(...)")
    }
    t |> run("sampler2DArray lowers to a sampler2DArray uniform") <| @(t : T?) {
        t |> success(has(tx2_fs_glsl, "sampler2DArray"), "fragment declares the sampler2DArray uniform")
    }
    t |> run("sampler2DShadow lowers to a sampler2DShadow uniform") <| @(t : T?) {
        t |> success(has(tx_shadow_fs_glsl, "sampler2DShadow"), "fragment declares the sampler2DShadow uniform")
    }
    t |> run("textureCompare rewrites to GL texture(s, vec3(uv, ref))") <| @(t : T?) {
        t |> success(has(tx_shadow_fs_glsl, "texture(") && has(tx_shadow_fs_glsl, "vec3("),
            "shadow sample emits texture(...) with a vec3 coordinate")
    }
    t |> run("the daslang-only textureCompare name is fully gone (GL has no such builtin)") <| @(t : T?) {
        t |> success(!has(tx_shadow_fs_glsl, "textureCompare"), "textureCompare(...) is rewritten away")
    }
}
