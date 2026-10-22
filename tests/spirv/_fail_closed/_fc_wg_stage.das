// Fail-closed fixture: @workgroup memory exists only in workgroup-
// dispatched stages (compute / mesh / task). A fragment shader that
// declares one is rejected with a clean diagnostic.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @workgroup fc_wgs_val : float[4]
var @out @location = 0 fc_wgs_color : float4

[fragment_shader(name="fc_wg_stage_spv")]
def fc_wg_stage {
    fc_wgs_color = float4(fc_wgs_val[0], 0.0f, 0.0f, 1.0f)
}
