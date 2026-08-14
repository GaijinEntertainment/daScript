options gen2
options indenting = 4

// Compiled by test_const_fold_under_lint with folding off, so every const here reaches the emitter
// as the arithmetic that was written rather than as a literal. Read by name from the test; the
// leading underscore keeps the directory scan from running it as a test of its own.

require spirv/spirv_shader

let FOLD_BASE = 3
let FOLD_SLOT = FOLD_BASE + 4
let FOLD_SET = FOLD_BASE - 2
let FOLD_LOCAL = FOLD_BASE * 16
let FOLD_SCALE = 2u * 3u
let FOLD_ALIAS : uint = FOLD_SCALE

var @ssbo @set = FOLD_SET @binding = FOLD_SLOT fold_data : array<uint>

[compute_shader(local_size_x=FOLD_LOCAL, name="fold_spv"), marker(no_coverage)]
def fold_shader {
    let i = gl_GlobalInvocationID.x
    fold_data[i] = i * FOLD_ALIAS
}
