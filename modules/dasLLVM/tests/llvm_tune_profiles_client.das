options gen2

require llvm_tune_profiles_lib

// App-face fixture for llvm_tune_profiles.das (no [test] on purpose — the suite just
// compiles it): default policy (auto), so an untuned start walks adopt -> filtered race ->
// re-exec. RESULT is the k-fingerprint of the stamped tier (jit: 2+3+k; interp reference: 5).
[export]
def main : int {
    var n = 2  // nolint:LINT003 — var defeats const-fold so the call survives to codegen
    print("RESULT {prof_add(n, 3)}\n")
    var st <- tune_status()
    for (s in st) {
        print("STATUS {s.fname}|{s.scope}|{s.suffix}|{s.source}\n")
    }
    delete st
    return 0
}
