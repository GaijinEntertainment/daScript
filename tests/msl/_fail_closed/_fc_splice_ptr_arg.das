// Fail-closed fixture: a pointer parameter bound to an argument that is not a pointer shape. A
// splice substitutes argument TEXT, so the argument has to name something a pointer can be
// derived from — a member, a pointer local, or an addr().

expect 50501

options gen2

require metal/msl_shader

class FcSplicePtrArg {
    @ssbo @binding = 0 dat : array<float>

    def head(p : float const?) : float => unsafe(p[0])

    [metal_kernel(name="fc_splice_ptr_arg_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = head(null)
    }
}
