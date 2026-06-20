// Fail-closed fixture (PR-E): a non-positive max_vertices must be rejected cleanly at the emit gate
// in generate_spirv (a count of 0 can't size an output array). `_`-prefix + `expect 50501` keep
// dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

[mesh_shader(local_size_x=1, max_vertices=0, name="fc_mesh_count_spv")]
def fc_mesh_count {
    SetMeshOutputsEXT(0u, 0u)
}
