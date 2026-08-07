// Fail-closed fixture ([spirv_kernel]): a data member with NO resource-kind annotation must be
// rejected by the member scan — every member declares what it is.
expect 50501

options gen2

require spirv/spirv_shader

class FcPlain {
    @ssbo @binding = 0 xs : array<uint>
    stray : int

    [spirv_kernel(local_size_x=64, name="fckp_spv")]
    def fc {
        let i = gl_GlobalInvocationID.x
        xs[i] = i
    }
}
