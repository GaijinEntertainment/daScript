options gen2
options indenting = 4
options _dasllama_internal = true

expect 20800   // the lens refuses this fixture BY DESIGN — see uncategorized_no_aot_persistent_heap.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost
require _rf_helper_module

// Fixture: a buffer handed WHOLE to a `var` parameter of a required module's helper under a declared @role = "read" -
// the classifier derives `write` from that module's declaration, so the role is a compile error.

var private g_rf_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_modwrong", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfModuleWrong {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "read" dst : array<float>

    [metal_kernel(name="rf_modwrong_msl")]
    def rf_modwrong {
        let i = gl_GlobalInvocationID.x
        rf_mod_store(dst, i, src[i])
    }
}
