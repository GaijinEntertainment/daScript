// Fail-closed fixture: a @workgroup member bound to a thread-space `&` parameter crosses
// address spaces — the call site must reject it (pass the fixed array itself instead).

expect 50501

options gen2

require metal/msl_shader

def bump(var a : float&) {
    a += 1.0
}

class FcWgRef {
    @ssbo @binding = 0 dat : array<float>
    @workgroup acc : float

    [metal_kernel(name="fc_wg_ref_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        bump(acc)
        dat[i] = acc
    }
}
