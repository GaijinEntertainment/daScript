// Fail-closed fixture (RT): an accelerationStructureEXT descriptor is RT-pipeline-only (ray queries
// from other stages are a different extension, not yet supported); referencing one from a fragment
// shader must be rejected cleanly by classify_global. `_`-prefix + `expect 50501` keep dastest/lint
// off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @set = 0 @binding = 0 fc_rta_tlas : accelerationStructureEXT
var @ray_payload fc_rta_payload : float3
var @out @location = 0 fc_rta_out : float4
[fragment_shader(name="fc_rt_accel_spv")]
def fc_rt_accel {
    traceRayEXT(fc_rta_tlas, 0x01u, 0xFFu, 0u, 0u, 0u,
        float3(0.0, 0.0, -1.0), 0.001, float3(0.0, 0.0, 1.0), 100.0, fc_rta_payload)
    fc_rta_out = float4(1.0, 0.0, 0.0, 1.0)
}
