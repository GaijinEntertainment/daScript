// Fail-closed fixture (PR-F): gl_PrimitiveID is a fragment-only Input builtin; reading it in a vertex
// shader must be rejected cleanly by the builtin stage check. `_`-prefix + `expect 50501` keep
// dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

[vertex_shader(name="fc_primid_spv")]
def fc_primid {
    gl_Position = float4(float(gl_PrimitiveID), 0.0, 0.0, 1.0)
}
