// Fail-closed fixture: a method parameter whose type has no MSL form (a plain das struct — only
// scalars, their vectors, buffers and pointers cross a splice boundary).

expect 50501

options gen2

require metal/msl_shader

struct FcPair {
    x : float
    y : float
}

class FcSpliceParamType {
    @ssbo @binding = 0 dat : array<float>

    def dot2(p : FcPair; i : uint) : float => (p.x * p.y) + dat[i]

    [metal_kernel(name="fc_splice_param_type_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = dot2(FcPair(x = 1.0, y = 2.0), i)
    }
}
