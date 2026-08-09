// Fail-closed fixture: a method used in VALUE position must be a single `return <expr>` — a
// multi-statement body has no expression to substitute into the call site.

expect 50501

options gen2

require metal/msl_shader

class FcMValBody {
    @ssbo @binding = 0 dat : array<float>

    def acc2(i : uint) : float {
        var s = dat[i]
        s += dat[i + 1u]
        return s
    }

    [metal_kernel(name="fc_mvalbody_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = acc2(i)
    }
}
