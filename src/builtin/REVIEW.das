options gen2

require ./review_nttp // nolint:STYLE030 — compile-time gate, no runtime symbol to reference
require dastest/review_gate

// The mechanical half of src/builtin/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang src/builtin/REVIEW.das.

[export]
def main {
    return gate_verdict("src/builtin")
}
