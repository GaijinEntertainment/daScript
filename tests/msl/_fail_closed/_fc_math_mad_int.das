// Fail-closed fixture: mad on an integer class must be rejected - MSL has no integer fma, so the
// lowering would be an ambiguous call the Metal compiler refuses.

expect 50501

options gen2

require metal/msl_shader
require math

class FcMathMadInt {
    @ssbo @binding = 0 dst : array<int>

    [metal_kernel(name="fc_math_mad_int_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dst[i] = mad(int(i), 3, 4)
    }
}
