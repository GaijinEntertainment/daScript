// Fail-closed fixture: [unroll_full] over a runtime bound. The emitter unrolls the loop at emission, so both
// bounds must be literals; a runtime bound is refused rather than silently downgraded to the driver's hint.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_unroll_runtime_spv")]
def fc_unroll_runtime {
    let i = gl_GlobalInvocationID.x
    let n = data[i]
    var acc = 0u
    for [unroll_full] (j in urange(0u, n)) {
        acc += j
    }
    data[i] = acc
}
