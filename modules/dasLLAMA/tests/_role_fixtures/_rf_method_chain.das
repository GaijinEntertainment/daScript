options gen2
options indenting = 4

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: the family shape — a base owns the loop, each format overrides one step, and the
// kernel body only delegates. The classifier must follow the method chain (base AND the
// concrete class's override) to derive the roles; without that it sees an empty access set and
// the lens stages no hazards at all.

var private g_rf_pso : MetalComputePipeline?

class RfBase {
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

[metal_dispatch(name = "enc_rf_chain", pso = "g_rf_pso", tg = 64, grid = "rows", params = "rows : int64")]
class RfChain : RfBase {
    @ssbo @binding = 0 @role = "read" qs : array<uint>   // touched ONLY by the override

    def override decode(k : uint) : float => float(qs[k] & 15u)

    [metal_kernel(name="rf_chain_msl")]
    def rf_chain {
        run()
    }
}
