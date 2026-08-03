options gen2

module _vkd_toy shared public

require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require math

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

struct ArArgs {
    dim : uint
    add_on : uint
    eps : float
    ascale : float
}

// ar_add_rms_b's CLASS twin (phase-3 pilot): the same math with clean per-member bindings —
// x/a/w/y stop sharing packed buffers, eps/ascale ride the push struct instead of vk_xs slots.
// The reduce order is VERBATIM (stride-256 accum, subgroupAdd, sequential partials, 1/sqrt),
// so against the vk_add_rms seam on the same inputs the outputs are BIT-IDENTICAL.
[vk_dispatch(name = "cls_ar", grid = "nrows", params = "nrows : int64")]
class ClsArAddRms {
    @ssbo @binding = 0 xs : array<float>   // x rows, the residual stream — updated in place
    @ssbo @binding = 1 aa : array<float>   // the add partner rows
    @ssbo @binding = 2 wn : array<float>   // the norm weight row
    @ssbo @binding = 3 yo : array<float>   // the normed output rows
    @push_constant pa : ArArgs
    @workgroup row : float[4096]           // AR_MAX_DIM — staged so the post-reduce reread is local
    @workgroup part : float[64]            // one partial per subgroup (subgroupSize >= 4 floor)
    @workgroup inv1 : float[1]

    // the residual add + row stash + sum-of-squares pass over one row at `base`
    def accum_row(base : uint) : float {
        var ss = 0.0
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            let vv = pa.add_on != 0u ? (xs[base + k] + aa[base + k]) * pa.ascale : xs[base + k]
            xs[base + k] = vv
            row[k] = vv
            ss += vv * vv
            k += 256u
        }
        return ss
    }

    // workgroup-wide rms inverse from per-thread sum-of-squares partials
    def rms_inv(ss0 : float) : float {
        let ss = subgroupAdd(ss0)
        if (gl_SubgroupInvocationID == 0u) {
            part[gl_SubgroupID] = ss
        }
        barrier()
        if (gl_LocalInvocationID.x == 0u) {
            var tot = 0.0
            var sgi = 0u
            while (sgi < gl_NumSubgroups) {
                tot += part[sgi]
                sgi++
            }
            inv1[0u] = 1.0 / sqrt(tot / float(pa.dim) + pa.eps)   // the CPU rmsnorm's own form
        }
        barrier()
        return inv1[0u]
    }

    [spirv_kernel(local_size_x = 256, name = "cls_ar_spv")]
    def run {
        let base = gl_WorkGroupID.x * pa.dim
        let ss = accum_row(base)
        let inv = rms_inv(ss)
        var k = gl_LocalInvocationID.x
        while (k < pa.dim) {
            yo[base + k] = wn[k] * (row[k] * inv)
            k += 256u
        }
    }
}
