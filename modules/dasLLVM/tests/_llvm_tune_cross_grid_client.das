options gen2

require daslib/tune
require daslib/strings_boost
require llvm/daslib/llvm_tune

// The child fixture of llvm_tune.das's cross-grid tests, spawned
// under DAS_TUNE_MODE=test: main prints each family's registry rows
// and the build's tune identity.

[tune_perm(k = 1, requires = "simd128"),
 tune_perm(k = 2, requires = "llvm-tune-test-no-such-feature"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1")]
def cross_mixed(a, b : int) : int => a + b

[tune_perm(k = 3, requires = "llvm-tune-test-no-such-feature"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k3")]
def cross_seatless(a, b : int) : int => a + b

[tune_perm(k = 4, requires = "avx2|neon"),
 tune_perm(k = 5, requires = "!avx2,!neon"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k4")]
def cross_by_class(a, b : int) : int => a + b

[export]
def main {
    var mixed <- cross_mixed_variants()
    print("MIXED {join([for (v in mixed); v._0], ",")}\n")
    delete mixed
    var seatless <- cross_seatless_variants()
    print("SEATLESS {join([for (v in seatless); v._0], ",")}\n")
    delete seatless
    var byClass <- cross_by_class_variants()
    print("BYCLASS {join([for (v in byClass); v._0], ",")}\n")
    delete byClass
    print("BOX {tune_box_identity()}\n")
}
