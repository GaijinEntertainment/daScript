// Fail-closed fixture: a user shader function with an unsupported parameter type (double — not a
// 32-bit scalar/vector/matrix) must be rejected at registration with a clear per-parameter message.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<float>

def scale(x : double) : float {
    return float(x)
}

[compute_shader(local_size_x=64, name="fc_badparam_spv")]
def fc_badparam {
    let i = gl_GlobalInvocationID.x
    data[i] = scale(2.0lf)
}
