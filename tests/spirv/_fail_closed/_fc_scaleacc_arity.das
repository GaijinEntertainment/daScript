// Fail-closed fixture: the emitter lowers coopmatScaleAcc by name, so
// a module function of that name with another arity reaches the
// intrinsic arm and must be rejected at emit time with a clean
// diagnostic.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_saa_out : array<float>

def coopmatScaleAcc(x : float) : float => x * 2.0

[compute_shader(local_size_x=64, name="fc_scaleacc_arity_spv")]
def fc_scaleacc_arity {
    let tid = gl_LocalInvocationID.x
    fc_saa_out[tid] = coopmatScaleAcc(fc_saa_out[tid])
}
