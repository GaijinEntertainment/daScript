// Fail-closed fixture (PR-G): @task_payload is a task/mesh-only storage class (TaskPayloadWorkgroupEXT).
// Declaring + referencing one from a fragment shader must be rejected cleanly by classify_global.
// `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct FcStagePayload {
    count : uint
}
var @task_payload fc_stage_payload : FcStagePayload
var @out @location = 0 fc_stage_out : float4
[fragment_shader(name="fc_payload_stage_spv")]
def fc_payload_stage {
    fc_stage_out = float4(float(fc_stage_payload.count), 0.0, 0.0, 1.0)
}
