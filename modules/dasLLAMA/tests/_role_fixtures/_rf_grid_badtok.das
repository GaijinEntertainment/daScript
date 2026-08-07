options gen2
options indenting = 4

expect 30838   // the "4l" grid token falls to the name path and is an unresolved variable BY DESIGN

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common
require metal/das_metal_boost

// Fixture: a malformed grid token ("4l" — not all digits) falls through to the name path and
// must FAIL the compile as an unresolved variable, never silently truncate through to_int.

var private g_rf_gb_pso : MetalComputePipeline?

[metal_dispatch(name = "enc_rf_grid_badtok", pso = "g_rf_gb_pso", tg = 64, grid = "4l", params = "n : int64")]
class RfGridBadtok {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>

    [metal_kernel(name="rf_grid_badtok_msl")]
    def rf_grid_badtok {
        let i = gl_GlobalInvocationID.x
        dst[i] = src[i] * 2.0
    }
}
