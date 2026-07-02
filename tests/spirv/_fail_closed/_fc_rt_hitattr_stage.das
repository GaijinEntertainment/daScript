// Fail-closed fixture (RT): @hit_attribute (triangle barycentrics) only exists in a closest-hit
// shader; declaring + referencing one from a raygen shader must be rejected cleanly by
// classify_global. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @hit_attribute fc_rth_bary : float2
var @ray_payload fc_rth_payload : float3
[raygen_shader(name="fc_rt_hitattr_spv")]
def fc_rt_hitattr {
    fc_rth_payload = float3(fc_rth_bary, 0.0)
}
