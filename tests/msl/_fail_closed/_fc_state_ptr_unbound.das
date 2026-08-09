// Fail-closed fixture: a pointer state member that is only READ. Its address space comes from
// what it is assigned from, so a member that is never bound to a kernel buffer cannot be declared
// at all — device and threadgroup are different pointer types in MSL.

expect 50501

options gen2

require metal/msl_shader

class FcStatePtrUnbound {
    @ssbo @binding = 0 dat : array<float>
    cur : float const?

    [metal_kernel(name="fc_state_ptr_unbound_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        dat[i] = unsafe(cur[0])
    }
}
