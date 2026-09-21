options gen2

require strings
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasLLAMA/harness/REVIEW.md (contract: REVIEW_COMMON.md at the
// repo root). Run from the repo root: bin/daslang modules/dasLLAMA/harness/REVIEW.das - exit 0
// clean, 1 with findings.

let private TUNE_KERNELS = "modules/dasLLAMA/harness/tune_kernels.das"
let private LAST_BENCH = "dot_q8q8_laneq4x4"

// Running that bench pins one matmul backend for the rest of the process, so it stays the LAST
// row of the `benches` list: any CPU timing after it would run against the pinned backend.
def private check_last_bench_row {
    let text = fread(TUNE_KERNELS)
    let open = find(text, "var benches <- [")
    if (open < 0) {
        gate_finding(TUNE_KERNELS, "no `var benches <- [` list - the last-row gate reads the bench list from it")
        return
    }
    let close = find(text, "\n    ]", open)
    if (close < 0) {
        gate_finding(TUNE_KERNELS, "the `benches` list has no closing `]` line at its indentation")
        return
    }
    let body = slice(text, open, close)
    let last_row = rfind(body, "(\"")
    if (last_row < 0) {
        gate_finding(TUNE_KERNELS, "the `benches` list carries no (\"name\", @@bench) row")
        return
    }
    let name_end = find(body, "\"", last_row + 2)
    let last_name = name_end < 0 ? "" : slice(body, last_row + 2, name_end)
    if (last_name != LAST_BENCH) {
        gate_finding(TUNE_KERNELS, "`{LAST_BENCH}` is not the LAST row of `benches` (the last row is `{last_name}`) - that bench pins one matmul backend for the rest of the process, so every CPU timing after it runs against the pinned backend")
    }
}

[export]
def main() : int {
    if (!fexist(TUNE_KERNELS)) {
        to_log(LOG_ERROR, "modules/dasLLAMA/harness/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_last_bench_row()
    return gate_verdict("dasllama-harness")
}
