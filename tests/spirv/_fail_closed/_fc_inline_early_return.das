// Fail-closed fixture: a [spirv_inline] helper that returns before
// its last statement must be rejected - an inlined body falls
// through to the call's result, so only a single tail return lowers.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<float>

[spirv_inline]
def clamp_up(x : float) : float {
    if (x < 0.0f) {
        return 0.0f
    }
    return x
}

[compute_shader(local_size_x=64, name="fc_inline_early_return_spv")]
def fc_inline_early_return {
    let i = gl_GlobalInvocationID.x
    data[i] = clamp_up(data[i])
}
