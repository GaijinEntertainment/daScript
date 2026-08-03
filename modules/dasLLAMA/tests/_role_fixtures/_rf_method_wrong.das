options gen2
options indenting = 4

expect 20800   // the lens refuses this fixture BY DESIGN — see test_metal_role_infer.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: the negative twin of _rf_method_chain — the override only READS qs, so a declared
// @role of "write" must be refused. It proves the derivation actually reaches through the
// method chain rather than merely failing to contradict anything.


var private g_rf_pso : MetalComputePipeline?

class RfWrongBase {
    @ssbo @binding = 1 @role = "read" xf : array<float>
    @ssbo @binding = 2 @role = "write" y : array<float>
    @uniform @binding = 3 n : uint

    def abstract decode(k : uint) : float

    def run {
        let i = gl_GlobalInvocationID.x
        var acc = xf[i]
        var k = 0u
        while (k < n) {
            acc += decode(k)
            k++
        }
        y[i] = acc
    }
}

[metal_dispatch(name = "enc_rf_wrong", pso = "g_rf_pso", tg = 64, grid = "rows", params = "rows : int64")]
class RfWrong : RfWrongBase {
    @ssbo @binding = 0 @role = "write" qs : array<uint>   // touched ONLY by the override

    def override decode(k : uint) : float => float(qs[k] & 15u)

    [metal_kernel(name="rf_wrong_msl")]
    def rf_wrong {
        run()
    }
}
