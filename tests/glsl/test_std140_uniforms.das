options gen2
options indenting = 4
options no_unused_function_arguments = false

// Emission gate for std140 UBO packing (Phase 2 / D5 of the dasOpenGL <-> dasVulkan harmonization). A
// struct @uniform / @push_constant packs as a `layout(std140) uniform <instance>_ubo { fields }
// <instance>;` interface block (the GL/WebGL2 portable floor) instead of a free `uniform Struct name;`.
// The generated <shader>_bind_uniform calls std140_reset / std140_set / std140_flush by BARE name
// (resolved in the consumer's opengl_boost chain), so the local dummy stubs below keep this suite free
// of opengl/opengl_boost and its GL-runtime AOT surface, exactly like the sampler test. The test
// asserts only the emitted GLSL string (not the host bind), so the dummies are inert stand-ins.

require dastest/testing_boost public
require glsl/glsl_opengl
require math
require strings

// Host-bind dummies (see header): the generated std140 bind body calls these by name.
[unused_argument(program, blockName, size)] def std140_reset(program : uint; blockName : string; size : int) {}
[unused_argument(program, blockName, offset, value)] def std140_set(program : uint; blockName : string; offset : uint; value) {}
[unused_argument(program, blockName, bindingPoint)] def std140_flush(program : uint; blockName : string; bindingPoint : int) {}

struct SceneUniforms {
    model     : float4x4
    tint      : float4
    intensity : float
}

var @uniform u_scene : SceneUniforms
var @in @location u_pos : float3
var @out u_color : float4

[vertex_program(name="ubo_vs_glsl")]
def ubo_vertex {
    gl_Position = u_scene.model * float4(u_pos, 1.0)
}

[fragment_program(name="ubo_fs_glsl")]
def ubo_fragment {
    u_color = u_scene.tint * u_scene.intensity
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_std140_uniforms(t : T?) {
    t |> run("a struct @uniform packs as a std140 interface block") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "layout(std140) uniform"), "vertex emits a layout(std140) uniform block")
    }
    t |> run("the block name is <instance>_ubo, not the struct type name") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "uniform u_scene_ubo"), "block is named u_scene_ubo")
    }
    t |> run("the std140 block declares its fields inline") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "mat4 model") && has(ubo_vs_glsl, "vec4 tint") && has(ubo_vs_glsl, "float intensity"),
            "block declares mat4 model / vec4 tint / float intensity")
    }
    t |> run("the block instance keeps the daslang global's name") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "} u_scene;"), "block instance is u_scene")
    }
    t |> run("the shader body reads fields through the instance") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "u_scene.model"), "body reads u_scene.model")
    }
    t |> run("the fragment stage shares the same block (the linker merges it)") <| @(t : T?) {
        t |> success(has(ubo_fs_glsl, "layout(std140) uniform u_scene_ubo"), "fragment also declares u_scene_ubo")
    }
}
