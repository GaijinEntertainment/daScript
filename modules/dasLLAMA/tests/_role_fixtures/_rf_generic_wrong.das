options gen2
options indenting = 4

expect 20800   // the lens refuses this fixture BY DESIGN — see test_metal_role_infer.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: the helper is a GENERIC. Post-infer the call carries the mangled instance name
// (`module`fn`hash`), so resolving the callee by that name finds nothing — the classifier used
// to give up, and giving up means the declared roles stand UNVERIFIED. Here the declaration is
// wrong, so an unverified pass is a silent hazard-staging hole; the refusal is the gate.

var private g_rf_pso : MetalComputePipeline?

def private rf_gen_store(var dst : array<auto(TT)>; i : uint; v : TT) {
    dst[i] = v
}

[metal_dispatch(name = "enc_rf_gen", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfGenericWrong {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "read" dst : array<float>

    [metal_kernel(name="rf_gen_msl")]
    def rf_gen {
        let i = gl_GlobalInvocationID.x
        rf_gen_store(dst, i, src[i])
    }
}
