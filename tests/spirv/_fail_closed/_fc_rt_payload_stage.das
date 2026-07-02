// Fail-closed fixture (RT): @ray_payload is an RT-pipeline storage class (RayPayloadKHR); declaring +
// referencing one from a vertex shader must be rejected cleanly by classify_global. `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ray_payload fc_rtp_payload : float3
[vertex_shader(name="fc_rt_payload_spv")]
def fc_rt_payload {
    fc_rtp_payload = float3(0.0, 0.0, 0.0)
    gl_Position = float4(fc_rtp_payload, 1.0)
}
