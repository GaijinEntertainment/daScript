options gen2
options indenting = 4
options _dasllama_internal = true

expect 20800   // the lens refuses this fixture BY DESIGN — see test_metal_role_infer.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: a buffer handed WHOLE to a helper that takes it `var` — the classifier must derive
// `write` from the parameter, so the declared @role = "read" is a compile error.

var private g_rf_pso : MetalComputePipeline?

def private rf_store(var dst : array<float>; i : uint; v : float) {
    dst[i] = v
}

[metal_dispatch(name = "enc_rf_write", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfWriteThrough {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "read" dst : array<float>

    [metal_kernel(name="rf_write_msl")]
    def rf_write {
        let i = gl_GlobalInvocationID.x
        rf_store(dst, i, src[i])
    }
}
