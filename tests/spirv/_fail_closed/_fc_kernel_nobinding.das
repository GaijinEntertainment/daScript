// Fail-closed fixture ([spirv_kernel]): an @ssbo member without an explicit @binding must be
// rejected — the class frontend never defaults a binding index silently.
expect 50501

options gen2

require spirv/spirv_shader

class FcNoBind {
    @ssbo xs : array<uint>

    [spirv_kernel(local_size_x=64, name="fcknb_spv")]
    def fc {
        let i = gl_GlobalInvocationID.x
        xs[i] = i
    }
}
