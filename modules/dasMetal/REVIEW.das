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

let private FC_DIR = "tests/msl/_fail_closed"
let private FC_TEST = "tests/msl/test_msl_fail_closed.das"

def private check_fail_closed_sync {
    var fixtures : table<string>
    dir(FC_DIR) $(name) {
        if (name |> ends_with(".das")) {
            fixtures |> insert(slice(name, 0, length(name) - 4))
        }
    }
    var asserted : table<string; int>
    var line_no = 0
    for (line in split(fread(FC_TEST), "\n")) {
        line_no++
        let at = find(line, "check_rejects(t, \"")
        continue if (at < 0)
        let rest = slice(line, at + length("check_rejects(t, \""))
        let q = find(rest, "\"")
        continue if (q <= 0)
        asserted[slice(rest, 0, q)] = line_no
    }
    for (fx in keys(fixtures)) {
        if (!key_exists(asserted, fx)) {
            gate_finding(FC_TEST, 1, "fixture {FC_DIR}/{fx}.das has no check_rejects assert - its rejection is unpinned")
        }
    }
    for (name, ln in keys(asserted), values(asserted)) {
        if (!key_exists(fixtures, name)) {
            gate_finding(FC_TEST, ln, "check_rejects(\"{name}\") has no {FC_DIR}/{name}.das fixture - the assert is stranded")
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
    check_fail_closed_sync()
    return gate_verdict("dasmetal")
}
