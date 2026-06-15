// Fail-closed fixture (Phase 10.2): a multi-component swizzle used as an assignment target
// (`v.xy = …`) is not supported. The typer accepts it, so the emitter must reject it with a CLEAN
// message at ptr_of rather than the generic "internal: not an lvalue". `_`-prefix + `expect 50501`
// keep dastest/lint off.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @out @location = 0 fc_wswz_out : float4

[fragment_shader(name="fc_wswz_spv")]
def fc_wswz {
    fc_wswz_out.xy = float2(1.0, 0.0)   // write-swizzle -> clean fail-closed rejection
    fc_wswz_out = float4(0.0, 0.0, 0.0, 1.0)
}
