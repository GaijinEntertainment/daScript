// Fail-closed fixture: sgmma_q8_blk2 reads its weights through a
// device byte view - a @workgroup array handed as the weights must
// name the mismatch instead of emitting a device pointer over
// threadgroup memory.

expect 50501

options gen2

require metal/msl_shader

class FcSgmmaWorkgroup {
    @ssbo @binding = 0 xa : array<float>
    @ssbo @binding = 1 out_ : array<float>
    @workgroup codes : int8[64]

    [metal_kernel(name="fc_sgmma_workgroup_msl")]
    def kern {
        let lane = gl_SubgroupInvocationID
        let p = sgmma_q8_blk2(codes, 0u, 32u, xa, 0u, 32u, lane)
        out_[lane] = p.x
    }
}
