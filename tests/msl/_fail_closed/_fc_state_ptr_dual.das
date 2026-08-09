// Fail-closed fixture: a pointer state member bound from TWO different buffers. One root buffer
// per pointer member — the root decides the address space and the write-set entry, and a member
// that names two of them has neither.

expect 50501

options gen2

require metal/msl_shader

class FcStatePtrDual {
    @ssbo @binding = 0 xs : array<float>
    @ssbo @binding = 1 zs : array<float>
    @ssbo @binding = 2 ys : array<float>
    cur : float const?

    def seed_x(i : uint) {
        cur = unsafe(addr(xs[i]))
    }

    def seed_z(i : uint) {
        cur = unsafe(addr(zs[i]))
    }

    [metal_kernel(name="fc_state_ptr_dual_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        seed_x(i)
        seed_z(i)
        ys[i] = unsafe(cur[0])
    }
}
