options gen2
options no_aot

// Driver for the SMT lint fixtures. Each fixture marks the lines it expects to be
// reported with a trailing `// smt_expect <CODE>` comment; this compares that set
// against what the pass actually reports, so a missed detection and a false
// positive both fail.
//
// The fixtures MUST be compiled with optimizations off. The optimizer rewrites
// `if (c) { return x }` followed by `return y` into `return c ? x : y`, which
// leaves no if-node for the pass to look at — the same reason utils/lint/main.das
// sets these policies.

require smt/daslib/smt_lint
require daslib/ast_boost
require daslib/strings_boost
require daslib/fio
require strings
require daslib/rtti
require daslib/lint_config

struct Expectation {
    line : int
    code : string
}

var g_failures = 0

def fail(msg : string) {
    g_failures++
    to_log(LOG_ERROR, "FAIL {msg}\n")
}

def read_expectations(path : string) : array<Expectation> {
    var want : array<Expectation>
    let body = fread(path)
    let lines <- split(body, "\n")
    for (i in range(length(lines))) {
        let marker = find(lines[i], "smt_expect ")
        if (marker >= 0) {
            let rest = slice(lines[i], marker + length("smt_expect "))
            want |> emplace(Expectation(line = i + 1, code = "{strip(rest)}"))
        }
    }
    return <- want
}

def lint_fixture(path : string) : array<LintIssue> {
    var reports : array<LintIssue>
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    fail("{path} did not compile: {issues}")
                    return
                }
                let disabled : table<string>
                let enabled : table<string>
                var found : array<LintIssue>
                smt_lint_collect_issues(program, found, disabled, enabled)
                reports |> push_clone_from(found)
            }
        }
    }
    return <- reports
}

def check_fixture(path : string) {
    let want <- read_expectations(path)
    let reports <- lint_fixture(path)
    to_log(LOG_INFO, "{base_name(path)}: {length(want)} expected, {length(reports)} reported\n")

    var matched : array<int>
    for (r in reports) {
        var found = false
        for (e in want) {
            if (e.line == r.line && e.code == r.code) {
                found = true
                break
            }
        }
        if (found) {
            matched |> push(r.line)
        } else {
            fail("unexpected {r.code} at {base_name(path)}:{r.line} — {r.message}")
        }
    }
    for (e in want) {
        var seen = false
        for (m in matched) {
            if (m == e.line) {
                seen = true
                break
            }
        }
        if (!seen) {
            fail("missing {e.code} at {base_name(path)}:{e.line}")
        }
    }
}

[export]
def main() : int {
    let here = dir_name(get_das_root() + "/modules/dasSMT/tests/x")
    for (f in ["smt001_unreachable_branch.das", "smt002_always_true_condition.das",
               "smt003_definite_defects.das"]) {
        check_fixture(path_join(here, f))
    }
    if (g_failures != 0) {
        to_log(LOG_ERROR, "smt_lint: {g_failures} failure(s)\n")
        return 1
    }
    to_log(LOG_INFO, "smt_lint: all fixtures OK\n")
    return 0
}
