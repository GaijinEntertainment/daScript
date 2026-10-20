options gen2
options indenting = 4
require dastest/testing_boost public
require glsl/glsl_opengl
require math
require strings

var calls : array<string>
def glUniformBound(_program : uint; name : string; _value) { calls |> push(name) }
var @uniform @opengl_manual_binding palette : float4x4[2]
var @uniform @opengl_manual_binding = false tint : float4
var @in @location position : float3
[vertex_program(name="manual_uniform_source")]
def manual_uniform_vertex {
    gl_Position = palette[0] * float4(position, 1.0) + tint
}
[test]
def manual_uniform_remains_in_shader_but_skips_automatic_binding(t : T?) {
    clear(calls)
    manual_uniform_vertex_bind_uniform(7u)
    t |> equal(length(calls), 1)
    t |> equal(calls[0], "tint")
    t |> success(find(manual_uniform_source, "uniform mat4 palette[2]") >= 0)
    palette[0] = identity4x4()
    glUniformBound(7u, "palette", palette)
    t |> equal(length(calls), 2)
    t |> equal(calls[1], "palette")
}
