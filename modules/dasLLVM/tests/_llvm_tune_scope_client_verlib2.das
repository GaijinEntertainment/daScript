options gen2

module _llvm_tune_scope_client_verlib2 public

require daslib/tune public
require llvm/daslib/llvm_tune public
require _llvm_tune_scope_verlib  // nolint:STYLE030 — version_of= below names TESTVER by string

// version_key= override fixture for llvm_tune_scope.das: the same pin
// constant as verlib, recorded under the custom provenance key "tv2" instead of the
// lowercased const name.
[tune_scope(name = "llvm_tune_scope_verprobe2", tuner = "./_llvm_tune_scope_tuner_ver2.das",
            version_of = "_llvm_tune_scope_verlib/TESTVER", version_key = "tv2")]
struct private ScopeDecl {}

[tune_perm(k = 0), tune_perm(k = 1), tune_perm(k = 2),
 tune(gen = "llvm_code_selftest::add_plus_k", fallback = "k1")]
def ver2_add(a, b : int) : int => a + b
