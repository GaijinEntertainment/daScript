// Fail-closed fixture: the int8 fill (tmm2d_rx_left4) on a product
// begun over half left elements would land four bytes as four halves,
// a silent wrong operand, so it is rejected.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRxLeft4Half {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx_left4_half_msl")]
    def kern {
        var acc : float[128]
        tmm2d_rx_begin(acc, unsafe(addr(xah[0])), 64u, 16u, 8u, 32u)
        tmm2d_rx_left4(acc, 0u, 0x01010101u)
        tmm2d_rx_run(acc, 0u)
        oc[gl_LocalInvocationID.x] = tmm2d_rx_dest(acc, 0u)
    }
}
