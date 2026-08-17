options gen2
options _comment_hygiene = true
options indenting = 4

// Compute shaders authored in daslang and lowered to SPIR-V at compile time by dasSpirv's
// [compute_shader] macro (require spirv/spirv_shader). This replaces the previously committed
// examples/shaders/square.comp(.spv): the emitted blob is the public `square_spv : array<uint>`
// global, fed straight to create_shader_module(device, code : array<uint>).

module compute_shaders shared public

require spirv/spirv_shader
require spirv/spirv_builtins public

// SSBO data[] at set=0/binding=0 — the buffer the dispatch reads and writes.
var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="square_spv")]
def square {
    let i = gl_GlobalInvocationID.x
    data[i] = i * i
}
