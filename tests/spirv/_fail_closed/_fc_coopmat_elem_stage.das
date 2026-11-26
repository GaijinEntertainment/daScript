// Fail-closed fixture: coopmatElem reads one element of a
// subgroup-scoped accumulator, lowered only in a COMPUTE shader. A
// call from a fragment shader must be rejected at emit time with a
// clean diagnostic.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @out @location = 0 fc_ces_color : float4

[fragment_shader(name="fc_coopmat_elem_stage_spv")]
def fc_coopmat_elem_stage {
    var acc : coopmatAcc_s32_16x16
    let e = coopmatElem(acc, 0u)
    fc_ces_color = float4(float(e), 0.0f, 0.0f, 1.0f)
}
