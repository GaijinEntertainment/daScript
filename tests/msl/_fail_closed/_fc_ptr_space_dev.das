// Fail-closed fixture: the mirror direction — a pointer into @workgroup memory handed to an
// UNMARKED (device) parameter. Unmarked is device by design, so this must name the mismatch
// instead of silently emitting a device pointer over threadgroup memory.

expect 50501

options gen2

require metal/msl_shader

def sum_dev(p : float const?; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += unsafe(p[k])
        k++
    }
    return s
}

class FcPtrSpaceDev {
    @ssbo @binding = 0 dat : array<float>
    @ssbo @binding = 1 out_ : array<float>
    @workgroup tile : float[64]

    [metal_kernel(name="fc_ptr_space_dev_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        tile[gl_LocalInvocationID.x] = dat[i]
        barrier()
        out_[i] = sum_dev(unsafe(addr(tile[0])), 4u)
    }
}
