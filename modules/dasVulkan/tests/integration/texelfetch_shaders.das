options gen2
options _comment_hygiene = true
options indenting = 4

// Phase-7.2 texelFetch GPU gate shader: a compute shader reads each texel of a sampled source texture
// by integer coordinate (texelFetch, unfiltered, lod 0) and writes it to a read-write storage image at
// the same coordinate. The host uploads a known source, reads the destination back, and asserts they
// match exactly -- the real-driver proof that texelFetch reads the right texel (spirv-val cannot check
// the coordinate semantics). texelFetch is derivative-free, so this runs in a compute shader.

module texelfetch_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

var @binding = 0 src_tex : sampler2D
var @binding = 1 dst_img : image2D

[compute_shader(local_size_x=8, local_size_y=8, name="fetch_copy_spv")]
def fetch_copy {
    let gid = gl_GlobalInvocationID
    let coord = int2(int(gid.x), int(gid.y))
    let texel = texelFetch(src_tex, coord, 0)
    imageStore(dst_img, coord, texel)
}
