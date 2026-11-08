// Fail-closed fixture: GLSL forbids reading a `writeonly` image, and
// the NonReadable decoration says the same thing to the driver.
// imageLoad on a @writeonly storage image must be rejected at the
// call site rather than emitted as an OpImageRead against a
// NonReadable variable.  (imageSize is a query, not a read, and stays
// legal -- test_storage_image.das covers that half.) `_`-prefix +
// `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @binding = 0 @format = "rgba32f" @writeonly fc_wo_img : image3D
var @ssbo @binding = 1 fc_wo_out : array<float>

[compute_shader(local_size_x=64, name="fc_image_writeonly_load_spv")]
def fc_image_writeonly_load {
    let i = gl_GlobalInvocationID.x
    fc_wo_out[i] = imageLoad(fc_wo_img, int3(int(i), 0, 0)).x
}
