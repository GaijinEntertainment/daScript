// Fail-closed fixture: a [spirv_inline] [spirv_decode] method called
// on a plane element must be rejected - the element's index binds
// only to a called OpFunction's parameter.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct FcBlk {
    d : float16
    q : float16
}

class FcElemCls {
    @ssbo @binding = 0 wq : array<FcBlk>
    @ssbo @binding = 1 y : array<float>

    [spirv_decode, spirv_inline]
    def decode(blk : FcBlk; bc, cib : uint2) : float16 {
        return blk.d * blk.q
    }

    [spirv_kernel(local_size_x = 64, name = "fc_inline_plane_elem_spv")]
    def run {
        let gid = gl_GlobalInvocationID.x
        y[gid] = float(decode(wq[gid >> 5u], uint2(0u, gid >> 5u), uint2(0u, gid & 31u)))
    }
}
