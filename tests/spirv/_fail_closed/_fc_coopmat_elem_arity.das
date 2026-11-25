// Fail-closed fixture: the emitter lowers coopmatElem by name, so a
// module function of that name with another arity reaches the
// intrinsic arm and must be rejected at emit time with a clean
// diagnostic.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_cea_out : array<int>

def coopmatElem(x : int) : int => x * 2

[compute_shader(local_size_x=32, name="fc_coopmat_elem_arity_spv")]
def fc_coopmat_elem_arity {
    let tid = gl_LocalInvocationID.x
    fc_cea_out[tid] = coopmatElem(fc_cea_out[tid])
}
