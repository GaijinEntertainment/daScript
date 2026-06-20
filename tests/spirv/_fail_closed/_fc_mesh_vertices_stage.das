// Fail-closed fixture (PR-E2): the mesh per-vertex output gl_MeshVerticesEXT is mesh-stage-only;
// referencing it from a vertex shader must be rejected cleanly by classify_mesh_output. `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

[vertex_shader(name="fc_mesh_vstage_spv")]
def fc_mesh_vstage {
    gl_MeshVerticesEXT[0].gl_Position = float4(0.0, 0.0, 0.0, 1.0)
}
