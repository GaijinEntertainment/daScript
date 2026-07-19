options gen2
options _comment_hygiene = true

module dasllama_gemm_register shared private

require dasllama/dasllama_gemm_gen

// The require?-target half of the generator wiring (gemm_generator_plan.md part 1):
// llvm_user_modules does `require ?dasllama/dasllama_gemm_gen dasllama/dasllama_gemm_register`,
// a PATH guard — this file (and through it the generator module) is pulled into the llvm-jit
// context exactly when the dasllama package is mounted (the guard's own file resolves).
// dasLLVM itself never hard-depends on dasLLAMA.

[macro_function]
def public register_dasllama_gemm_code_generators {
    register_dasllama_gemm_generators()
}
