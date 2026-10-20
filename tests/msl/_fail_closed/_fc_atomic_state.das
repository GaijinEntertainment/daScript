// Fail-closed fixture: an atomic add whose target is an element of a
// state member - thread-private storage, no atomic form - not of an
// @ssbo or @workgroup member.

expect 50501

options gen2

require metal/msl_shader

class FcAtomicState {
    @ssbo @binding = 0 src : array<uint>
    @ssbo @binding = 1 dst : array<uint>
    tmp : uint[4]

    [metal_kernel(name="fc_atomic_state_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        tmp[0] = src[i]
        let _o = atomicAdd(tmp[0], 1u)
        dst[i] = tmp[0]
    }
}
