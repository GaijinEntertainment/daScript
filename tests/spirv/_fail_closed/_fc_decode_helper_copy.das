// Fail-closed fixture: a [spirv_decode] method the kernel calls on plane elements is called on a
// copy from a helper the kernel reaches - the element form is fixed per method, so the copy is
// refused. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins
require daslib/shader_lingua_franca

struct FcBlk3 {
    qs : int16[16]
}

class FcDecodeHelperCopy {
    @ssbo @binding = 0 wq : array<FcBlk3>
    @ssbo @binding = 1 y : array<float>

    [spirv_decode]
    def decode(blk : FcBlk3; bc, cib : uint2) : float16 {
        return float16(float(int(blk.qs[int(cib.y & 15u)])))
    }

    def first_lane(gid : uint) : float {
        let blk = wq[0]
        return float(decode(blk, uint2(0u, 0u), uint2(0u, gid & 31u)))
    }

    [spirv_kernel(local_size_x = 64, name = "fc_decode_helper_copy_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        y[gid] = float(decode(wq[gid >> 5u], uint2(0u, gid >> 5u), uint2(0u, gid & 31u))) + first_lane(gid)
    }
}
