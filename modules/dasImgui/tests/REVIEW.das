options gen2

require dastest/review_gate
require dastest/test_shape

// The mechanical half of modules/dasImgui/tests/REVIEW.md (contract:
// REVIEW_COMMON.md at the repo root). Run from the repo root:
// bin/daslang modules/dasImgui/tests/REVIEW.das - exit 0 clean, 1
// with findings. The root is judged tests-only: the recording
// drivers, the lint smokes and the feature apps the tests spawn sit
// beside the suites as bare scripts, not as tests that forgot their
// annotation.

[export]
def main() : int {
    check_test_shape("modules/dasImgui/tests", 100, true)
    return gate_verdict("modules/dasImgui/tests")
}
