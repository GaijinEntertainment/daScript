// Fail-closed fixture: a recursive user function cannot lower — MSL forbids recursion; the
// discovery stack must reject it with a clean error, never emit a self-calling MSL function.

expect 50501

options gen2

require metal/msl_shader

def rec_down(x : int) : int {
    return x <= 0 ? 0 : rec_down(x - 1)
}

class FcRecursion {
    @ssbo @binding = 0 dat : array<int>

    [metal_kernel(name="fc_recursion_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = rec_down(dat[i])
    }
}
