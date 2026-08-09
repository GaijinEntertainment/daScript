// Fail-closed fixture: a pointer state member with an initializer. The decl is always
// `= nullptr` — the address space (and the root buffer) resolve from the ASSIGNMENT sites, so an
// initializer would silently do nothing.

expect 50501

options gen2

require metal/msl_shader

class FcStatePtrInit {
    @ssbo @binding = 0 dat : array<float>
    cur : float const? = null

    [metal_kernel(name="fc_state_ptr_init_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        cur = unsafe(addr(dat[i]))
        dat[i] = unsafe(cur[0])
    }
}
