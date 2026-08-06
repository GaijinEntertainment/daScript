options gen2

module _vkd_toy shared public

require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_kqformat

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

// old-side pipeline picks for the parity arms — they live HERE because they name vulkan types,
// and this fixture (require ?vulkan from the test) drops out whole when dasVulkan is absent
def cm_pipe_for_test(v : int) : Pipeline {
    if (v == 0) {
        return weak_copy(g_gpu.batch_coopmat_f16)
    } elif (v == 1) {
        return weak_copy(g_gpu.batch_coopmat_i8)
    } elif (v == 2) {
        return weak_copy(g_gpu.batch_coopmat_mm)
    } elif (v == 3) {
        return weak_copy(g_gpu.batch_coopmat_mm_a)
    }
    return weak_copy(g_gpu.batch_coopmat_mm_m)
}

def gemv_pipe_batch_for_test(fmt : int) : Pipeline {
    if (fmt == int(KqFmt.k4)) {
        return weak_copy(g_gpu.batch_k4)
    } elif (fmt == int(KqFmt.k5)) {
        return weak_copy(g_gpu.batch_k5)
    } elif (fmt == int(KqFmt.k6)) {
        return weak_copy(g_gpu.batch_k6)
    }
    return weak_copy(g_gpu.batch_q40)
}
