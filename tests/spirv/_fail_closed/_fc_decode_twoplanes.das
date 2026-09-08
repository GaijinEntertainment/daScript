// Fail-closed fixture: a [spirv_decode] method called directly on plane elements takes them from
// ONE plane - the callee's entry chains through that plane, so elements of two planes are refused.
// `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins
require daslib/shader_lingua_franca

struct FcBlk2 {
    qs : int16[16]
}

class FcDecodeTwoPlanes {
    @ssbo @binding = 0 wq : array<FcBlk2>
    @ssbo @binding = 1 wq2 : array<FcBlk2>
    @ssbo @binding = 2 y : array<float>

    [spirv_decode]
    def decode(blk : FcBlk2; bc, cib : uint2) : float16 {
        return float16(float(int(blk.qs[int(cib.y & 15u)])))
    }

    [spirv_kernel(local_size_x = 64, name = "fc_decode_twoplanes_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        y[gid] = float(decode(wq[gid >> 5u], uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
            + float(decode(wq2[gid >> 5u], uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
    }
}
