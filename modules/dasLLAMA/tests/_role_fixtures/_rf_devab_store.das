options gen2
options indenting = 4
options _dasllama_internal = true

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common
require metal/das_metal_boost

// Fixture: the device-pair step reads BOTH operands through inline unsafe(addr(buf[...])),
// and the store writes its C through one. The classifier must derive two reads and a write.

var private g_rf_devab_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_devab", pso = "g_rf_devab_pso", tg = 128, grid = "m/32, n/64", params = "m : int64, n : int64")]
class RfDevabStore {
    @ssbo @binding = 0 @role = "read" a : array<float16>
    @ssbo @binding = 1 @role = "read" b : array<float16>
    @ssbo @binding = 2 @role = "write" dst : array<float>
    @uniform @binding = 3 kdim : uint
    @uniform @binding = 4 ndim : uint

    [metal_kernel(name = "rf_devab_msl")]
    def rf_devab {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        var acc : float[2048]
        tmm2d_tg_begin_devab(acc, 32u, 64u, 4u)
        tmm2d_tg_step_devab(acc, unsafe(addr(a[mBase * kdim])), kdim, unsafe(addr(b[nBase * kdim])), kdim, 32u, 64u, kdim)
        tmm2d_tg_store(acc, unsafe(addr(dst[mBase * ndim + nBase])), 32u, 64u, ndim)
    }
}
