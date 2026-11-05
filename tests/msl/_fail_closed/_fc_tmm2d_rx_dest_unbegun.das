// Fail-closed fixture: a destination read on an accumulator no rx
// begin opened would name a product tensor nothing declares, so the
// read is rejected at emit time.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRxDestUnbegun {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx_dest_unbegun_msl")]
    def kern {
        var acc : float[128]
        oc[gl_LocalInvocationID.x] = tmm2d_rx_dest(acc, 0u)
    }
}
