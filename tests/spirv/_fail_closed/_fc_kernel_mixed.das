// Fail-closed fixture ([spirv_kernel]): a member mixing two resource kinds must be rejected.
expect 50501

options gen2

require spirv/spirv_shader

class FcMixed {
    @ssbo @uniform @binding = 0 xs : array<uint>

    [spirv_kernel(local_size_x=64, name="fckm_spv")]
    def fc {
        let i = gl_GlobalInvocationID.x
        xs[i] = i
    }
}
