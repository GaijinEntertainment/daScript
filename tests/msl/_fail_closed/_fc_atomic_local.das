// Fail-closed fixture: an atomic add whose target is a local, not
// an element of an @ssbo or @workgroup member. MSL atomics live
// in device or threadgroup memory; a thread-private value has no
// atomic form.


expect 50501

options gen2

require metal/msl_shader

class FcAtomicLocal {
    @ssbo @binding = 0 src : array<uint>
    @ssbo @binding = 1 dst : array<uint>

    [metal_kernel(name="fc_atomic_local_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        var acc = src[i]
        let _o = atomicAdd(acc, 1u)
        dst[i] = acc
    }
}
