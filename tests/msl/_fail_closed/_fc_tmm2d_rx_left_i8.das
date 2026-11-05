// Fail-closed fixture: the half fill (tmm2d_rx_left) on a product
// begun over int8 left elements would convert a half into an int8
// element, a silent wrong operand, so it is rejected.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRxLeftI8 {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx_left_i8_msl")]
    def kern {
        var acc : float[128]
        tmm2d_rx_begin_i8(acc, unsafe(addr(xah[0])), 64u, 16u, 8u, 16u)
        tmm2d_rx_left(acc, 0u, ushort_bits_to_half(0x3C00u))
        tmm2d_rx_run(acc, 0u)
        oc[gl_LocalInvocationID.x] = tmm2d_rx_dest(acc, 0u)
    }
}
