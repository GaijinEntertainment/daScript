// Fail-closed fixture: a fixed-array local initializer with a non-literal element. Only an
// all-literal `let` hoists to a program-scope constant table; anything else has no MSL form.

expect 50501

options gen2

require metal/msl_shader

class FcArrayInitNonconst {
    @ssbo @binding = 0 src : array<uint>
    @ssbo @binding = 1 dst : array<uint>

    [metal_kernel(name="fc_array_init_nonconst_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        let tbl = fixed_array<uint>(1u, src[i], 3u, 4u)
        dst[i] = tbl[i & 3u]
    }
}
