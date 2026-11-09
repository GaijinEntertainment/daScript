options gen2

require dastest/review_gate
require dastest/test_shape

// The mechanical half of modules/dasVulkan/tests/REVIEW.md (contract:
// REVIEW_COMMON.md at the repo root). Run from the repo root:
// bin/daslang modules/dasVulkan/tests/REVIEW.das - exit 0 clean, 1
// with findings. The tutorials are a tool tree: each tutorial's
// program and shaders sit beside its one pixel-oracle test, so only
// the tests are judged there.

[export]
def main() : int {
    check_test_shape("modules/dasVulkan/tests", 100)
    check_test_shape("modules/dasVulkan/tutorials", 100, true)
    return gate_verdict("modules/dasVulkan/tests")
}
