options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of ci/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang ci/REVIEW.das - exit 0 clean, 1 with findings.

def private read_text(path : string) : string {
    var text = ""
    fopen(path, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    return text
}

def private workflows_text() : string {
    var all : array<string>
    dir(".github/workflows") $(name) {
        return if (!(name |> ends_with(".yml")))
        all |> push(read_text(path_join(".github/workflows", name)))
    }
    return join(all, "\n")
}

//! every ci/test_*.py is named by a workflow step - a pin no row runs never fires
def private check_tests_wired(workflows : string) {
    dir("ci") $(name) {
        return if (!(name |> starts_with("test_")) || !(name |> ends_with(".py")))
        if (find(workflows, "ci/{name}") < 0) {
            gate_finding("ci/{name}", "no .github/workflows step names ci/{name} - a test no CI row runs never runs again; add the step (extended_checks.yml's core role carries the python gates)")
        }
    }
}

[export]
def main() : int {
    if (!fexist("ci/REVIEW.das") || !stat(".github/workflows").is_dir) {
        to_log(LOG_ERROR, "ci/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_tests_wired(workflows_text())
    return gate_verdict("ci")
}
