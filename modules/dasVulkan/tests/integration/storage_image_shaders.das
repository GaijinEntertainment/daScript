options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7 storage-image GPU gate shader: a compute shader writes a coordinate-derived gradient into a
// read-write storage image (image2D, Rgba8). The host reads it back and asserts EVERY pixel exactly
// (exact-analytic oracle) — the real-driver proof that imageStore + the coordinate mapping + the Rgba8
// format are correct, which spirv-val cannot check. readback pixel (x,y) = (x, y, 0, 255): R from
// gl_GlobalInvocationID.x, G from .y, B=0, A=1.0 (stored -> 255 through the unorm8 conversion). With the
// image <= 256 wide, x/255 round-trips to x through the unorm8 store.

module storage_image_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

var @binding = 0 grad_img : image2D

[compute_shader(local_size_x=8, local_size_y=8, name="grad_spv")]
def grad {
    let gid = gl_GlobalInvocationID
    let coord = int2(int(gid.x), int(gid.y))
    imageStore(grad_img, coord, float4(float(gid.x) / 255.0f, float(gid.y) / 255.0f, 0.0f, 1.0f))
}
