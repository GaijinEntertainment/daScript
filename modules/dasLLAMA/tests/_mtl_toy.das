options gen2
options indenting = 4
options _dasllama_internal = true

module _mtl_toy shared public

require dasllama/dasllama_lint public
require das_metal
require metal/msl_shader
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_lens

// The [metal_dispatch] multi-kernel bring-up fixture (_vkd_toy's sibling): ONE class, two
// [metal_kernel] methods, two lens instances picking per kernel=. Each generated builder binds
// the full field set but stages only ITS kernel's derived access — kshift never touches xs, so
// its builder must not stage it; ys diverges write vs readwrite between the two. The declared
// @role on xs exercises the multi cover rule (read covers read AND unused-here). The test
// drives the same methods on the CPU as the oracle.

struct MtToyArgs {
    n : uint
    gain : float
}

[metal_dispatch(name = "enc_mtl_toy_scale", pso = "g_mtl_toy_scale_pso", kernel = "kscale", tg = 64, grid = "n/64", params = "n : int64"),
 metal_dispatch(name = "enc_mtl_toy_shift", pso = "g_mtl_toy_shift_pso", kernel = "kshift", tg = 64, grid = "n/64", params = "n : int64")]
class MtlToy {
    @ssbo @binding = 0 @role = "read" xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @uniform @binding = 2 ka : MtToyArgs

    [metal_kernel(name = "mtl_toy_scale_msl")]
    def kscale {
        let i = gl_GlobalInvocationID.x
        if (i < ka.n) {
            ys[i] = xs[i] * ka.gain
        }
    }

    [metal_kernel(name = "mtl_toy_shift_msl")]
    def kshift {
        let i = gl_GlobalInvocationID.x
        if (i < ka.n) {
            ys[i] = ys[i] + ka.gain
        }
    }
}

// the generated builders reference these by name (kn_pipeline's first bind); the test fills
// them from the emitted mtl_toy_*_msl sources before dispatching
var g_mtl_toy_scale_pso : MetalComputePipeline?
var g_mtl_toy_shift_pso : MetalComputePipeline?
