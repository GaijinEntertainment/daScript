// Fail-closed fixture: a three-argument math builtin on a non-numeric class (double here) must be
// rejected - the lowering carries only the float, int and uint classes.

expect 50501

options gen2

require metal/msl_shader
require math

class FcMathTernaryClass {
    @ssbo @binding = 0 dst : array<float>

    [metal_kernel(name="fc_math_ternary_class_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dst[i] = float(clamp(double(i), 1.0lf, 2.0lf))
    }
}
