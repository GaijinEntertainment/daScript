options gen2

require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasVulkan/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang modules/dasVulkan/REVIEW.das - exit 0
// clean, 1 with findings.

[export]
def main() : int {
    if (!fexist("modules/dasVulkan/REVIEW.das")) {
        to_log(LOG_ERROR, "modules/dasVulkan/REVIEW.das: run from the repo root\n")
        return 2
    }
    let non_module_tokens <- { "vulkan", "Module_dasVULKAN" }
    gate_descriptor_census("modules/dasVulkan/.das_module", ["modules/dasVulkan/daslib"], non_module_tokens)
    return gate_verdict("dasvulkan")
}
