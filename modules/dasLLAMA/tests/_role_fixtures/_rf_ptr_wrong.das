options gen2
options indenting = 4
options _dasllama_internal = true

expect 20800   // the lens refuses this fixture BY DESIGN — see test_metal_role_infer.das

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: the pointer handed over has a MUTABLE pointee, so the callee may store through it
// and the buffer is written — the declared @role = "read" must be refused. Without the pointer
// rule this derived a conservative 'read' and matched, staging no hazard for a real write.

var private g_rf_pso : MetalComputePipeline?

def private rf_ptr_put(var o : float?; i : uint; v : float) {
    unsafe(o[i]) = v
}

[metal_dispatch(name = "enc_rf_ptrw", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfPtrWrong {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "read" dst : array<float>

    [metal_kernel(name="rf_ptrw_msl")]
    def rf_ptrw {
        let i = gl_GlobalInvocationID.x
        var dp = unsafe(addr(dst[0]))
        rf_ptr_put(dp, i, src[i])
    }
}
