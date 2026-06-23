options gen2
options indenting = 4
options no_unused_function_arguments = false

// Emission gate for Phase 2 of the dasOpenGL <-> dasVulkan harmonization arc: the GL generator now
// ACCEPTS the Vulkan-style @-annotations and maps them to GL semantics via the existing loose-uniform
// binder. A shader authored exactly like a dasVulkan shader compiles + emits on the GL rail:
//   - @uniform struct  -> `uniform StructType name;` (a GLSL struct uniform); fields bound loose
//   - @push_constant   -> a GL uniform (GL has no push constants), NOT a dead zero-init global
//   - @set             -> ignored (GL has one implicit descriptor set)
//   - @binding         -> tolerated
//   - @out @location in the vertex stage -> a real `out` varying
//
// Dummy host-bind stubs keep this suite free of opengl/opengl_boost (see test_lingua_franca_textures.das):
// the struct uniform's generated bind flattens to glUniformAny(loc, name.field) per leaf field, so the
// only stubs needed are glGetUniformLocation + the float4x4 glUniformAny. Asserts the emitted GLSL only.

require dastest/testing_boost public
require glsl/glsl_opengl
require math
require strings

[unused_argument(program, name)] def glGetUniformLocation(program : uint; name : string) : int { return 0 }
[unused_argument(location, value)] def glUniformAny(location : int; value : float4x4) {}

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

def private has(hay, needle : string) : bool => find(hay, needle) >= 0

[test]
def test_lingua_franca_annotations(t : T?) {
    t |> run("@uniform struct lowers to a GLSL struct uniform") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "uniform VkCamera vk_cam"), "emits `uniform VkCamera vk_cam`")
    }
    t |> run("@push_constant maps to a GL uniform, not a dead global") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "uniform VkModelPC vk_pc") && !has(vk_ann_vs, "VkModelPC vk_pc ="),
            "emits `uniform VkModelPC vk_pc;` with no zero-initializer")
    }
    t |> run("@out @location in the vertex stage emits a real out varying") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "out vec3 vk_v_nrm"), "emits `out vec3 vk_v_nrm`")
    }
    t |> run("Vulkan @set / @binding are tolerated on the GL rail") <| @(t : T?) {
        t |> success(has(vk_ann_vs, "void main"), "the VK-annotated shader emitted a body")
    }
}
