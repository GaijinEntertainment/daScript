options gen2

require dastest/review_gate
require dastest/test_shape

// The mechanical half of dastest/tests/REVIEW.md (contract:
// REVIEW_COMMON.md at the repo root). Run from the repo root:
// bin/daslang dastest/tests/REVIEW.das - exit 0 clean, 1 with
// findings.

[export]
def main() : int {
    check_test_shape("dastest/tests", 100)
    return gate_verdict("dastest/tests")
}
