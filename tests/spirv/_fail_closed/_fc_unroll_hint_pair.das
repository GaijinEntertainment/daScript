// Fail-closed fixture: a driver loop control beside [unroll_full]. The unrolled copies emit no loop construct
// to carry [dont_unroll], so the pair is refused rather than the second hint dropped without a word.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_unroll_hint_pair_spv")]
def fc_unroll_hint_pair {
    let i = gl_GlobalInvocationID.x
    var acc = 0u
    for [unroll_full, dont_unroll] (j in range(8)) {
        acc += uint(j)
    }
    data[i] = acc
}
