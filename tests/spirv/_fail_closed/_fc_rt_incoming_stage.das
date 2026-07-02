// Fail-closed fixture (RT): @incoming_ray_payload only exists where a payload arrives (miss /
// closest-hit); declaring + referencing one from a raygen shader must be rejected cleanly by
// classify_global. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @incoming_ray_payload fc_rti_prd : float3
[raygen_shader(name="fc_rt_incoming_spv")]
def fc_rt_incoming {
    fc_rti_prd = float3(0.0, 0.0, 0.0)
}
