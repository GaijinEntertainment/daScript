// Fail-closed fixture: a `var` POINTER parameter in a VALUE-position splice. das passes a pointer
// by value, so the callee walks its own copy — that copy is a hoisted local, and value position
// cannot hoist.

expect 50501

options gen2

require metal/msl_shader

class FcSplicePtrValue {
    @ssbo @binding = 0 dat : array<float>
    @ssbo @binding = 1 out_ : array<float>

    def head(var p : float const?) : float => unsafe(p[0])

    [metal_kernel(name="fc_splice_ptr_value_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        var sp = unsafe(addr(dat[i]))
        out_[i] = head(sp)
    }
}
