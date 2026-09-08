// Fail-closed fixture: the index of a plane element passed to a [spirv_decode] method must be int
// or uint - the callee's entry chain takes a 32-bit index, so a 64-bit one is refused by name.
// `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins
require daslib/shader_lingua_franca

struct FcBlk4 {
    qs : int16[16]
}

class FcDecodeIndex64 {
    @ssbo @binding = 0 wq : array<FcBlk4>
    @ssbo @binding = 1 y : array<float>

    [spirv_decode]
    def decode(blk : FcBlk4; bc, cib : uint2) : float16 {
        return float16(float(int(blk.qs[int(cib.y & 15u)])))
    }

    [spirv_kernel(local_size_x = 64, name = "fc_decode_index64_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        y[gid] = float(decode(wq[uint64(gid >> 5u)], uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
    }
}
