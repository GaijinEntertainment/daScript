// Fail-closed fixture: a run on an accumulator no rx begin opened has
// no operands to multiply, so it is rejected at emit time.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRxRunUnbegun {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx_run_unbegun_msl")]
    def kern {
        var acc : float[128]
        tmm2d_rx_run(acc, 0u)
        oc[gl_LocalInvocationID.x] = acc[0]
    }
}
