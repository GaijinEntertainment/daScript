// Fail-closed fixture: a [spirv_decode] method called directly takes its block the same way at
// every call - one call on the plane element and one on a copy of it is refused at discovery.
// `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins
require daslib/shader_lingua_franca

struct FcBlk {
    qs : int16[16]
}

class FcDecodeMixed {
    @ssbo @binding = 0 wq : array<FcBlk>
    @ssbo @binding = 1 y : array<float>

    [spirv_decode]
    def decode(blk : FcBlk; bc, cib : uint2) : float16 {
        return float16(float(int(blk.qs[int(cib.y & 15u)])))
    }

    [spirv_kernel(local_size_x = 64, name = "fc_decode_mixed_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        let blk = wq[gid >> 5u]
        y[gid] = float(decode(wq[gid >> 5u], uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
            + float(decode(blk, uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
    }
}
