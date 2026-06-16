// Fail-closed fixture: mutual recursion (a -> b -> a) is just as illegal as direct recursion and
// must be caught by the call-graph cycle check. `expect 50501` makes lint skip the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<int>

def ping(n : int) : int {
    if (n <= 0) {
        return 0
    }
    return pong(n - 1) + 1
}

def pong(n : int) : int {
    if (n <= 0) {
        return 0
    }
    return ping(n - 1) + 1
}

[compute_shader(local_size_x=64, name="fc_mutual_spv")]
def fc_mutual {
    let i = int(gl_GlobalInvocationID.x)
    data[i] = ping(data[i])
}
