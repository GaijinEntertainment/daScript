// Fail-closed fixture: a pointer into a @ssbo buffer handed to a `@threadgroup` parameter.
// MSL has no conversion between address spaces, so the call site must reject it here rather
// than let the runtime MSL compile fail with a null pipeline.

expect 50501

options gen2

require metal/msl_shader

def sum_tg(@threadgroup p : float const?; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += unsafe(p[k])
        k++
    }
    return s
}

class FcPtrSpaceTg {
    @ssbo @binding = 0 dat : array<float>
    @ssbo @binding = 1 out_ : array<float>

    [metal_kernel(name="fc_ptr_space_tg_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        out_[i] = sum_tg(unsafe(addr(dat[0])), 4u)
    }
}
