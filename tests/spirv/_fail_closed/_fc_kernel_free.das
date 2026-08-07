// Fail-closed fixture ([spirv_kernel]): the annotation on a FREE FUNCTION must be rejected at
// apply — resources are the class's annotated members, so there is no class to scan.
expect 20800

options gen2

require spirv/spirv_shader

var @ssbo @binding = 0 data : array<uint>

[spirv_kernel(local_size_x=64, name="fckf_spv")]
def fc_kernel_free {
    let i = gl_GlobalInvocationID.x
    data[i] = i
}
