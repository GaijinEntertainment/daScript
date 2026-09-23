options gen2

require strings
require daslib/strings_boost
require daslib/fio
require daslib/regex
require daslib/regex_boost
require dastest/review_gate
require verify_core

// The mechanical half of utils/internal/dasweb-verify/REVIEW.md (contract: REVIEW_COMMON.md at
// the repo root). Run from the repo root: bin/daslang utils/internal/dasweb-verify/REVIEW.das -
// exit 0 clean, 1 with findings.

let private VERIFY_CORE = "utils/internal/dasweb-verify/verify_core.das"
let private STAGE_CMAKE = "web/stage_playground_imgui_samples.cmake"
let private SAMPLES_PREFIX = "examples/"

var private DEST_PATH <- %regex~"[$]\{DEST\}/([^"]+\.das)"%%
var private REPO_PATH <- %regex~"[$]\{REPO\}/([^"]+\.das)"%%

def private group1_captures(var re : Regex&; text : string) : table<string> {
    var found : table<string>
    regex_foreach(re, text) $(_at) {
        found |> insert(regex_group(re, 1, text))
        return true
    }
    return <- found
}

//! The file the stage script writes next after offset `at`; "" when it writes none.
def private next_staged(cmake : string; at : int) : string {
    let m = regex_search(DEST_PATH, cmake, at)
    return m.x < 0 ? "" : regex_group(DEST_PATH, 1, cmake)
}

[arch(at = "README.md#generated-samples")]
def private check_generated_samples_match {
    let core = fread(VERIFY_CORE)
    let cmake = fread(STAGE_CMAKE)
    var inscope cmake_dests <- group1_captures(DEST_PATH, cmake)
    var inscope cmake_sources <- group1_captures(REPO_PATH, cmake)
    var inscope table_sources : table<string>
    for (sample, source in keys(GENERATED_SAMPLES), values(GENERATED_SAMPLES)) {
        table_sources |> insert(source)
        let line = find_line(core, "\"{sample}\"")
        if (!starts_with(sample, SAMPLES_PREFIX)) {
            gate_finding(VERIFY_CORE, line, "GENERATED_SAMPLES key `{sample}` is not under `{SAMPLES_PREFIX}`, so {STAGE_CMAKE} cannot stage it")
            continue
        }
        let dest = slice(sample, length(SAMPLES_PREFIX))
        let read_at = find(cmake, "\"$\{REPO\}/{source}\"")
        if (read_at < 0) {
            gate_finding(VERIFY_CORE, line, "GENERATED_SAMPLES maps `{sample}` to `{source}`, but {STAGE_CMAKE} reads no `$\{REPO\}/{source}` - match the stage script's source, in this change")
            continue
        }
        let staged = next_staged(cmake, read_at)
        if (staged != dest) {
            gate_finding(VERIFY_CORE, line, "GENERATED_SAMPLES maps `{sample}` to `{source}`, but {STAGE_CMAKE} stages that source as `$\{DEST\}/{staged}` - make the two agree, in this change")
        }
    }
    for (dest in keys(cmake_dests)) {
        if (!key_exists(GENERATED_SAMPLES, "{SAMPLES_PREFIX}{dest}")) {
            gate_finding(STAGE_CMAKE, find_line(cmake, "/{dest}\""), "writes `$\{DEST\}/{dest}`, but {VERIFY_CORE}'s GENERATED_SAMPLES has no `{SAMPLES_PREFIX}{dest}` entry - add it, in this change")
        }
    }
    for (source in keys(cmake_sources)) {
        if (!key_exists(table_sources, source)) {
            gate_finding(STAGE_CMAKE, find_line(cmake, "/{source}\""), "reads `$\{REPO\}/{source}`, but no GENERATED_SAMPLES entry in {VERIFY_CORE} names it as a source - add or correct the entry, in this change")
        }
    }
}

[export]
def main() : int {
    check_generated_samples_match()
    return gate_verdict("dasweb-verify")
}
