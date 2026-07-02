options gen2

require dastest/testing_boost
require daslib/ast
require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/lint_config
require daslib/strings_boost
require strings

def fixture_path() : string {
    return "{get_das_root()}/tests/lint/_collect_issues_fixture.das"
}

def check_issue(t : T?; issue : LintIssue; code : string; line : int; needle : string) {
    t |> equal(issue.code, code)
    t |> equal(issue.line, line)
    t |> success(issue.column > 0)
    t |> success(issue.file |> ends_with("_collect_issues_fixture.das"))
    t |> success(find(issue.message, needle) >= 0)
}

[test]
def test_collect_issues(t : T?) {
    t |> run("*_collect_issues emit structured twins of the string collectors") @@(t : T?) {
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.export_all = true
                cop.lint_check = true
                cop.no_optimizations = true
                cop.no_infer_time_folding = true
                compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                    if (!ok) {
                        t |> failure("fixture failed to compile: {issues}")
                        return
                    }
                    let empty_set : table<string>

                    var paranoid_issues : array<LintIssue>
                    let paranoid_n = paranoid_collect_issues(program, paranoid_issues, empty_set, empty_set)
                    t |> equal(paranoid_n, 1)
                    t |> equal(length(paranoid_issues), 1)
                    check_issue(t, paranoid_issues[0], "LINT003", 9, "can_be_const")

                    var perf_issues : array<LintIssue>
                    let perf_n = perf_lint_collect_issues(program, perf_issues, empty_set, empty_set)
                    t |> equal(perf_n, 1)
                    t |> equal(length(perf_issues), 1)
                    check_issue(t, perf_issues[0], "PERF018", 11, "direct iteration")

                    var style_issues : array<LintIssue>
                    let style_n = style_lint_collect_issues(program, style_issues, empty_set, empty_set)
                    t |> equal(style_n, 1)
                    t |> equal(length(style_issues), 1)
                    check_issue(t, style_issues[0], "STYLE024", 14, "redundant 'unsafe(...)' wrap")

                    // string collectors stay the source of truth — same finding,
                    // same order, message embedded in the display string
                    var paranoid_strings : array<string>
                    let strings_n = paranoid_collect(program, paranoid_strings, empty_set, empty_set)
                    t |> equal(strings_n, paranoid_n)
                    t |> success(paranoid_strings[0] |> starts_with(paranoid_issues[0].message))
                }
            }
        }
    }
}
