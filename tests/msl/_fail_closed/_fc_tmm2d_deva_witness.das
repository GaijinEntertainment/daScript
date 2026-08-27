// Fail-closed fixture: the deva begin's second argument is the A TYPE WITNESS — the destination
// cooperative tensor's layout is parameterized by the A element type, so begin and every step
// must stream the same one. A step streaming the other type would surface only as an MSL template
// error at PSO compile, with no das-level trace. The float witness below is licensed, so the
// element-type mismatch is the single thing left to reject.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dDevaWitness {
    @ssbo @binding = 0 xa : array<float>
    @ssbo @binding = 1 xah : array<float16>
    @ssbo @binding = 2 wh : array<float16>
    @ssbo @binding = 3 oc : array<float>
    @workgroup tgb : float16[512]

    [metal_kernel(name="fc_tmm2d_deva_witness_msl", float_a_ok=true)]
    def kern {
        var acc : float[128]
        var cp = unsafe(addr(oc[0]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(xa[0])), 8u, 16u, 2u, 32u)
        var i = gl_LocalInvocationID.x
        while (i < 512u) {
            tgb[i] = wh[i]
            i += gl_WorkGroupSize.x
        }
        barrier()
        tmm2d_tg_step_deva(acc, unsafe(addr(xah[0])), 64u, tgb, 8u, 16u, 32u)
        barrier()
        tmm2d_tg_store(acc, cp, 8u, 16u, 16u)
    }
}
