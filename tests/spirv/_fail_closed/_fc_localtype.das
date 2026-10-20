// Fail-closed fixture (Phase 6.4): an unsupported LOCAL TYPE (a
// `range` local - it has no SPIR-V value type) must be rejected
// cleanly by declare_local_var. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_localtype_spv")]
def fc_localtype {
    let i = gl_GlobalInvocationID.x
    var d = range(1, 3)     // range local -> no SPIR-V value type
    data[i] = data[i] + uint(d.y)
}
