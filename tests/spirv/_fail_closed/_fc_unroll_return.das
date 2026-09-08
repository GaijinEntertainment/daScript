// Fail-closed fixture: return inside a [unroll_full] body. A return in one copy leaves the copies after it
// unreachable, so the emitter refuses it instead of emitting instructions after a block terminator.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_unroll_return_spv")]
def fc_unroll_return {
    let i = gl_GlobalInvocationID.x
    var acc = 0u
    for [unroll_full] (j in range(8)) {
        acc += uint(j)
        if (acc > 6u) return
    }
    data[i] = acc
}
