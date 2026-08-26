// Fail-closed fixture: a FLOAT A operand keeps matmul2d off the native half x half path, so
// every float-A tmm2d form carries the deliberate [metal_kernel(float_a_ok=true)] license. An
// unlicensed f32 activation panel is refused rather than silently stamped off the fast path —
// the failure mode is a kernel that compiles, runs, and is quietly slower.

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dFloatA {
    @ssbo @binding = 0 xa : array<float>
    @ssbo @binding = 1 wh : array<float16>
    @ssbo @binding = 2 oc : array<float>

    [metal_kernel(name="fc_tmm2d_float_a_msl")]
    def kern {
        var ap = unsafe(addr(xa[0]))
        var wp = unsafe(addr(wh[0]))
        var cp = unsafe(addr(oc[0]))
        tmm2d_f16w_f32(8u, 16u, 2u, ap, 64u, wp, 64u, cp, 16u, 64u)
    }
}
