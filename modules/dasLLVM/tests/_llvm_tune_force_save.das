options gen2

require llvm/daslib/llvm_tune

// Saver fixture for the forced-file test of llvm_tune.das: asks the
// tune framework to write a winner into the sidecar this run names.

[export]
def main() {
    print("SAVED {tune_manifest_set("fat_add", "k1")}\n")
}
