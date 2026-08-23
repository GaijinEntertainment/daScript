options gen2

require ./review_nttp
require dastest/review_gate

// The mechanical half of src/builtin/REVIEW.md (contract: REVIEW_COMMON.md at
// the repo root). Run from the repo root: bin/daslang src/builtin/REVIEW.das —
// exit 0 clean, nonzero with findings (the NTTP-policy scan runs at compile
// time in ./review_nttp, since bind flavor only exists in the module registry).

[export]
def main {
    return gate_verdict("src/builtin")
}
