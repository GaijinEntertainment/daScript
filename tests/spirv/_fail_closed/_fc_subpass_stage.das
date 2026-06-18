// Fail-closed fixture: a subpassInput global in a non-fragment shader must be rejected cleanly.
// SubpassData images + the InputAttachment capability are fragment-only per the Vulkan spec, and
// spirv-val would flag a compute/vertex-stage usage; we surface the error at emit time with a
// clean message instead. The `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @set = 0 @binding = 0 @input_attachment_index = 0 fc_subpass : subpassInput
var @ssbo @binding = 1 out_data : array<float>

[compute_shader(local_size_x=64, name="fc_subpass_spv")]
def fc_subpass {
    let i = gl_GlobalInvocationID.x
    let sample = subpassLoad(fc_subpass)        // would compile if this were a fragment shader
    out_data[i] = sample.x
}
