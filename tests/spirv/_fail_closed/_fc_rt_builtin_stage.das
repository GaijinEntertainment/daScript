// Fail-closed fixture (RT): gl_LaunchIDEXT is an RT-pipeline builtin (raygen / miss / closest-hit);
// reading it in a compute shader must be rejected cleanly by the builtin stage check. `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @ssbo @binding = 0 fc_rtb_out : array<uint>
[compute_shader(local_size_x=64, name="fc_rt_builtin_spv")]
def fc_rt_builtin {
    fc_rtb_out[0] = gl_LaunchIDEXT.x
}
