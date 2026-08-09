// Fail-closed fixture: a `var` POINTER parameter whose argument points into THREAD space. The
// hoisted copy has to name an address space, and das `T?` carries none — a thread local is
// neither device nor threadgroup, so there is nothing to declare.

expect 50501

options gen2

require metal/msl_shader

class FcSplicePtrTrace {
    @ssbo @binding = 0 dat : array<float>
    @ssbo @binding = 1 out_ : array<float>
    tot : float

    def walk(var p : float const?; n : uint) {
        var k = 0u
        while (k < n) {
            tot += unsafe(p[0])
            p = unsafe(p + 1)
            k++
        }
    }

    [metal_kernel(name="fc_splice_ptr_trace_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        var loc : float[4]
        loc[0] = dat[i]
        walk(unsafe(addr(loc[0])), 2u)
        out_[i] = tot
    }
}
