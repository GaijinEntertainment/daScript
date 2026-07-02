// Fail-closed fixture (RT): traceRayEXT is an RT-pipeline intrinsic; calling it from a compute
// shader must be rejected cleanly by the intrinsic's stage gate (which fires before argument
// resolution, so the message names the stage, not a missing accel/payload). `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @set = 0 @binding = 0 fc_rtt_tlas : accelerationStructureEXT
var @ray_payload fc_rtt_payload : float3
[compute_shader(local_size_x=64, name="fc_rt_trace_spv")]
def fc_rt_trace {
    traceRayEXT(fc_rtt_tlas, 0x01u, 0xFFu, 0u, 0u, 0u,
        float3(0.0, 0.0, -1.0), 0.001, float3(0.0, 0.0, 1.0), 100.0, fc_rtt_payload)
}
