// Fail-closed fixture (PR-F): a mesh @out varying whose array element type is not an emittable varying
// (here double) must be rejected with a clean compile error, not crash emit_type. `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @out @location = 0 fc_bad_out : array<double>

[mesh_shader(local_size_x=1, max_vertices=3, max_primitives=1, name="fc_mesh_out_badtype_spv")]
def fc_mesh_out_badtype {
    SetMeshOutputsEXT(1u, 1u)
    fc_bad_out[0] = 1.0lf
}
