// Fail-closed fixture: a recursive METHOD cannot lower — same MSL rule as free functions,
// caught on the method discovery stack.

expect 50501

options gen2

require metal/msl_shader

class FcMRec {
    @ssbo @binding = 0 dat : array<int>

    def mrec(x : int) : int {
        return x <= 0 ? 0 : mrec(x - 1)
    }

    [metal_kernel(name="fc_mrec_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = mrec(dat[i])
    }
}
