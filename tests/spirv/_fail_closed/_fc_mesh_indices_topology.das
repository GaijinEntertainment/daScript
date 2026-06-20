// Fail-closed fixture (PR-E2): gl_PrimitiveTriangleIndicesEXT is the TRIANGLE connectivity array, so
// it requires output_topology="triangles". Using it under a "lines" topology must be rejected cleanly
// by classify_mesh_output. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

[mesh_shader(local_size_x=1, max_vertices=2, max_primitives=1, output_topology="lines", name="fc_mesh_topo_spv")]
def fc_mesh_topo {
    SetMeshOutputsEXT(2u, 1u)
    gl_PrimitiveTriangleIndicesEXT[0] = uint3(0u, 1u, 0u)
}
