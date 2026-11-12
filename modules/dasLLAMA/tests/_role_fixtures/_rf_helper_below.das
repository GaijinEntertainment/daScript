options gen2
options indenting = 4
options _dasllama_internal = true

expect 20800   // the lens refuses this fixture BY DESIGN — see uncategorized_no_aot_persistent_heap.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: a buffer handed WHOLE to a helper declared BELOW the class - the lens runs at the class, where the helper is not
// declared yet, so the direction cannot be read and the compile is refused naming where a helper goes.

var private g_rf_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_below", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfHelperBelow {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>

    [metal_kernel(name="rf_below_msl")]
    def rf_below {
        let i = gl_GlobalInvocationID.x
        rf_late_store(dst, i, src[i])
    }
}

def private rf_late_store(var dst : array<float>; i : uint; v : float) {
    dst[i] = v
}
