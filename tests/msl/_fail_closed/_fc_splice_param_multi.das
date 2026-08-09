// Fail-closed fixture: a COMPUTED argument bound to a parameter the value-position body names
// more than once. Substituting the argument text would evaluate it twice; value position has no
// place to hoist it, so the call must be refused.

expect 50501

options gen2

require metal/msl_shader

class FcSpliceMulti {
    @ssbo @binding = 0 a : array<float>
    @ssbo @binding = 1 b : array<float>
    @ssbo @binding = 2 c : array<float>

    def pick2(k : uint) : float => a[k] + b[k]

    [metal_kernel(name="fc_splice_multi_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        c[i] = pick2(i * 2u)
    }
}
