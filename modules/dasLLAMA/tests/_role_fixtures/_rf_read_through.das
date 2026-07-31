options gen2
options indenting = 4

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: the same shape with the helper taking the buffer read-only — the classifier derives
// `read`, matching the declaration, so this compiles clean. (The compile-clean half of the
// whole-buffer rule; its twin _rf_write_through fails on the `var` parameter.)

var private g_rf_pso : MetalComputePipeline?

def private rf_pick(src : array<float>; i : uint) : float => src[i] + src[i]

[metal_dispatch(name = "enc_rf_read", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfReadThrough {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>

    [metal_kernel(name="rf_read_msl")]
    def rf_read {
        let i = gl_GlobalInvocationID.x
        dst[i] = rf_pick(src, i)
    }
}
