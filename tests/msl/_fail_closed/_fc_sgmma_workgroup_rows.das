// Fail-closed fixture: sgmma_q8_blk2 reads its token rows through a
// device float view - a @workgroup array handed as the rows must name
// the mismatch instead of emitting a device pointer over threadgroup
// memory.

expect 50501

options gen2

require metal/msl_shader

class FcSgmmaWorkgroupRows {
    @ssbo @binding = 0 codes : array<int8>
    @ssbo @binding = 1 out_ : array<float>
    @workgroup xa : float[64]

    [metal_kernel(name="fc_sgmma_workgroup_rows_msl")]
    def kern {
        let lane = gl_SubgroupInvocationID
        let p = sgmma_q8_blk2(codes, 0u, 32u, xa, 0u, 32u, lane)
        out_[lane] = p.x
    }
}
