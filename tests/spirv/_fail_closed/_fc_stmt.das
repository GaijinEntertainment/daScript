// Fail-closed fixture (Phase 6.4): an unsupported STATEMENT (memzero) must hit the visitor's
// explicit preVisit rejection — the case the value_of/ptr_of backstop alone can't catch (a void
// statement whose result no parent ever reads). `_`-prefix + `expect 50501` keep dastest/lint off.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="fc_stmt_spv")]
def fc_stmt {
    let i = gl_GlobalInvocationID.x
    var acc = data[i]
    memzero(acc)            // ExprMemZero statement -> explicit fail-closed rejection
    data[i] = acc
}
