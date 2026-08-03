// Fail-closed fixture: an abstract method has no body to devirtualize to — the resolver must
// reject it cleanly, never emit a call into nothing.

expect 50501

options gen2

require metal/msl_shader

class FcAbs {
    @ssbo @binding = 0 dat : array<float>

    def abstract stage(v : float) : float

    [metal_kernel(name="fc_abs_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = stage(dat[i])
    }
}
