options gen2
options indenting = 4
options _dasllama_internal = true

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost
require _rf_helper_module
require _rf_helper_private   // nolint:STYLE030 - loads a private namesake, which has no vote

// Fixture: a buffer handed WHOLE to a helper of a REQUIRED module that takes it `var` - the classifier finds the helper
// outside the compiling module and derives `write`, matching the declared role.

var private g_rf_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_modthrough", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfModuleThrough {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>

    [metal_kernel(name="rf_modthrough_msl")]
    def rf_modthrough {
        let i = gl_GlobalInvocationID.x
        rf_mod_store(dst, i, src[i])
    }
}
