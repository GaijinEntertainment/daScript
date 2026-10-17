options gen2
module texture_grad_shaders shared public
require spirv/spirv_shader
require spirv/spirv_builtins public
require math
var @binding = 0 grad_source : sampler2D
var @binding = 1 grad_destination : image2D
[compute_shader(local_size_x=8, local_size_y=8, name="grad_copy_spv")]
def grad_copy {
    let gid = gl_GlobalInvocationID
    let scale = exp2(float(gid.x / 16u)) / 64.0
    var dx = float2(0.0)
    var dy = float2(0.0)
    if (gid.y < 32u) { dx = float2(scale, 0.0) }
    else { dy = float2(0.0, scale) }
    imageStore(grad_destination, int2(int(gid.x), int(gid.y)),
        textureGrad(grad_source, float2(0.5), dx, dy))
}
