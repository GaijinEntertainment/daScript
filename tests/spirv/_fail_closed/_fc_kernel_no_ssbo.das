// Fail-closed fixture ([spirv_kernel]): a kernel class with no @ssbo members must be rejected —
// a kernel with no buffers cannot observe or produce anything.
expect 50501

options gen2

require spirv/spirv_shader

struct FcArgs {
    n : uint
}

class FcNoSsbo {
    @push_constant pa : FcArgs
    @private_global scratch : uint

    [spirv_kernel(local_size_x=64, name="fckns_spv")]
    def fc {
        scratch = pa.n
    }
}
