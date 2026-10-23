// Fail-closed fixture: barrier() is a workgroup rendezvous, legal
// only in compute / mesh / task stages. A fragment shader calling it
// is rejected with a clean diagnostic.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @out @location = 0 fc_bs_color : float4

[fragment_shader(name="fc_barrier_stage_spv")]
def fc_barrier_stage {
    barrier()
    fc_bs_color = float4(1.0f, 0.0f, 0.0f, 1.0f)
}
