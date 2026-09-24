options gen2
options indenting = 4
options _dasllama_internal = true

module _metal4_probe shared public

require ?das_metal metal/das_metal_boost
require ?das_metal dasllama/dasllama_metal_prefill

var private g_m4_probed = false
var private g_m4_ok = false

//! the tensor arms' skip gate, a capability probe run once: true when either of two independent Metal-4 tensor twin PSOs compiles
def metal4_tensor_available(dev) : bool {
    if (!g_m4_probed) {
        g_m4_probed = true
        var err : string
        var p1 = pipeline_from_source(dev, MetalQ8MulMmT_metal_q8_mulmm_t_msl, MetalQ8MulMmT_metal_q8_mulmm_t_msl_entry, MetalQ8MulMmT_metal_q8_mulmm_t_msl_fastmath, err)
        var p2 = pipeline_from_source(dev, MetalBf16MulMmT_metal_bf16_mulmm_t_msl, MetalBf16MulMmT_metal_bf16_mulmm_t_msl_entry, MetalBf16MulMmT_metal_bf16_mulmm_t_msl_fastmath, err)
        g_m4_ok = p1 != null || p2 != null
        metal_release(p1)
        metal_release(p2)
    }
    return g_m4_ok
}
