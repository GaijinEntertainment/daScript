// Fail-closed fixture: a `&` parameter bound to an lvalue shape a splice cannot alias by
// substitution. A pointer dereference is a legal das lvalue, but the splice binds argument TEXT —
// it needs a name it can write through (a variable, a member, an element, a lane).

expect 50501

options gen2

require metal/msl_shader

class FcSpliceRefArg {
    @ssbo @binding = 0 dat : array<float>

    def accum(var acc : float&; v : float) {
        acc += v
    }

    [metal_kernel(name="fc_splice_ref_arg_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        var sp = unsafe(addr(dat[i]))
        accum(unsafe(*sp), 2.0)
    }
}
