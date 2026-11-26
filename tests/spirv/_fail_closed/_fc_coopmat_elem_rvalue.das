// Fail-closed fixture: coopmatElem reads an accumulator LOCAL. A
// product handed to it as a value names no local, and must be
// rejected at emit time with a clean diagnostic.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_cer_a : array<int8>
var @ssbo @binding = 1 fc_cer_b : array<int8>
var @ssbo @binding = 2 fc_cer_c : array<int>

[compute_shader(local_size_x=32, name="fc_coopmat_elem_rvalue_spv")]
def fc_coopmat_elem_rvalue {
    var a : coopmatA_s8_16x16
    var b : coopmatB_s8_16x16
    var acc : coopmatAcc_s32_16x16
    coopmatLoad(a, fc_cer_a, 0, 16, 0)
    coopmatLoad(b, fc_cer_b, 0, 16, 1)
    fc_cer_c[gl_LocalInvocationID.x] = coopmatElem(coopmatMulAdd(a, b, acc), 0u)
}
