// Fail-closed fixture: a user shader function with an unsupported
// parameter type (a range - it has no SPIR-V value type) must be
// rejected at registration with a clear per-parameter message.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<float>

def scale(x : range) : float {
    return float(x.x)
}

[compute_shader(local_size_x=64, name="fc_badparam_spv")]
def fc_badparam {
    let i = gl_GlobalInvocationID.x
    data[i] = scale(range(1, 2))
}
