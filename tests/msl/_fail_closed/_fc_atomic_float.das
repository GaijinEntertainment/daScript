// Fail-closed fixture: an atomic add on a float element. The
// SPIR-V builtins declare the float form; the MSL emitter lowers
// int and uint elements only and refuses the rest by name.


expect 50501

options gen2

require metal/msl_shader
require spirv/spirv_builtins

class FcAtomicFloat {
    @ssbo @binding = 0 dst : array<float>

    [metal_kernel(name="fc_atomic_float_msl")]
    def kern {
        let i = gl_GlobalInvocationID.x
        let _o = atomicAdd(dst[i], 1.0)
    }
}
