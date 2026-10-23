// Fail-closed fixture: coopmatScaleAcc is a workgroup-scope
// cooperative-matrix op, lowered only in a COMPUTE shader. A call
// from a fragment shader must be rejected at emit time with a clean
// diagnostic.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_sas_rs : array<float>
var @ssbo @binding = 1 fc_sas_cs : array<float>
var @out @location = 0 fc_sas_color : float4

[fragment_shader(name="fc_scaleacc_stage_spv")]
def fc_scaleacc_stage {
    var facc : coopmatWgAcc_f32_64x64
    var sacc : coopmatWgAcc_s32_64x64
    coopmatScaleAcc(facc, sacc, fc_sas_rs, 0u, fc_sas_cs, 0u)
    fc_sas_color = float4(1.0f, 0.0f, 0.0f, 1.0f)
}
