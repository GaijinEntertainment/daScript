// Fail-closed fixture (PR-E): an unrecognized output_topology must be rejected cleanly by the
// [mesh_shader] annotation patch (only "triangles" / "lines" / "points" are valid). `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

[mesh_shader(local_size_x=1, output_topology="quads", name="fc_mesh_topology_spv")]
def fc_mesh_topology {
    SetMeshOutputsEXT(0u, 0u)
}
