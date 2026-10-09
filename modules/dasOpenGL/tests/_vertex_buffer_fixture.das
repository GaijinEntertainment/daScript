options gen2
require opengl/opengl_boost

[vertex_buffer]
struct ProbeVertex {
    xyz : float3
    uv : float2
}

//! Never run: each call must resolve to exactly one overload and strip `self` as a template argument.
def resolve_both_binders() {
    bind_vertex_buffer(null, type<ProbeVertex>)
    bind_vertex_buffer(1u, 2u, type<ProbeVertex>)
}
