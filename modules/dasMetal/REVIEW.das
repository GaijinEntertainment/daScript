options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of modules/dasMetal/REVIEW.md (contract: REVIEW_COMMON.md at the repo
// root). Run from the repo root: bin/daslang modules/dasMetal/REVIEW.das — exit 0 clean,
// 1 with findings.

let private EMITTER = "modules/dasMetal/metal/msl_emit.das"

// every matmul2d_descriptor the emitter writes sets relaxed_precision = true (arg 6) — an
// RP = false descriptor runs the op off the tensor-unit fast path
def private check_rp_law {
    var line_no = 0
    for (line in split(fread(EMITTER), "\n")) {
        line_no++
        let at = find(line, "matmul2d_descriptor(")
        continue if (at < 0)
        let args <- split(slice(line, at + length("matmul2d_descriptor(")), ",")
        if (length(args) < 6) {
            gate_finding(EMITTER, line_no, "matmul2d_descriptor with fewer than 6 arguments - the relaxed_precision position is missing")
            continue
        }
        if (strip(args[5]) != "true" && !(strip(args[5]) |> starts_with("true)"))) {
            gate_finding(EMITTER, line_no, "matmul2d_descriptor with relaxed_precision != true - RP = false keeps the op off the tensor-unit fast path")
        }
    }
}

[export]
def main() : int {
    if (!fexist("modules/dasMetal/REVIEW.das") || !fexist(EMITTER)) {
        to_log(LOG_ERROR, "modules/dasMetal/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_rp_law()
    return gate_verdict("dasmetal")
}
