options gen2
options _dasllama_internal = true

module _vkd_toy shared public

require dasllama/dasllama_lint public
require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch

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
