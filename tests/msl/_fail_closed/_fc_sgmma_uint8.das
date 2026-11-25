// Fail-closed fixture: sgmma_q8_blk2 decodes its weights as signed
// bytes - a uint8 plane handed as the weights must name the mismatch
// instead of emitting a kernel that reads the codes signed where the
// CPU body reads them unsigned.

expect 50501

options gen2

require metal/msl_shader

class FcSgmmaUint8 {
    @ssbo @binding = 0 xa : array<float>
    @ssbo @binding = 1 out_ : array<float>
    @ssbo @binding = 2 codes : array<uint8>

    [metal_kernel(name="fc_sgmma_uint8_msl")]
    def kern {
        let lane = gl_SubgroupInvocationID
        let p = sgmma_q8_blk2(codes, 0u, 32u, xa, 0u, 32u, lane)
        out_[lane] = p.x
    }
}
