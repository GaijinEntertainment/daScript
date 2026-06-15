// Fail-closed fixture (Phase 6.4 / round-2 review): unreachable code after a block terminator. SPIR-V
// forbids any instruction after a block terminator; the visitor can't skip the dead sibling subtree, so
// it must reject it cleanly (daslang only warns LINT001). `_`-prefix + `expect 50501` keep dastest/lint off.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_deadcode_spv")]
def fc_deadcode {
    let i = gl_GlobalInvocationID.x
    data[i] = i
    return
    data[i] = i * i         // unreachable: a statement after return in the same block
}
