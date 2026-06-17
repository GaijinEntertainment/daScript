// Fail-closed fixture: calling a builtin/host function the emitter does not lower (here `safe_acos`,
// a math builtin with no GLSL.std.450 mapping in the emitter) must be rejected cleanly — never a
// silent blob. User-defined function calls ARE supported now (see test_functions.das); this guards
// the "builtin not allowed" boundary. `expect 50501` makes lint skip the intentionally-failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins
require math

var @ssbo @binding = 0 data : array<float>

[compute_shader(local_size_x=64, name="fc_call_spv")]
def fc_call {
    let i = gl_GlobalInvocationID.x
    data[i] = safe_acos(data[i])
}
