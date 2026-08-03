// Fail-closed fixture: a pointer whose provenance is a THREAD-space local. The emitter derives
// the address space from what the argument points into, and a thread local is neither device nor
// threadgroup — with nothing to derive, the call must be refused rather than guessed.

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

class FcPtrUntraceable {
    @ssbo @binding = 0 dat : array<float>
    @ssbo @binding = 1 out_ : array<float>

    [metal_kernel(name="fc_ptr_untraceable_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        var loc : float[4]
        loc[0] = dat[i]
        out_[i] = sum_dev(unsafe(addr(loc[0])), 4u)
    }
}
