options gen2
options _comment_hygiene = true
options indenting = 4

// The compute kernel the compute examples share, authored in daslang
// and lowered to SPIR-V at compile time by dasSpirv's
// [compute_shader] macro: the blob is the public `square_spv`
// global, fed straight to create_shader_module - no GLSL, no glslang,
// no committed .spv. One copy, so the three examples differ only in
// how they bind `data`.

module compute_example_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

//! local_size_x of `square`; N elements dispatch as N / this groups
let public SQUARE_LOCAL_SIZE_X = 64

//! the storage buffer `square` writes: set 0, binding 0
var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="square_spv")]
def square {
    let i = gl_GlobalInvocationID.x
    data[i] = i * i
}
