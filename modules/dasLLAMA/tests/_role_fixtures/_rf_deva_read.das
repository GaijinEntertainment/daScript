options gen2
options indenting = 4
options _dasllama_internal = true

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common
require metal/das_metal_boost

// Fixture: the staged-GEMM deva step reads its device A pointer through an INLINE
// unsafe(addr(src[...])) - the production spelling. The classifier must derive the read.

var private g_rf_deva_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_deva", pso = "g_rf_deva_pso", tgmem = "rf_deva_msl_tgmem", tg = 128, grid = "m/32, n/64", params = "m : int64, n : int64")]
class RfDevaRead {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>
    @uniform @binding = 2 kdim : uint
    @uniform @binding = 3 ndim : uint
    @workgroup twb : float16[4096]

    [metal_kernel(name = "rf_deva_msl", float_a_ok=true)]
    def rf_deva {
        let mBase = gl_WorkGroupID.x * 32u
        let nBase = gl_WorkGroupID.y * 64u
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]
        var cp = unsafe(addr(dst[mBase * ndim + nBase]))
        tmm2d_tg_begin_deva(acc, unsafe(addr(src[0])), 32u, 64u, 4u, 64u)
        var kb = 0u
        while (kb * 32u < kdim) {
            twb[lid] = float16(0.0)
            barrier()
            tmm2d_tg_step_deva(acc, unsafe(addr(src[mBase * kdim + kb * 32u])), kdim, twb, 32u, 64u, 64u)
            barrier()
            kb += 2u
        }
        tmm2d_tg_store(acc, cp, 32u, 64u, ndim)
    }
}
