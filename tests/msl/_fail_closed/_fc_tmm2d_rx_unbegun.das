// Fail-closed fixture: a half fill on an accumulator no rx begin
// opened would emit a left tensor nothing declares - an MSL compile
// error later with no das-level trace - so the fill is rejected at
// emit time.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRxUnbegun {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx_unbegun_msl")]
    def kern {
        var acc : float[128]
        tmm2d_rx_left(acc, 0u, ushort_bits_to_half(0x3C00u))
        oc[gl_LocalInvocationID.x] = acc[0]
    }
}
