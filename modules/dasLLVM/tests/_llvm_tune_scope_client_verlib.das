options gen2

module _llvm_tune_scope_client_verlib public

require daslib/tune public
require llvm/daslib/llvm_tune public
require _llvm_tune_scope_verlib  // nolint:STYLE030 — version_of= below names TESTVER by string

// Version-pinned scope fixture for llvm_tune_scope.das: winners are
// valid only while the sidecar's provenance.testver equals TESTVER's current folded value.
[tune_scope(name = "llvm_tune_scope_verprobe", tuner = "./_llvm_tune_scope_tuner_ver.das",
            version_of = "_llvm_tune_scope_verlib/TESTVER")]
struct private ScopeDecl {}

[tune_perm(k = 0), tune_perm(k = 1), tune_perm(k = 2),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1")]
def ver_add(a, b : int) : int => a + b
