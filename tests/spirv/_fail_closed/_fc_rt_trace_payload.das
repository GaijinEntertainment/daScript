// Fail-closed fixture (RT): OpTraceRayKHR's payload operand must name a @ray_payload /
// @incoming_ray_payload VARIABLE (SPIR-V takes the pointer, not a value); handing traceRayEXT a
// plain local must be rejected cleanly by the intrinsic's payload resolution. `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @set = 0 @binding = 0 fc_rtl_tlas : accelerationStructureEXT
[raygen_shader(name="fc_rt_trace_payload_spv")]
def fc_rt_trace_payload {
    var local_payload = float3(0.0, 0.0, 0.0)
    traceRayEXT(fc_rtl_tlas, 0x01u, 0xFFu, 0u, 0u, 0u,
        float3(0.0, 0.0, -1.0), 0.001, float3(0.0, 0.0, 1.0), 100.0, local_payload)
}
