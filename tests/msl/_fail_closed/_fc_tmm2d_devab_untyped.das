//! `null` type-checks against the `float16 const?` operand but carries no float16 pointee

expect 50501

options gen2

require metal/msl_shader

class FcTmm2dDevabUntyped {
    @ssbo @binding = 0 xah : array<float16>
    @ssbo @binding = 1 oc : array<float>

    [metal_kernel(name="fc_tmm2d_devab_untyped_msl")]
    def kern {
        var acc : float[128]
        var cp = unsafe(addr(oc[0]))
        tmm2d_tg_begin_devab(acc, 8u, 16u, 2u)
        tmm2d_tg_step_devab(acc, unsafe(addr(xah[0])), 64u, null, 64u, 8u, 16u, 32u)
        tmm2d_tg_store(acc, cp, 8u, 16u, 16u)
    }
}
