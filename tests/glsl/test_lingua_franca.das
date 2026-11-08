options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require glsl/glsl_opengl
require strings
require math
require daslib/strings_boost

// Emission gate for the shader_lingua_franca union builtins (the
// dasOpenGL<->dasVulkan harmonization arc). Each
// [vertex_program]/[fragment_program] bakes its emitted GLSL into a
// compile-time `const string` global; we assert the GL lowering of
// the three builtins grown into the shared surface. No GPU — pure
// compile-time emission, so this is the CI regression lock the arc's
// union growth previously lacked. gl_VertexIndex   -> gl_VertexID
// (the unified Vulkan spelling, renamed on the GL way out)
// gl_InstanceIndex -> gl_InstanceID  (the sibling per-instance-index
// rename) fwidth           -> fwidth(...)     (name-identical
// passthrough) gl_FragCoord     -> gl_FragCoord    (name-identical
// passthrough) gl_FragDepth     -> gl_FragDepth    (name-identical
// passthrough, write-only Output)
var @in @location lf_v_pos : float2
var @out lf_frag_color : float4

[vertex_program(name="lf_vs_glsl")]
def lf_vertex {
    gl_Position = float4(lf_v_pos, 0.0, 1.0)
    if (gl_VertexIndex == 0) {
        gl_Position = float4(0.0, 0.0, 0.0, 1.0)
    }
    if (gl_InstanceIndex > 0) {
        gl_Position = float4(lf_v_pos, 0.0, 2.0)
    }
}

[fragment_program(name="lf_fs_glsl")]
def lf_fragment {
    let w = fwidth(gl_FragCoord.x)
    lf_frag_color = float4(w, gl_FragCoord.y, 0.0, 1.0)
    gl_FragDepth = gl_FragCoord.z
}

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test(description = "Vertex and instance index lower to gl_VertexID and gl_InstanceID, fwidth, gl_FragCoord and gl_FragDepth keep their names, and both stages carry the desktop-core version line.")]
def test_lingua_franca_builtins(t : T?) {
    t |> run("gl_VertexIndex lowers to gl_VertexID on the GL rail") <| @(t : T?) {
        t |> success(has(lf_vs_glsl, "gl_VertexID"), "vertex shader emits gl_VertexID")
        t |> success(!has(lf_vs_glsl, "gl_VertexIndex"),
                     "the Vulkan spelling gl_VertexIndex is fully renamed away")
    }
    t |> run("gl_InstanceIndex lowers to gl_InstanceID on the GL rail") <| @(t : T?) {
        t |> success(has(lf_vs_glsl, "gl_InstanceID"), "vertex shader emits gl_InstanceID")
        t |> success(!has(lf_vs_glsl, "gl_InstanceIndex"),
                     "the Vulkan spelling gl_InstanceIndex is fully renamed away")
    }
    t |> run("fwidth + gl_FragCoord + gl_FragDepth lower name-identically") <| @(t : T?) {
        t |> success(has(lf_fs_glsl, "fwidth("), "fragment shader emits fwidth(...)")
        t |> success(has(lf_fs_glsl, "gl_FragCoord"), "fragment shader emits gl_FragCoord")
        t |> success(has(lf_fs_glsl, "gl_FragDepth"),
                     "fragment shader emits gl_FragDepth (write-only Output)")
    }
    t |> run("both stages carry the desktop-core version line") <| @(t : T?) {
        t |> success(has(lf_vs_glsl, "#version 330 core"), "vertex shader has #version 330 core")
        t |> success(has(lf_fs_glsl, "#version 330 core"), "fragment shader has #version 330 core")
    }
}

