// Fail-closed fixture (Phase 6.4): an unsupported LOCAL TYPE (a `double` local; only 32-bit
// int/uint/float scalars+vectors, bool, and fixed arrays are emittable) must be rejected cleanly
// by declare_local_var. `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_localtype_spv")]
def fc_localtype {
    let i = gl_GlobalInvocationID.x
    var d = 1.0lf           // double local -> unsupported scalar width
    d += 2.0lf
    data[i] = data[i] + uint(d)
}
