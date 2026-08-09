// Fail-closed fixture: a value-returning method called as a STATEMENT. The splice would drop the
// value silently, so the emitter refuses it rather than emit a body whose result goes nowhere.

expect 50501

options gen2

require metal/msl_shader

class FcMDiscard {
    @ssbo @binding = 0 dat : array<float>

    def dbl(i : uint) : float => dat[i] * 2.0

    [metal_kernel(name="fc_mdiscard_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dbl(i)
        dat[i] = 1.0
    }
}
