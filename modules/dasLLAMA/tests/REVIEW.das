options gen2

require dastest/review_gate
require dastest/test_shape

// The mechanical half of modules/dasLLAMA/tests/REVIEW_FILES.md
// (contract: REVIEW_COMMON.md at the repo root). Run from the repo
// root: bin/daslang modules/dasLLAMA/tests/REVIEW.das - exit 0
// clean, 1 with findings. The root is judged tests-only: run.das,
// the runner, is a script beside the suites it runs.

[export]
def main() : int {
    check_test_shape("modules/dasLLAMA/tests", 100, true)
    return gate_verdict("modules/dasLLAMA/tests")
}
