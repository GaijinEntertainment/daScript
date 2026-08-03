// Fail-closed fixture: a user function whose parameter type has no MSL form (string) must be
// rejected at signature emission — never a silently mistyped MSL parameter.

expect 50501

options gen2

require metal/msl_shader

def tagged(s : string; v : int) : int {
    return v + 1
}

class FcUfnParam {
    @ssbo @binding = 0 dat : array<int>

    [metal_kernel(name="fc_ufn_param_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = tagged("x", dat[i])
    }
}
