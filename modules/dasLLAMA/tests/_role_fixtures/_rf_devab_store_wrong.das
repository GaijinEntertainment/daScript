options gen2
options indenting = 4
options _dasllama_internal = true

expect 20800   // the lens refuses this fixture BY DESIGN — see test_metal_role_infer.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common
require metal/das_metal_boost

// Fixture: _rf_devab_store with the store's buffer declared 'read'. The store writes through an
// inline unsafe(addr(dst[...])), so the declaration contradicts the body and must be refused.

var private g_rf_devab_bad_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_devab_bad", pso = "g_rf_devab_bad_pso", tg = 128, grid = "m/32, n/64", params = "m : int64, n : int64")]
class RfDevabStoreWrong {
    @ssbo @binding = 0 @role = "read" a : array<float16>
    @ssbo @binding = 1 @role = "read" b : array<float16>
    @ssbo @binding = 2 @role = "read" dst : array<float>
    @uniform @binding = 3 kdim : uint
    @uniform @binding = 4 ndim : uint

    [metal_kernel(name = "rf_devab_bad_msl")]
    def rf_devab_bad {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var acc : float[2048]
        tmm2d_tg_begin_devab(acc, 32u, 64u, 4u)
        tmm2d_tg_step_devab(acc, unsafe(addr(a[mBase * kdim])), kdim, unsafe(addr(b[nBase * kdim])), kdim, 32u, 64u, kdim)
        tmm2d_tg_store(acc, unsafe(addr(dst[mBase * ndim + nBase])), 32u, 64u, ndim)
    }
}
