options gen2
options indenting = 4
options _dasllama_internal = true
options no_global_variables = false

module llvm_user_modules shared private

require llvm/daslib/llvm_code_selftest
require llvm/daslib/llvm_tune_fat_word
require ?dasllama/dasllama_gemm_gen dasllama/dasllama_gemm_register  // nolint:STYLE030,LINT019 — used inside the static_if below; on builds without the package both the require and the branch drop (path guard: pure-das package, nothing C++ to guard on)

//! User-extensible wiring for `[llvm_code]` generator modules. The JIT calls
//! `register_user_llvm_code_generators` (lazily, in the context that reads the registry)
//! before the first generator lookup. To plug in a generator module: add a
//! `require ?<impl-path> <registration>` line above — a PATH guard loads exactly when the
//! contributor's package is mounted (the guard's own file resolves), so this
//! module never hard-depends on the contributor. Pair each line with a `static_if
//! (typeinfo module_exists(<registration>))` around the registration call below - the
//! TARGET the guarded require loaded, which is what is visible from here; registered
//! generators are name-keyed and inert for programs whose `[llvm_code]` annotations never
//! name them.

var private g_registered = false

[macro_function]
def public register_user_llvm_code_generators {
    if (g_registered) {
        return
    }
    g_registered = true
    llvm_code_selftest::register_selftest()  // worked example + self-test generator (add_i32, add_plus_k)
    llvm_tune_fat_word::register_fat_word()  // the fat dispatch mask as an LLVM-side word
    static_if (typeinfo module_exists(dasllama_gemm_register)) {
        dasllama_gemm_register::register_dasllama_gemm_code_generators()
    }
}
