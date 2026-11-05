// Fail-closed fixture: an rx product begun over int8 left elements is
// filled four signed bytes a word (tmm2d_rx_left4); the half fill's
// packed word would land as two halves converted into int8 elements,
// a silent wrong operand, so the half fill on an int8 product is
// rejected.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRx8Left2 {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx8_left2_msl")]
    def kern {
        var acc : float[128]
        tmm2d_rx_begin_i8(acc, unsafe(addr(xah[0])), 64u, 16u, 8u, 16u)
        tmm2d_rx_left2(acc, 0u, 2u, 0x3C003C00u)
        tmm2d_rx_run(acc, 0u)
        oc[gl_LocalInvocationID.x] = tmm2d_rx_dest(acc, 0u)
    }
}
