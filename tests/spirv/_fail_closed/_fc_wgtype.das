// Fail-closed fixture (Phase 10.5): an unsupported @workgroup shared TYPE (a struct; only scalars,
// vectors, matrices, and fixed arrays of those are emittable in Workgroup storage) must be rejected
// cleanly by classify_global -- never reach emit_type's panic. `_`-prefix + `expect 50501` keep
// dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>
struct Foo { a : int; b : int }
var @workgroup wgfoo : Foo

[compute_shader(local_size_x=64, name="fc_wgtype_spv")]
def fc_wgtype {
    let i = gl_GlobalInvocationID.x
    data[i] = uint(wgfoo.a)
}
