// Fail-closed fixture (PR-G): a @task_payload field must be a scalar / vector / matrix / fixed array of
// those. A nested-struct field would need a recursive PLAIN struct (TaskPayloadWorkgroupEXT must NOT
// carry Block / Offset), so build_plain_struct rejects it cleanly for now. `_`-prefix + `expect 50501`
// keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct FcInner {
    x : float
}
struct FcNestedPayload {
    count : uint
    inner : FcInner       // nested-struct payload field -> rejected
}
var @task_payload fc_nested_payload : FcNestedPayload
[task_shader(local_size_x=1, name="fc_payload_nested_spv")]
def fc_payload_nested {
    fc_nested_payload.count = 1u
    EmitMeshTasksEXT(1u, 1u, 1u)
}
