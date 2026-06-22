// Fail-closed fixture: an image2D storage global with an unknown @format qualifier must be rejected
// cleanly by classify_global (the format parse fails -> errors non-empty, no silent blob). `_`-prefix
// + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

var @binding = 0 @format = "bogus" bad_img : image2D

[compute_shader(local_size_x=64, name="fc_imgfmt_spv")]
def fc_imgfmt {
    let i = gl_GlobalInvocationID.x
    imageStore(bad_img, int2(int(i), 0), float4(0.0f, 0.0f, 0.0f, 1.0f))
}
