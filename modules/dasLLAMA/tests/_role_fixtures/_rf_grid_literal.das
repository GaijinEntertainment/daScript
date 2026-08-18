options gen2
options indenting = 4
options _dasllama_internal = true

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common
require metal/das_metal_boost

// Fixture: integer-literal grid dims fold to constants (the shared mk_grid_dim's superset
// grammar — metal folded only "1" before the micro-grammar hoist). Compile-clean is the gate;
// its twin _rf_grid_badtok proves a malformed token still fails loudly.

var private g_rf_gl_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_grid_literal", pso = "g_rf_gl_pso", tg = 64, grid = "4, n/4", params = "n : int64")]
class RfGridLiteral {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>

    [metal_kernel(name="rf_grid_literal_msl")]
    def rf_grid_literal {
        let i = gl_GlobalInvocationID.x
        dst[i] = src[i] * 2.0
    }
}
