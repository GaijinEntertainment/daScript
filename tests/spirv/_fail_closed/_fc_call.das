// Fail-closed fixture (Phase 6.4): an unsupported CALL (a plain user function, not a vector
// constructor / dot / GLSL.std.450 builtin) must be rejected cleanly by the emitter — never
// silently dropped. `_`-prefix keeps dastest from auto-running it; `expect 50501`
// (runtime_function_annotation) makes lint skip the intentionally-failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

def helper(x : uint) : uint {
    return x + 1u
}

[compute_shader(local_size_x=64, name="fc_call_spv")]
def fc_call {
    let i = gl_GlobalInvocationID.x
    data[i] = helper(data[i])
}
