// Fail-closed fixture: coopmatScaleAcc reads its row and column
// scales from @workgroup float arrays. An @ssbo scale array passes
// the generic stub and must be rejected at emit time.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_sass_rs : array<float>
var @workgroup fc_sass_cs : float[64]

[compute_shader(local_size_x=128, name="fc_scaleacc_shared_spv")]
def fc_scaleacc_shared {
    var facc : coopmatWgAcc_f32_64x64
    var sacc : coopmatWgAcc_s32_64x64
    coopmatScaleAcc(facc, sacc, fc_sass_rs, 0u, fc_sass_cs, 0u)
}
