//! the emitter lowers by bare name, so a same-named local function reaches the devab step's arity check

expect 50501

options gen2

require metal/msl_shader

def tmm2d_tg_step_devab(var acc : float[128]; k : uint) {
    acc[0] += float(k)
}

class FcTmm2dDevabArgc {
    @ssbo @binding = 0 oc : array<float>

    [metal_kernel(name="fc_tmm2d_devab_argc_msl")]
    def kern {
        var acc : float[128]
        var cp = unsafe(addr(oc[0]))
        tmm2d_tg_begin_devab(acc, 8u, 16u, 2u)
        tmm2d_tg_step_devab(acc, 32u)
        tmm2d_tg_store(acc, cp, 8u, 16u, 16u)
    }
}
