options gen2

require daslib/tune
require daslib/strings_boost

// The child fixture for the cross-grid test of llvm_tune.das, spawned
// under DAS_TUNE_MODE=test: main prints each family's registry rows.

[tune_perm(k = 1, requires = "simd128"),
 tune_perm(k = 2, requires = "llvm-tune-test-no-such-feature"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1")]
def cross_mixed(a, b : int) : int => a + b

[tune_perm(k = 3, requires = "llvm-tune-test-no-such-feature"),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k3")]
def cross_seatless(a, b : int) : int => a + b

[export]
def main {
    static_if (typeinfo module_exists(llvm_tune)) {
        var mixed <- cross_mixed_variants()
        print("MIXED {join([for (v in mixed); v._0], ",")}\n")
        delete mixed
        var seatless <- cross_seatless_variants()
        print("SEATLESS {join([for (v in seatless); v._0], ",")}\n")
        delete seatless
    }
}