// Emission gate for Phase 2 of the dasOpenGL <-> dasVulkan
// harmonization arc: the GL generator ACCEPTS the Vulkan-style
// @-annotations and maps them to GL semantics. A shader authored
// exactly like a dasVulkan shader compiles + emits on the GL rail: -
// @uniform / @push_constant struct -> a std140 UBO block,
// `layout(std140) uniform <name>_ubo { ... } <name>;`, bound via
// std140_reset/set/flush (the GL/WebGL2 portable floor). float3x3 /
// fixed-arrays / nested structs / arrays-of-struct all pack -- the
// shared std140 walk (shader_block_layout) decomposes them into
// per-column / per-element / nested leaf writes. - @push_constant
// -> packs like @uniform (GL has no push constants), NOT a dead
// global - @set             -> ignored (GL has one implicit
// descriptor set) - @binding         -> tolerated - @out @location in
// the vertex stage -> a real `out` varying
//
// Dummy host-bind stubs keep this suite free of opengl/opengl_boost
// (see test_lingua_franca_textures.das): every packed block's bind
// calls std140_reset/set/flush by bare name. Asserts the emitted GLSL
// only.
[unused_argument(program, blockName, size)] def std140_reset(program : uint; blockName : string;
                                                             size : int) {}
[unused_argument(program, blockName, offset, value)] def std140_set(program : uint;
                                                                    blockName : string;
    offset : uint; value) {}
[unused_argument(program, blockName, bindingPoint)] def std140_flush(program : uint;
                                                                     blockName : string;
    bindingPoint : int) {}

struct VkCamera {
    view : float4x4
    proj : float4x4
}
var @uniform @set = 0 @binding = 0 vk_cam : VkCamera

struct VkModelPC {
    model : float4x4
}
var @push_constant vk_pc : VkModelPC

var @in @location = 0 vk_a_pos : float3
var @out @location = 0 vk_v_nrm : float3

[vertex_program(name="vk_ann_vs")]
def vk_ann_vertex {
    gl_Position = vk_cam.proj * vk_cam.view * vk_pc.model * float4(vk_a_pos, 1.0)
    vk_v_nrm = vk_a_pos
}

// A nested struct + mat3 + array-of-struct (the deferred-tut UBO
// shape) ALSO packs std140: the shared std140 walk emits per-column
// writes for normal_mat (float3x3) and per-element writes for
// lights[] (array of struct), so it is a layout(std140) UBO block
// like the flat ones above.
struct VkLight {
    pos   : float4
    color : float4
}
struct VkMaterial {
    normal_mat : float3x3
    base_color : float4
    lights     : VkLight[2]
}
var @uniform vk_mat : VkMaterial
var @out @location = 0 vk_lit : float4

// vk_mat is collected by USING any field; the block then declares
// EVERY field (incl normal_mat / lights[]) and the std140 walk packs
// them all. The body avoids a matrix-column index (`normal_mat[0]`
// trips a no-opt bounds-check the GLSL emitter rejects) -- the mat3
// is repacked host-side per column, not read column-wise in the
// shader.
[fragment_program(name="vk_mat_fs")]
def vk_mat_fragment {
    vk_lit = vk_mat.base_color + vk_mat.lights[0].color + vk_mat.lights[1].color
}

[test(description = "@uniform and @push_constant structs pack as std140 UBO blocks, @out varyings are emitted, Vulkan @set and @binding are tolerated, and nested or array-of-struct blocks stay in the block.")]
def test_lingua_franca_annotations(t : T?) {
    t |> run("@uniform struct (all-flat fields) packs as a std140 UBO block") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "layout(std140) uniform vk_cam_ubo"),
                     "emits `layout(std140) uniform vk_cam_ubo`")
    }
    t |> run("@push_constant packs/binds like a UBO, not a dead global (GL has no push constants)") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "layout(std140) uniform vk_pc_ubo")
                     && !has(vk_ann_vs, "vk_pc ="),
            "emits `layout(std140) uniform vk_pc_ubo` with no zero-initializer")
    }
    t |> run("@out @location in the vertex stage emits a real out varying") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "out vec3 vk_v_nrm"), "emits `out vec3 vk_v_nrm`")
    }
    t |> run("Vulkan @set / @binding are tolerated on the GL rail") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "void main"), "the VK-annotated shader emitted a body")
    }
    t |> run("std140 pack: a nested struct + mat3 block packs as a UBO") <| @(t : T?) {
        t |> success(has(vk_mat_fs, "layout(std140) uniform vk_mat_ubo")
                     && has(vk_mat_fs, "mat3 normal_mat"),
            "emits `layout(std140) uniform vk_mat_ubo` with a mat3 member (per-column packed host-side)")
    }
    t |> run("std140 pack: an array-of-struct field stays in the block") <| @(t : T?) {
        t |> success(has(vk_mat_fs, "VkLight lights[2]"),
                     "emits the VkLight lights[2] array member")
    }
}

