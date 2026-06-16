// Fail-closed fixture: SPIR-V (without the recursion capability) forbids any function being part of
// a call cycle. A directly self-recursive shader function must be rejected cleanly. `expect 50501`
// makes lint skip the intentionally-failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<int>

def fib(n : int) : int {
    if (n < 2) {
        return n
    }
    return fib(n - 1) + fib(n - 2)
}

[compute_shader(local_size_x=64, name="fc_recursion_spv")]
def fc_recursion {
    let i = int(gl_GlobalInvocationID.x)
    data[i] = fib(data[i])
}
