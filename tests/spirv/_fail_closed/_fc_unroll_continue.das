// Fail-closed fixture: continue inside a [unroll_full] body. The unrolled copies form no loop construct, so a
// continue has no continue block to branch to; it is refused rather than lowered into an invalid structured CFG.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_unroll_continue_spv")]
def fc_unroll_continue {
    let i = gl_GlobalInvocationID.x
    var acc = 0u
    for [unroll_full] (j in range(8)) {
        if (j == 4) continue
        acc += uint(j)
    }
    data[i] = acc
}
