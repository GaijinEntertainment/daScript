// Fail-closed fixture: a two-argument math builtin on a non-numeric class (double here) must be
// rejected - the lowering carries only the float, int and uint classes.

expect 50501

options gen2

require metal/msl_shader
require math

class FcMathBinaryClass {
    @ssbo @binding = 0 dst : array<float>

    [metal_kernel(name="fc_math_binary_class_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dst[i] = float(pow(double(i), 2.0lf))
    }
}
