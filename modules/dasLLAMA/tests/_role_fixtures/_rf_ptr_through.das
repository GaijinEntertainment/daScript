options gen2
options indenting = 4

require metal/msl_shader
require dasllama/dasllama_metal_lens
require dasllama/dasllama_metal_common   // hz_gate + the pso the generated builder binds
require metal/das_metal_boost

// Fixture: a POINTER into a tracked buffer handed to a helper — a pointer local for the read
// side, an inline addr() for the write side. Both take their direction from the callee's
// POINTEE const, so the declared roles here must be derived exactly.

var private g_rf_pso : MetalComputePipeline?

def private rf_ptr_sum(var p : float const?; n : uint) : float {
    var s = 0.0
    var k = 0u
    while (k < n) {
        s += unsafe(p[0])
        p = unsafe(p + 1)   // advancing, but still read-only — the POINTER's const says nothing
        k++
    }
    return s
}

def private rf_ptr_put(var o : float?; i : uint; v : float) {
    unsafe(o[i]) = v
}

[metal_dispatch(name = "enc_rf_ptr", pso = "g_rf_pso", tg = 64, grid = "n", params = "n : int64")]
class RfPtrThrough {
    @ssbo @binding = 0 @role = "read" src : array<float>
    @ssbo @binding = 1 @role = "write" dst : array<float>

    [metal_kernel(name="rf_ptr_msl")]
    def rf_ptr {
        let i = gl_GlobalInvocationID.x
        var sp = unsafe(addr(src[i * 4u]))
        rf_ptr_put(unsafe(addr(dst[0])), i, rf_ptr_sum(sp, 4u))
    }
}
