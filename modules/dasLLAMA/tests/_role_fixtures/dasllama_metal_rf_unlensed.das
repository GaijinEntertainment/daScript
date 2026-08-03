options gen2
options indenting = 4

expect 20800   // the census refuses this fixture BY DESIGN — see test_metal_role_infer.das

module dasllama_metal_rf_unlensed   // the census keys on the module name: dasllama*metal*

require metal/msl_shader
require dasllama/dasllama_metal_lens

// Fixture for the UN-LENSED half of the ratchet (the lint-time manual-dispatch census). An
// un-lensed class's declared roles are the hand-written builder's contract, cross-checked
// against the same body derivation. The helper here is a GENERIC, so post-infer the call
// carries its mangled instance name — resolving the callee by that name finds nothing, and a
// classifier that gives up lets a WRONG declaration through unverified.

def private rfu_store(var dst : array<auto(TT)>; i : uint; v : TT) {
    dst[i] = v
}

class RfUnlensedWrong {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "read" dst : array<float>

    [metal_kernel(name="rfu_msl")]
    def rfu {
        let i = gl_GlobalInvocationID.x
        rfu_store(dst, i, src[i])
    }
}
