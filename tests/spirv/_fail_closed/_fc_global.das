// Fail-closed fixture (Phase 6.4): an unsupported GLOBAL (a plain module variable, not a gl_*
// builtin / @ssbo / @uniform / @push_constant / sampler) must be rejected cleanly by
// classify_global. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>
var g_scale : uint = 2u

[compute_shader(local_size_x=64, name="fc_global_spv")]
def fc_global {
    let i = gl_GlobalInvocationID.x
    data[i] = data[i] * g_scale
}
