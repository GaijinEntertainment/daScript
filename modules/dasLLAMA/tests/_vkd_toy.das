options gen2
options _dasllama_internal = true

module _vkd_toy shared public

require dasllama/dasllama_lint public
require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_gpu_kernels_discrete
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

//! the one-count push constant the subgroup, mul-convert and q8 decode probes share
struct VdArgs {
    n : uint
}

//! every invocation writes the width its subgroup runs at: the tier's pipeline builder pins it where the device's own differs
[vk_dispatch(name = "vkd_subgroup", grid = "n/64", params = "n : int64")]
class VkdSubgroup {
    @ssbo @binding = 0 ws : array<uint>
    @push_constant pa : VdArgs

    [spirv_kernel(local_size_x = 64, name = "vkd_subgroup_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            ws[i] = gl_SubgroupSize
        }
    }
}

//! an f16 store of a product, the flash tile's `OUT16` store shape: a device that fuses the multiply and the convert rounds the
//! exact product once, one that rounds it to f32 first rounds twice - the two differ on a product that lands on an f16 midpoint
[vk_dispatch(name = "vkd_mulcvt", grid = "n/64", params = "n : int64")]
class VkdMulCvt {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 ys : array<float>
    @ssbo @binding = 2 hs : array<float16>
    @push_constant pa : VdArgs

    [spirv_kernel(local_size_x = 64, name = "vkd_mulcvt_spv")]
    def run {
        let i = gl_GlobalInvocationID.x
        if (i < pa.n) {
            hs[i] = float16(xs[i] * ys[i])
        }
    }
}

struct VdBlk {
    qs : int16[16]   //! 32 int8 quants as 16-bit lanes (the q8 block shape)
}

[vk_dispatch(name = "vkd_direct_decode", grid = "wgs", params = "wgs : int64")]
class VkdDirectDecode {
    @ssbo @binding = 0 wq : array<VdBlk>
    @ssbo @binding = 1 ws : array<float16>
    @ssbo @binding = 2 y : array<float>
    @push_constant pa : VdArgs

    [spirv_decode]
    def decode(blk : VdBlk; bc, cib : uint2) : float16 {
        let q = unpack8(blk.qs[int((cib.y & 30u) >> 1u)])[int(cib.y & 1u)]
        return ws[bc.y] * float16(float(int(q)))
    }

    [spirv_kernel(local_size_x = 64, name = "vkd_direct_decode_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        if (gid < pa.n) {
            y[gid] = float(decode(wq[gid >> 5u], uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
        }
    }
}

// Two stamps of the served q8 cm2 l-tile, for the four-wide decode's device pair: the test
// ensures one with `g_gpu.decvec_on` off (its words stripped to the scalar callback) and one
// with it on (the twin served), and holds both to one CPU oracle. Stamps, so the served class's
// own pipeline is never rebuilt under a flag the test flips.
[vk_dispatch(name = "toy_q8_cm2_strip_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ToyQ8Cm2Strip : Q8Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}

[vk_dispatch(name = "toy_q8_cm2_twin_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ToyQ8Cm2Twin : Q8Cm2T {
    typedef AT = coopmatWgA_f16_128x64
    typedef BT = coopmatWgB_f16_64x256
    typedef ACC = coopmatWgAcc_f16_128x256
    typedef ACCW = coopmatWgAcc_f32_128x256

    [spirv_kernel(local_size_x = 256)]
    def run {
        cm2_body()
    }
}
