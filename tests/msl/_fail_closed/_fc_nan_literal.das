// Fail-closed fixture: a non-finite float literal (constant folding mints -nan here)
// must be rejected - MSL has no spelling for it.

expect 50501

options gen2

require metal/msl_shader

class FcNanLiteral {
    @ssbo @binding = 0 dst : array<float>

    [metal_kernel(name="fc_nan_literal_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dst[i] = 0.0f / 0.0f
    }
}
