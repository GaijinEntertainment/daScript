// Fail-closed fixture: an rx begin with a zero product depth would
// declare nothing and leave every later fill and read undefined, so a
// zero m, n or kk is rejected by name.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dRxZero {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_rx_zero_msl")]
    def kern {
        var acc : float[128]
        tmm2d_rx_begin(acc, unsafe(addr(xah[0])), 64u, 16u, 8u, 0u)
        tmm2d_rx_run(acc, 0u)
        oc[gl_LocalInvocationID.x] = tmm2d_rx_dest(acc, 0u)
    }
}
