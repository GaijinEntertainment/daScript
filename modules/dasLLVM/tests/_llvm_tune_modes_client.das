options gen2

require daslib/tune

// The child fixture for the grid-mode test of llvm_tune.das (spawned
// with DAS_TUNE_MODE=test, -jit: main prints every grid variant's
// result); llvm_tune.das checks the same shape's normal-mode fallback
// contract in its own program. The variants registry exists in every
// mode - reference-row-only under normal - so the fixture compiles in
// either. The framework is reached through `daslib/tune` alone, so
// `module_exists(llvm_tune)` below is the witness answer: in a build
// without it the shells are inert, no registry exists, and main
// prints nothing.

// companion sibling — grid modes clone it per perm too (modes_kv__k0 ... ), so the child run
// proves per-perm args reach the companion generator and the declined row falls back with it
def modes_kv() : int => -1  // reference tier sentinel

[tune_perm(k = 0), tune_perm(k = 1), tune_perm(k = 2), tune_perm(suffix = "declined", decline = true),
 tune_companion(fn = "modes_kv", gen = "llvm_code_selftest::k_value"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1")]
def modes_add(a, b : int) : int => a + b

[export]
def main {
    static_if (typeinfo module_exists(llvm_tune)) {
        var n = 2  // nolint:LINT003,LINT019 — var defeats const-fold so the calls survive to codegen; the line sits in the llvm_tune static_if half, so a host without dasLLVM never compiles it
        var vs <- modes_add_variants()
        for (v in vs) {
            print("VARIANT {v._0} -> {invoke(v._1, n, 3)}\n")
        }
        delete vs
        var kvs <- modes_kv_variants()
        for (v in kvs) {
            print("KVARIANT {v._0} -> {invoke(v._1)}\n")
        }
        delete kvs
    }
}
