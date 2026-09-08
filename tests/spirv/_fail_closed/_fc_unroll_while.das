// Fail-closed fixture: [unroll_full] on a while loop. Only a `for` over literal bounds unrolls at emission;
// a while loop has no copy count, so the hint is refused and the driver's [unroll] named instead.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_unroll_while_spv")]
def fc_unroll_while {
    let i = gl_GlobalInvocationID.x
    var acc = 0u
    var k = 0u
    while [unroll_full] (k < 4u) {
        acc += k
        k++
    }
    data[i] = acc
}
