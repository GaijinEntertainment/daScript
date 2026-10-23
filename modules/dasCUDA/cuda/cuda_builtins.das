options gen2
options indenting = 4

// The CUDA kernel vocabulary is the Vulkan one: the compute IDs, barriers, atomics and the
// subgroup set a `[spirv_kernel]` body speaks, so a kernel class the Vulkan home stamps stamps
// here unchanged. Every re-exported declaration is an AST marker the emitter lowers by NAME; the CPU stub
// bodies keep the same method running on the CPU as the correctness oracle (a width-1 subgroup:
// the reductions and shuffles are identities).

module cuda_builtins shared public

require spirv/spirv_builtins public
