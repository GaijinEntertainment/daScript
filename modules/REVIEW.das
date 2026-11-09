options gen2

require dastest/review_gate
require dastest/test_shape

// The mechanical half of modules/REVIEW.md (contract:
// REVIEW_COMMON.md at the repo root). Run from the repo root:
// bin/daslang modules/REVIEW.das - exit 0 clean, 1 with findings. It
// gates the module test folders that carry no REVIEW.md of their own;
// a module whose tests folder has one (dasImgui, dasLLVM, dasVulkan)
// gates its tests there.

let TEST_ROOTS <- [
    "modules/dasAudio/tests", "modules/dasGLTF/tests", "modules/dasGlfw/tests",
    "modules/dasLR1/tests", "modules/dasLiveHost/tests", "modules/dasMeshOptimizer/tests",
    "modules/dasOPENAI/tests", "modules/dasOpenGL/tests", "modules/dasPEG/tests",
    "modules/dasStbImage/tests"
]

[export]
def main() : int {
    for (root in TEST_ROOTS) {
        check_test_shape(root, 100)
    }
    return gate_verdict("modules")
}
