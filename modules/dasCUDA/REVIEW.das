options gen2

require dastest/review_gate

// The mechanical half of modules/dasCUDA/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang modules/dasCUDA/REVIEW.das - exit 0 clean,
// 1 with findings.

[export]
def main : int {
    gate_reset()
    gate_descriptor_census("modules/dasCUDA/.das_module", ["modules/dasCUDA/cuda"], { "cuda", "Module_CUDA" })
    return gate_verdict("dasCUDA")
}
