// Fail-closed fixture: a `var` (writable) parameter in a VALUE-position splice. A writable
// parameter needs a hoisted local of its own, and value position has nowhere to put a statement.

expect 50501

options gen2

require metal/msl_shader

class FcSpliceWritable {
    @ssbo @binding = 0 dat : array<float>

    def bump(var v : float) : float => v * 2.0

    [metal_kernel(name="fc_splice_writable_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = bump(dat[i])
    }
}
