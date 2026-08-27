// Fail-closed fixture: the q8u staged K-chunk depth is a compile-time constant that NAMES the
// generated helper, and only the 32- and 64-deep bodies exist. Any other bk would name a helper
// the emitter never wrote, so the call site is refused here rather than at MSL link time.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dBk {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 bsh : array<float16>
    @ssbo @binding = 2 bqb : array<int8>
    @ssbo @binding = 3 oc : array<float>
    @workgroup tqu : float16[512]

    [metal_kernel(name="fc_tmm2d_bk_msl")]
    def kern {
        var sp = unsafe(addr(bsh[0]))
        var wp = unsafe(addr(bqb[2]))
        var ap = unsafe(addr(xah[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_q8u_f32(8u, 16u, 2u, tqu, sp, wp, ap, 64u, cp, 16u, 64u, 2u, gl_LocalInvocationIndex, 48u)
    }
}
