// Fail-closed fixture ([spirv_kernel]): @set != 0 must be rejected — a kernel class is one
// descriptor set (the generated per-class layout is set 0).
expect 50501

options gen2

require spirv/spirv_shader

class FcSet {
    @ssbo @set = 1 @binding = 0 xs : array<uint>

    [spirv_kernel(local_size_x=64, name="fcks_spv")]
    def fc {
        let i = gl_GlobalInvocationID.x
        xs[i] = i
    }
}
