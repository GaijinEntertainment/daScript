options gen2

require ./review_nttp // compile-time gate, no runtime symbol to reference
require dastest/review_gate

// The mechanical half of src/builtin/REVIEW.md (contract:
// REVIEW_COMMON.md at the repo root). Run from the repo root:
// bin/daslang src/builtin/REVIEW.das. The bind-flavor scan runs at
// compile time through review_nttp; inlineOnlyModules there is its
// one list.

[export]
def main => gate_verdict("src/builtin")
