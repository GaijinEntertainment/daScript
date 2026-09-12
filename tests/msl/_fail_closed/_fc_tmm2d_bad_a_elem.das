// Fail-closed fixture: the tmm2d builtins bind any pointer type for the A stream (one generic
// over the activation pointee), so the emitter is the gate - an activation panel that is neither
// float nor float16 is refused, never lowered as if it were float.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dBadAElem {
    @ssbo @binding = 0 xa : array<int>
    @ssbo @binding = 1 wh : array<float16>
    @ssbo @binding = 2 oc : array<float>

    [metal_kernel(name="fc_tmm2d_bad_a_elem_msl", float_a_ok=true)]
    def kern {
        var ap = unsafe(addr(xa[0]))
        var wp = unsafe(addr(wh[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_f16w_f32(8u, 16u, 2u, ap, 64u, wp, 64u, cp, 16u, 64u)
    }
}
