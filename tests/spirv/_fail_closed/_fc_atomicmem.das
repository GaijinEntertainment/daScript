// Fail-closed fixture (Phase 10.5): an atomic on a NON-writable storage class (a @uniform member;
// only @workgroup and @ssbo memory are valid atomic targets) must be rejected cleanly by the atomic
// handler -- never emit an OpAtomic* on read-only Uniform storage and lean on spirv-val to catch it.
// `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct Ubo { scale : int }
var @uniform @binding = 0 ubo : Ubo
var @ssbo @binding = 1 data : array<int>

[compute_shader(local_size_x=64, name="fc_atomicmem_spv")]
def fc_atomicmem {
    let i = gl_GlobalInvocationID.x
    let o = atomicAdd(ubo.scale, 1)
    data[i] = o
}
