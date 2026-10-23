// Fail-closed fixture: coopmatScaleAcc scales an s32 accumulator into
// an f32 workgroup accumulator of the same shape. An f16 accumulator
// passes the generic stub and must be rejected at emit time.
// `expect 50501` keeps dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @workgroup fc_sash_rs : float[64]
var @workgroup fc_sash_cs : float[64]

[compute_shader(local_size_x=128, name="fc_scaleacc_shape_spv")]
def fc_scaleacc_shape {
    var facc : coopmatWgAcc_f16_64x64
    var sacc : coopmatWgAcc_s32_64x64
    coopmatScaleAcc(facc, sacc, fc_sash_rs, 0u, fc_sash_cs, 0u)
}