// Emission gate for the sampler-using lingua-franca builtins (the
// dasOpenGL <-> dasVulkan harmonization arc). Covers textureLod /
// texelFetch (sampler2D, GLSL name-identical) plus the portable
// sampler dimensionalities that moved into shader_lingua_franca:
// samplerCube / sampler2DArray (sampled via texture(),
// name-identical) and sampler2DShadow (sampled via textureCompare(),
// which has NO GL builtin -- the emitter rewrites it to texture(s,
// vec3(uv, ref))).
//
// A sampler emission test needs @uniform samplers, whose generated
// <shader>_bind_uniform calls bind_sampler_* / glUniformBound. The GL
// bind codegen emits those by BARE (unqualified) name, so the local
// dummy stubs below resolve them -- keeping this suite free of
// opengl/opengl_boost and its GL-runtime AOT module surface, exactly
// like the rest of tests/glsl. The test asserts only the emitted GLSL
// string (not the host bind), so the dummies are inert stand-ins;
// never invoked. Host-bind dummies (see header): each sampler
// uniform's generated bind function calls these by name.
[unused_argument(unit, tex)] def bind_sampler_3d(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_2d(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_cube(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_2d_array(unit : int; tex : uint) {}
[unused_argument(unit, tex)] def bind_sampler_2d_shadow(unit : int; tex : uint) {}
[unused_argument(program, name, value)] def glUniformBound(program : uint; name : string;
                                                           value : int) {}

var @in @location tx_pos : float2
var @inout tx_uv : float2
var @uniform tx_volume : sampler3D
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

// Cube + 2D-array: both sampled via the dimension-agnostic texture()
// with a float3 coordinate (cubemap direction / (u, v, layer)).
// float3(...) emits as vec3(...) -- that is NOT the shadow path.
var @out tx2_color : float4
[fragment_program(name="tx2_fs_glsl")]
def tx2_fragment {
    let c = texture(tx_cube, float3(tx_uv, 1.0))
    let d = texture(tx_array, float3(tx_uv, 0.0))
    tx2_color = c + d
}

// Depth-compare (shadow): textureCompare must rewrite to texture(s,
// vec3(uv, ref)). This fragment's ONLY vec3 is that wrap (the output
// is a vec4), so a vec3( in the emission pins the lowering.
var @out tx_shadow_color : float4
[fragment_program(name="tx_shadow_fs_glsl")]
def tx_shadow_fragment {
    let s = textureCompare(tx_shadow, tx_uv, 0.5)
    tx_shadow_color = float4(s, s, s, 1.0)
}

[fragment_program(name="tx_grad_fs_glsl")]
def tx_grad_fragment {
    let dx = dFdx(tx_uv)
    let dy = dFdy(tx_uv)
    tx_color = (textureGrad(tx_tex, tx_uv, dx, dy)
        + textureGrad(tx_volume, float3(tx_uv, 0.5), float3(dx, 0.0), float3(dy, 0.0))
        + textureGrad(tx_cube, float3(tx_uv, 1.0), float3(dx, 0.0), float3(dy, 0.0))
        + textureGrad(tx_array, float3(tx_uv, 2.0), dx, dy))
}

[test(description = "textureGrad, textureLod, texelFetch and the sampler types lower to their GLSL names and textureCompare rewrites to texture(s, vec3(uv, ref)).")]
def test_lingua_franca_textures(t : T?) {
    t |> run("textureGrad contributes all four sampler types to output") <| @(t : T?) {
        var assignment = ""
        for (statement in split(tx_grad_fs_glsl, ";")) {
            if (has(statement, "tx_color =")) { assignment = statement }
        }
        t |> success(assignment != "", "fragment shader assigns its output")
        for (sampler_name in fixed_array("tx_tex", "tx_volume", "tx_cube", "tx_array")) {
            t |> success(has(assignment, "textureGrad({sampler_name},"),
                "output assignment includes textureGrad for {sampler_name}")
        }
    }
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
        t |> success(has(tx2_fs_glsl, "sampler2DArray"),
                     "fragment declares the sampler2DArray uniform")
    }
    t |> run("sampler2DShadow lowers to a sampler2DShadow uniform") <| @(t : T?) {
        t |> success(has(tx_shadow_fs_glsl, "sampler2DShadow"),
                     "fragment declares the sampler2DShadow uniform")
    }
    t |> run("textureCompare rewrites to GL texture(s, vec3(uv, ref))") <| @(t : T?) {
        t |> success(has(tx_shadow_fs_glsl, "texture(") && has(tx_shadow_fs_glsl, "vec3("),
            "shadow sample emits texture(...) with a vec3 coordinate")
    }
    t |> run("the daslang-only textureCompare name is fully gone (GL has no such builtin)") <| @(t : T?) {
        t |> success(!has(tx_shadow_fs_glsl, "textureCompare"),
                     "textureCompare(...) is rewritten away")
    }
}

// Emission gate for std140 UBO packing (Phase 2 / D5 of the dasOpenGL
// <-> dasVulkan harmonization). A struct @uniform / @push_constant
// packs as a `layout(std140) uniform <instance>_ubo { fields }
// <instance>;` interface block (the GL/WebGL2 portable floor) instead
// of a free `uniform Struct name;`. The generated
// <shader>_bind_uniform calls std140_reset / std140_set /
// std140_flush by BARE name (resolved in the consumer's opengl_boost
// chain), so the local dummy stubs at the top of the file keep this
// suite free of opengl/opengl_boost and its GL-runtime AOT surface,
// exactly like the sampler test. The test asserts only the emitted
// GLSL string (not the host bind), so the dummies are inert
// stand-ins.

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

[test(description = "A struct @uniform packs as a <instance>_ubo std140 block with inline fields that the shader body reads through the instance, shared by the fragment stage.")]
def test_std140_uniforms(t : T?) {
    t |> run("a struct @uniform packs as a std140 interface block") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "layout(std140) uniform"),
                     "vertex emits a layout(std140) uniform block")
    }
    t |> run("the block name is <instance>_ubo, not the struct type name") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "uniform u_scene_ubo"), "block is named u_scene_ubo")
    }
    t |> run("the std140 block declares its fields inline") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "mat4 model") && has(ubo_vs_glsl, "vec4 tint")
                     && has(ubo_vs_glsl, "float intensity"),
            "block declares mat4 model / vec4 tint / float intensity")
    }
    t |> run("the block instance keeps the daslang global's name") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "} u_scene;"), "block instance is u_scene")
    }
    t |> run("the shader body reads fields through the instance") <| @(t : T?) {
        t |> success(has(ubo_vs_glsl, "u_scene.model"), "body reads u_scene.model")
    }
    t |> run("the fragment stage shares the same block (the linker merges it)") <| @(t : T?) {
        t |> success(has(ubo_fs_glsl, "layout(std140) uniform u_scene_ubo"),
                     "fragment also declares u_scene_ubo")
    }
}
