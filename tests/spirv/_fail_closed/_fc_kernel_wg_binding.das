// Fail-closed fixture ([spirv_kernel]): @binding on a @workgroup member must be rejected —
// threadgroup memory is not a bindable descriptor.
expect 50501

options gen2

require spirv/spirv_shader

class FcWgBind {
    @ssbo @binding = 0 xs : array<uint>
    @workgroup @binding = 1 tile : uint[64]

    [spirv_kernel(local_size_x=64, name="fckwb_spv")]
    def fc {
        let li = gl_LocalInvocationID.x
        tile[li] = li
        barrier()
        xs[gl_GlobalInvocationID.x] = tile[li]
    }
}
