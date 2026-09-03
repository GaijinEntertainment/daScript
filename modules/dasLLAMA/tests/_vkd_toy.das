options gen2
options _dasllama_internal = true

module _vkd_toy shared public

require dasllama/dasllama_lint public
require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_vulkan_classes   // Q8Cm2T: the two toy stamps below
require daslib/typemacro_boost
require math   // the cloned template body's min/max

// The [vk_dispatch] bring-up fixture: a class kernel + its generated dispatch surface
// (ensure_vkd_toy / set_vkd_toy / enc_vkd_toy). The test drives the SAME method on the CPU
// as the oracle — the class body is ordinary daslang.

struct VtArgs {
    n : uint
    gain : float
}

[vk_dispatch(name = "vkd_toy", grid = "n/64", params = "n : int64")]
class VkdToy {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @push_constant pa : VtArgs

    [spirv_kernel(local_size_x = 64, name = "vkd_toy_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            ys[i] = xs[i] * pa.gain
        }
    }
}

// Two stamps of the served q8 cm2 l-tile, for the four-wide decode's device pair: the test
// ensures one with `g_gpu.decvec_on` off (its words stripped to the scalar callback) and one
// with it on (the twin served), and holds both to one CPU oracle. Stamps, so the served class's
// own pipeline is never rebuilt under a flag the test flips.
[vk_dispatch(name = "toy_q8_cm2_strip_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ToyQ8Cm2Strip : Q8Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}

[vk_dispatch(name = "toy_q8_cm2_twin_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ToyQ8Cm2Twin : Q8Cm2T {
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256
    typedef FLO = tensorLayout2D
}
