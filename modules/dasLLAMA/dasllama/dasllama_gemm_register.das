options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemm_register shared private

require dasllama/dasllama_lint public
require dasllama/dasllama_gemm_gen

//! The `llvm_code_generator` group member: the package's descriptor joins the group, and
//! `llvm_user_modules` calls `register_llvm_code_generators` on every member, so dasLLVM
//! never names dasLLAMA.

[macro_function]
def public register_llvm_code_generators {
    register_dasllama_gemm_generators()
}
